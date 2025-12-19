/*

sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c em++ -I ./ -Os -s STANDALONE_WASM -s INITIAL_MEMORY=16121856 -s STACK_SIZE=1048576 -s EXPORTED_FUNCTIONS="['_getInputBuffer','_getQueryBuffer','_getOutputBuffer','_getZobristHashBuffer','_getTranspositionTableBuffer','_getNegamaxSearchBuffer','_getAuxiliaryBuffer','_getKillerMovesBuffer','_getHistoryBuffer','_initSearch','_negamax']" -Wl,--no-entry "negamax.cpp" -o "negamax.wasm"

*/

#include "transposition.h"                                          /* Include the Transposition Table library. */
#include "zobrist.h"                                                /* Include the Zobrist hasher, which is an array of unsigned long longs (64-bit ints). */

#define _MAX_MOVES                 64                               /* A (generous) upper bound on how many moves may be made by a team in a single turn. */
#define _NONE                      64                               /* Required as a "blank" value without #include "gamestate.h". */
#define _NO_PROMO                   0                               /* Required as a "blank" value without #include "gamestate.h". */

#define _KILLER_MOVE_PER_PLY        2                               /* Typical for other chess engines. */
#define _KILLER_MOVE_MAX_DEPTH     64                               /* Simply something "comfortably large". */

#define _TREE_SEARCH_ARRAY_SIZE 65536                               /* Number of (game-state bytes, move-bytes). */
#define _NEGAMAX_NODE_BYTE_SIZE   130                               /* Number of bytes needed to store a negamax node. */

#define _PHASE_ENTER_NODE           0                               /* Indicating that transpo-check, terminal-check, null move, IID, early eval/pruning are to follow. */
#define _PHASE_GEN_AND_ORDER        1                               /* Indicating that move-generation, basic scoring / ordering are to follow. */
#define _PHASE_NEXT_MOVE            2                               /* Indicating that next-move picking, reduction-decision, child-node pushing are to follow. */
#define _PHASE_AFTER_CHILD          3                               /* Indicating that the child-return routine (update alpha/beta, killers, history, TT) is to follow. */
#define _PHASE_FINISH_NODE          4                               /* Indicating that the final TT store, parent update, then pop are to follow. */

#define NN_FLAG_NULL_TRIED       0x01                               /* Indicates that we already tried a null move here. */
#define NN_FLAG_NULL_IN_PROGRESS 0x02                               /* Indicates that there is currently a null-move child. */
#define NN_FLAG_IS_NULL_CHILD    0x04                               /* Indicates that this node itself was reached via null move. */

#define NN_FLAG_IS_PV            0x08                               /* Indicates a PV node. */
#define NN_FLAG_AT_ROOT          0x10                               /* Indicates a root node. */
#define NN_FLAG_IN_CHECK         0x20                               /* Indicates that the side to move is in check here (cached). */
                                                                    /* Convenience macros. */
#define NN_SET_FLAG(node, f)    ((node)->flags |=  (f))
#define NN_CLEAR_FLAG(node, f)  ((node)->flags &= ~(f))
#define NN_HAS_FLAG(node, f)   (((node)->flags &   (f)) != 0)

#define NULL_MOVE_BASE_REDUCTION    2                               /*  */
#define NULL_MOVE_EXTRA_REDUCTION   1                               /*  */

//#define _PHASE_TRANSPO_CHECK        0                               /* Indicating Transpo-Check is to follow. */
//#define _PHASE_NULL_MOVE_PRUNING    1                               /* Indicating Null-Move Pruning is to follow. */
//#define _PHASE_INTERNAL_ITER_DEEP   2                               /* Indicating Internal Iterative Deepening is to follow. */
//#define _PHASE_EXPANSION            3                               /* Indicating Expansion is to follow. */
//#define _PHASE_MOVE_ORDERING        4                               /* Indicating Move-Ordering is to follow. */
//#define _PHASE_LATE_MOVE_REDUCTION  5                               /* Indicating Late-Move-Reduction is to follow. */
//#define _PHASE_EVALUATION           6                               /* Indicating Evaluation is to follow. */
//#define _PHASE_KILLER_MOVES_UPDATE  7                               /* Indicating Killer-Move Updating is to follow. */
//#define _PHASE_HISTORY_UPDATE       8                               /* Indicating History-Updating is to follow. */
//#define _PHASE_TRANSPO_UPDATE       9                               /* Indicating Transpo-Update is to follow. */
//#define _PHASE_PARENT_UPDATE       10                               /* Indicating Parent-Update is to follow. */

/**************************************************************************************************
 Typedefs  */

typedef struct NegamaxNodeType                                      //  TOTAL: 130 = _NEGAMAX_NODE_BYTE_SIZE bytes.
  {
    //unsigned char gs[_GAMESTATE_BYTE_SIZE];                         //  (81 bytes) The given gamestate as a byte array.
    unsigned int parent;                                            //  (4 bytes) Index into "negamaxSearchBuffer" of this node's parent.
    unsigned char parentMove[_MOVE_BYTE_SIZE];                      //  (3 bytes) Describe the move that led from "parent" to this node.
    unsigned char bestMove[_MOVE_BYTE_SIZE];                        //  (3 bytes) Best move found so far from this node.
    unsigned int childrenCtr;                                       //  (4 bytes) Number of children.
    //unsigned int moveOffset;                                        //  (4 bytes) Index into a global move buffer.
    //unsigned int moveCount;                                         //  (4 bytes) How many moves generated for this node.
    //Suggests that childrenCtr should become the current move index; how many we've already searched.
    signed char depth;                                              //  (1 byte) Counts down to 0, or to -1 if quiescence searching.

    float originalAlpha;                                            //  (4 bytes) The alpha value saved at the top of the negamax call.
    float alpha;                                                    //  (4 bytes) Upper bound.
    float beta;                                                     //  (4 bytes) Lower bound.
    float color;                                                    //  (4 bytes) Either +1.0 or -1.0, for max or min respectively.

    unsigned long long zhash;                                       //  (8 bytes) Zobrist hash of the given gamestate byte array.
    unsigned int hIndex;                                            //  (4 bytes) Zobrist hash modulo table length, adjusted as necessary using linear probing.

    unsigned char phase;                                            //  (1 byte) In {_PHASE_ENTER_NODE, _PHASE_GEN_AND_ORDER, _PHASE_NEXT_MOVE,
                                                                    //               _PHASE_AFTER_CHILD, _PHASE_FINISH_NODE}.
    unsigned char flags;                                            //  (1 byte) Covers [NN_FLAG_NULL_TRIED, NN_FLAG_NULL_IN_PROGRESS, NN_FLAG_IS_NULL_CHILD,
                                                                    //                   NN_FLAG_IS_PV, NN_FLAG_AT_ROOT, NN_FLAG_IN_CHECK].
    float value;                                                    //  (4 bytes) Value computed over this node's children and/or eventually returned.
  } NegamaxNode;

/**************************************************************************************************
 Prototypes  */

 /*  Imported from the Player object's evaluationEngine module.
     This gives you linguistic independence: leave tree-search to C++. */
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Prepare to query the Evaluation Engine by copying the Negamax Engine's
                                                                    //  query-gamestate-buffer contents into the Evaluation Engine's input-gamestate buffer.
__attribute__((import_module("env"), import_name("_copyQuery2EvalGSInput"))) void copyQuery2EvalGSInput();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Prepare to query the Evaluation Engine by copying the Negamax Engine's
                                                                    //  query-move-buffer contents into the Evaluation Engine's input-move buffer.
__attribute__((import_module("env"), import_name("_copyQuery2EvalMoveInput"))) void copyQuery2EvalMoveInput();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Copy the Evaluation Engine's output-gamestate
                                                                    //  to the Negamax Engine's answer-gamestate buffer.
__attribute__((import_module("env"), import_name("_copyEvalOutput2AnswerGSBuffer"))) void copyEvalOutput2AnswerGSBuffer();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Copy the Evaluation Engine's output-moves
                                                                    //  to the Negamax Engine's answer-moves buffer.
__attribute__((import_module("env"), import_name("_copyEvalOutput2AnswerMovesBuffer"))) void copyEvalOutput2AnswerMovesBuffer();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Is the GameState encoded in Evaluation Engine's input buffer quiet?
__attribute__((import_module("env"), import_name("_isQuiet"))) bool isQuiet();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Is the GameState encoded in Evaluation Engine's input buffer terminal?
__attribute__((import_module("env"), import_name("_isTerminal"))) bool isTerminal();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Is the side to move in the GameState encoded in Evaluation Engine's input buffer in check?
__attribute__((import_module("env"), import_name("_isSideToMoveInCheck"))) bool isSideToMoveInCheck();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  How much non-pawn material does the side to move in the GameState encoded in Evaluation Engine's input buffer have?
__attribute__((import_module("env"), import_name("_nonPawnMaterial"))) unsigned char nonPawnMaterial();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Make Evaluation Engine write the child-state bytes resulting from the given move
                                                                    //  to the Evaluation Engine's output buffer.
__attribute__((import_module("env"), import_name("_makeMove"))) void makeMove();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Make Evaluation Engine write the child-state, bytes resulting from a null move
                                                                    //  to the Evaluation Engine's output buffer.
__attribute__((import_module("env"), import_name("_makeNullMove"))) void makeNullMove();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Evaluate the GameState encoded in Evaluation Engine's input buffer?
__attribute__((import_module("env"), import_name("_evaluate"))) float evaluate();
                                                                    //  Bridge between WebAssembly Modules:
                                                                    //  Query the Evaluation Engine.
                                                                    //  Make Evaluation Engine write a sorted list of (child-state, move) tuples
                                                                    //  to the Evaluation Engine's output buffer.
__attribute__((import_module("env"), import_name("_getMoves"))) unsigned int getMoves();
                                                                    //  Add a count to the running total of nodes searched.
__attribute__((import_module("env"), import_name("_incrementNodeCtr"))) void incrementNodeCtr(unsigned int);

extern "C"
  {
    unsigned char* getInputBuffer(void);
    unsigned char* getOutputBuffer(void);
    unsigned char* getQueryGameStateBuffer(void);
    unsigned char* getQueryMoveBuffer(void);
    unsigned char* getAnswerGameStateBuffer(void);
    unsigned char* getAnswerMovesBuffer(void);
    unsigned char* getZobristHashBuffer(void);
    unsigned char* getTranspositionTableBuffer(void);
    unsigned char* getNegamaxSearchBuffer(void);
    unsigned char* getKillerMovesTableBuffer(void);
    unsigned char* getHistoryTableBuffer(void);
    void initSearch(unsigned char);
    bool negamax(void);
  }

void transpoCheck_step(unsigned int, NegamaxNode*);
void evaluation_step(unsigned int, NegamaxNode*);
void expansion_step(unsigned int, NegamaxNode*);
void parentUpdate_step(unsigned int, NegamaxNode*);

unsigned int restoreNegamaxSearchBufferLength(void);
void saveNegamaxSearchBufferLength(unsigned int);
void restoreNode(unsigned int, NegamaxNode*);
void saveNode(NegamaxNode*, unsigned int);

unsigned long long hash(unsigned char*);

/**************************************************************************************************
 Globals  */
                                                                    //  81 bytes.
                                                                    //  Global array containing the serialized game state:
unsigned char inputGameStateBuffer[_GAMESTATE_BYTE_SIZE];           //  Input from Player.js to its negamaxEngine.

                                                                    //  89 bytes.
                                                                    //  Global array containing: {serialized game state,
                                                                    //                            1-byte uchar,
                                                                    //                            serialized move,
                                                                    //                            4-byte float}:
                                                                    //  Output from negamaxEngine to Player.js.
unsigned char outputBuffer[_GAMESTATE_BYTE_SIZE + 1 + _MOVE_BYTE_SIZE + 4];

                                                                    //  81 bytes.
                                                                    //  Global array containing the serialized (query) game state:
unsigned char queryGameStateBuffer[_GAMESTATE_BYTE_SIZE];           //  Input from negamaxEngine to evaluationEngine.

                                                                    //  3 bytes.
                                                                    //  Global array containing the serialized (query) move:
unsigned char queryMoveBuffer[_MOVE_BYTE_SIZE];                     //  Input from negamaxEngine to evaluationEngine.

                                                                    //  81 bytes.
                                                                    //  Global array containing a serialized (answer) game state:
unsigned char answerGameStateBuffer[_GAMESTATE_BYTE_SIZE];          //  Output from evaluationEngine to negamaxEngine.

                                                                    //  452 bytes.
                                                                    //  Global array containing: {4-byte unsigned int,
                                                                    //                            serialized (answer-move, rough score)}:
                                                                    //  Output from evaluationEngine to negamaxEngine.
unsigned char answerMovesBuffer[4 + _MAX_MOVES * (_MOVE_BYTE_SIZE + 4)];

                                                                    //  6,008 bytes.
                                                                    //  For "zobristHashBuffer" included in "zobrist.h".

                                                                    //  6,488,068 bytes.
                                                                    //  For "transpositionTableBuffer" included in "transposition.h".

                                                                    //  8,519,684 bytes.
                                                                    //  Flat, global array that behaves like a DFS stack for negamax nodes.
                                                                    //  First four bytes are for an unsigned int: the length of the array.
unsigned char negamaxSearchBuffer[_TREE_SEARCH_ARRAY_SIZE * _NEGAMAX_NODE_BYTE_SIZE + 4];

                                                                    //  384 bytes.
                                                                    //  Each entry is [from_1, to_1, promo_1, from_2, to_2, promo_2].
unsigned char killerMovesTableBuffer[_KILLER_MOVE_PER_PLY * _MOVE_BYTE_SIZE * _KILLER_MOVE_MAX_DEPTH];

                                                                    //  8,192 bytes.
                                                                    //  2 is for 2 teams, white and black. So read an entry as (side, from, to).
unsigned char historyTableBuffer[2 * _NONE * _NONE];                //  Note that we don't care about promotion choices here; just bump up moves (from, to).

                                                                    //  SUBTOTAL:  15,021,951 bytes.
                                                                    //  Give the stack 1,048,576 bytes.
                                                                    //  TOTAL:     16,070,527 bytes.
                                                                    //  Round to:  16,121,856 bytes = 246 pages (cover units of 65,536).

/**************************************************************************************************
 Pointer-retrieval functions  */

/* Expose the global array declared here to JavaScript.
   Player.js writes bytes arrays here and then calls its "negamaxEngine". */
unsigned char* getInputBuffer(void)
  {
    return &inputGameStateBuffer[0];
  }

/* Expose the global array declared here to JavaScript.
   "negamaxEngine" writes its output here for Player.js to pick up: {gamestate as byte array, unsigned int as byte array, move as byte array}.
   This stands for {child state, depth searched, move to play in reply}. */
unsigned char* getOutputBuffer(void)
  {
    return &outputBuffer[0];
  }

/* Expose the global array declared here to JavaScript (just so we can address it).
   Player's "negamaxEngine" writes a game states as a byte array here and then calls Player.js's "evaluationEngine". */
unsigned char* getQueryGameStateBuffer(void)
  {
    return &queryGameStateBuffer[0];
  }

/* Expose the global array declared here to JavaScript (just so we can address it).
   Player's "negamaxEngine" writes a move as a byte array here and then calls Player.js's "evaluationEngine". */
unsigned char* getQueryMoveBuffer(void)
  {
    return &queryMoveBuffer[0];
  }

/* Expose the global array decalred here to JavaScript (just so we can address it). */
unsigned char* getAnswerGameStateBuffer(void)
  {
    return &answerGameStateBuffer[0];
  }

/* Expose the global array decalred here to JavaScript (just so we can address it). */
unsigned char* getAnswerMovesBuffer(void)
  {
    return &answerMovesBuffer[0];
  }

/* Expose the global array declared here to JavaScript (just so we can fill it).
   Filled once and read-only throughout a match. */
unsigned char* getZobristHashBuffer(void)
  {
    return &zobristHashBuffer[0];                                   //  Defined in zobrist.h.
  }

/* Expose the global array declared in "transposition.h" to JavaScript (just so we can address it). */
unsigned char* getTranspositionTableBuffer(void)
  {
    return &transpositionTableBuffer[0];
  }

/* Expose the global array decalred here to JavaScript (just so we can address it). */
unsigned char* getNegamaxSearchBuffer(void)
  {
    return &negamaxSearchBuffer[0];
  }

/* Expose the global array decalred here to JavaScript (just so we can address it). */
unsigned char* getKillerMovesTableBuffer(void)
  {
    return &killerMovesTableBuffer[0];
  }

/* Expose the global array decalred here to JavaScript (just so we can address it). */
unsigned char* getHistoryTableBuffer(void)
  {
    return &historyTableBuffer[0];
  }

/**************************************************************************************************
 Negamax-search functions  */

/* Initialize the root node for (interrupatble) negamax search. */
void initSearch(unsigned char depth)
  {
    NegamaxNode root;
    unsigned int i;

    for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                       //  Copy root gamestate byte array from global "inputGameStateBuffer"
      root.gs[i] = inputGameStateBuffer[i];                         //  to negamax root node.

    root.parent = 0;                                                //  Set root's parent index to...

    root.parentMove[0] = _NONE;                                     //  Set the root's parent-move to a blank-move.
    root.parentMove[1] = _NONE;
    root.parentMove[2] = 0;

    root.bestMove[0] = _NONE;                                       //  Set the root's best-move to a blank-move.
    root.bestMove[1] = _NONE;
    root.bestMove[2] = 0;

    root.childrenCtr = 0;                                           //  Set root's child-count to zero.

    root.depth = (signed char)depth;                                //  Set root's depth to "depth".

    root.originalAlpha = -std::numeric_limits<float>::infinity();   //  Initialize root's originalAlpha.
    root.alpha = -std::numeric_limits<float>::infinity();           //  Initialize root's alpha.
    root.beta = std::numeric_limits<float>::infinity();             //  Initialize root's beta.
    root.color = 1.0;                                               //  Initialize root's color.

    root.zhash = 0L;                                                //  Initialize root's zhash.
    root.hIndex = 0;                                                //  Initialize root's hIndex.

    root.phase = _PHASE_ENTER_NODE;                                 //  Indicate that no work has been done on this node yet:
                                                                    //  the first thing to try is a transposition table lookup.
    root.flags = NN_FLAG_AT_ROOT;                                   //  Set root flag.
    root.value = -std::numeric_limits<float>::infinity();           //  Initialize root's value.

    saveNegamaxSearchBufferLength(1);                               //  Write number of NegamaxNodes in "negamaxSearchBuffer".
    saveNode(&root, 0);                                             //  Write serialized node to head of "negamaxSearchBuffer".

    return;
  }

/* HEARTBEAT NEGAMAX

   Depth-first search for a two-player, perfect-information, zero-sum game.
   Uses alpha-beta pruning and a transposition table.

   So that tree search does not overwhelm the client-side CPU, negamax must be redesigned in a "heartbeat" manner. */
bool negamax(void)
  {
    unsigned int gsIndex;
    NegamaxNode node, child;
    unsigned int i, j;
    unsigned char material;
    signed char R, newDepth;
    unsigned int negamaxSearchBufferLength;
    bool b;

    gsIndex = restoreNegamaxSearchBufferLength() - 1;               //  Index for top of stack is length minus one.
    restoreNode(gsIndex, &node);                                    //  Restore the node at the top of the stack.

    if(gsIndex == 0 && node.phase == _PHASE_PARENT_UPDATE)          //  If the root node has appeared at the top of the stack with this state, then we're done.
      {                                                             //  Recover data from search and write to "outputBuffer".
        i = 0;

        for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)                   //  Copy game state to output buffer.
          outputBuffer[i++] = node.gs[j];

        outputBuffer[i++] = (unsigned char)node.depth;              //  Copy depth to output buffer.

        for(j = 0; j < _MOVE_BYTE_SIZE; j++)                        //  Copy best move for this state, as determined by depth, to output buffer.
          outputBuffer[i++] = node.bestMove[j];

        //  Write the score to the last four bytes.

        return true;                                                //  Indicate that search has completed.
      }

    switch(node.phase)
      {
        //////////////////////////////////////////////////////////////  Nothing has been done with this node yet.
        case _PHASE_ENTER_NODE:
          transpoCheck_step(gsIndex, &node);                        //  - Attempt transpo lookup.
          if(node.phase == _PHASE_FINISH_NODE)                      //    Lookup succeeded: break.
            break;
                                                                    //  Copy "node"s "gs" to "queryGameStateBuffer"
          for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                 //  for isTerminal() and evaluate().
            queryGameStateBuffer[i] = node->gs[i];

          copyQuery2EvalInput();                                    //  Copy "queryGameStateBuffer" to Evaluation Module's "inputBuffer".

          b = isTerminal();                                         //  (Ask the Evaluation Module) Is the given game state terminal?
          if(b)                                                     //  - Terminal-state check.
            {
              node.value = node.color * evaluate();                 //  This imported function handles testing the AI's side.
              node.phase = _PHASE_FINISH_NODE;                      //  Mark for the finishing phase.
              saveNode(&node, gsIndex);                             //  Save the updated node.
              break;
            }

          b = isSideToMoveInCheck();                                //  (Ask the Evaluation Module) Is the side to move in the given game state in check?
          if(b)                                                     //  - In check?
            NN_SET_FLAG(&node, NN_FLAG_IN_CHECK);
          else
            NN_CLEAR_FLAG(&node, NN_FLAG_IN_CHECK);

          material = nonPawnMaterial();                             //  Are the conditions correct to try null-move pruning?
          if( !NN_HAS_FLAG(&node, NN_FLAG_NULL_TRIED) && !NN_HAS_FLAG(&node, NN_FLAG_IS_NULL_CHILD) &&
              !NN_HAS_FLAG(&node, NN_FLAG_AT_ROOT)    && !NN_HAS_FLAG(&node, NN_FLAG_IS_PV)         &&
              !NN_HAS_FLAG(&node, NN_FLAG_IN_CHECK)   && node.depth >= 3 && material > 3             )
            {
                                                                    //  - Null-move pruning.
              NN_SET_FLAG(&node, NN_FLAG_NULL_TRIED);               //  Indicate that we did try the null move for this node.
              NN_SET_FLAG(&node, NN_FLAG_NULL_IN_PROGRESS);         //  Indicate that this node's null-child is (WILL BE) on the stack.
              R = NULL_MOVE_BASE_REDUCTION;
              if(node.depth >= 6)
                R += NULL_MOVE_EXTRA_REDUCTION;
              newDepth = node.depth - 1 - R;
              if(newDepth < 0)
                newDepth = 0;

              makeNullMove();                                       //  (Ask the Evaluation Module) To make the null-move.
              copyEvalOutput2AuxBuffer(1);                          //  Copy ONE of (_GAMESTATE_BYTE_SIZE + _MOVE_BYTE_SIZE)
                                                                    //  from the Evaluation Module's output buffer
                                                                    //  to the Negamax Module's auxiliary buffer.
                                                                    //  Save the current length, before addition of child nodes.
              negamaxSearchBufferLength = restoreNegamaxSearchBufferLength();
              i = 0;
              for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)             //  Copy child gamestate byte-array to child node.
                child.gs[j] = auxiliaryBuffer[i++];
              child.parent = gsIndex;                               //  Set child's parent to the given index into "negamaxSearchBuffer".
              for(j = 0; j < _MOVE_BYTE_SIZE; j++)                  //  Copy the move the brought parent to child into the child node.
                child.parentMove[j] = auxiliaryBuffer[i++];
              child.bestMove[0] = _NONE;                            //  Set child's best-move to a blank move.
              child.bestMove[1] = _NONE;
              child.bestMove[2] = 0;

              child.childrenCtr = 0;                                //  Set child's children-counter to zero.
              child.depth = newDepth;                               //  Set child's depth to "newDepth".
              child.originalAlpha = -node.beta;                     //  Set child's alpha to negative parent's beta.
              child.alpha = -node.beta;
              child.beta = -node.alpha;                             //  Set child's beta to negative parent's alpha.
              child.color = -node.color;                            //  Set child's color to negative parent's color.
              child.zhash = 0L;
              child.hIndex = 0;
              child.phase = _PHASE_ENTER_NODE;                      //  Set child's phase.
              child.flags = 0;
              NN_SET_FLAG(&child, NN_FLAG_IS_NULL_CHILD);           //  Set the flag that says this is a null-move child.
              child.value = -std::numeric_limits<float>::infinity();

              node.phase = _PHASE_AFTER_CHILD;
              node.childrenCtr++;

              saveNode(&child, negamaxSearchBufferLength++);        //  Write serialized node to head of "negamaxSearchBuffer".
                                                                    //  Write number of NegamaxNodes in "negamaxSearchBuffer".
              saveNegamaxSearchBufferLength(negamaxSearchBufferLength);
              saveNode(&node, gsIndex);                             //  Save the updated node.
            }
          else                                                      //  Conditions do NOT allow null-move pruning.
            {
              node.phase = _PHASE_GEN_AND_ORDER;
              saveNode(&node, gsIndex);                             //  Save the updated node.
            }
          break;

        //////////////////////////////////////////////////////////////  Node has passed initial checks (transpo, terminal, leaf). Now generate moves/children.
        case _PHASE_GEN_AND_ORDER:
          expansion_step(gsIndex, &node);
          break;

        case _PHASE_NEXT_MOVE:                                      //  Node has already passed .
          break;

        //////////////////////////////////////////////////////////////  Node is somebody's child.
        case _PHASE_AFTER_CHILD:
          restoreNode(gsIndex, &child);                             //  Read bytes on the top of the stack into the struct named "child",
          restoreNode(child.parent, &node);                         //  and use "node" to refer to the stack-top's parent.
                                                                    //  This child is a null-move child.
          if(NN_HAS_FLAG(&node, NN_FLAG_NULL_IN_PROGRESS) && NN_HAS_FLAG(&child, NN_FLAG_IS_NULL_CHILD))
            {
              NN_CLEAR_FLAG(&node, NN_FLAG_NULL_IN_PROGRESS);       //  Un-set this flag: we are addressing the null-move child now.

              if(nullScore >= node.beta)                            //  Null-move fails high: prune this node.
                {                                                   //  We could store -child.value or node.beta; the parent only needs a value >= beta.
                  node.value = -child.value;
                  node.phase = _PHASE_FINISH_NODE;
                }
              else                                                  //  The null-move did NOT prune. Continue search as normal.
                node.phase = _PHASE_GEN_AND_ORDER;
              //  Save parent
            }
          else                                                      //  This "child" is a regular move we tried from "node".
            {
              if(-child.value >= node.beta)                         //  Beta cutoff?
                {                                                   //  Beta cutoff: this move is so good for us that the opponent will avoid this node.
                  node.value = -child.value;
                  node.phase = _PHASE_FINISH_NODE;
                  //  TODO:  killer / history updates go here once you have those tables:
                  //  TODO:  update_killers(parent, moveBytes);
                  //  TODO:  update_history(parent, moveBytes, -child.value);
                }
              if(-child.value > node.alpha)                         //  Improve alpha?
                {
                  node.alpha = -child.value;
                  node.value = -child.value;                        //  Best-so-far for this node.
                  // Track best move from this node
                  // (Assuming child->parentMove describes the move from parent->gs to child->gs)
                  //  TODO:  history update (no cutoff, but good move):
                  //  TODO:  update_history(parent, moveBytes, score);
                }

              // Decide what to do next:
              // - If there are more moves to try from this parent, go to NEXT_MOVE.
              // - If we've exhausted the move list, we're done with this node -> FINISH_NODE.

              // For that we probably need a per-parent move index & count; I'll assume:
              //   parent->childrenCtr = number of moves already searched
              //   parent->numMoves    = total moves generated for this node
              // You can adapt this to your actual fields.

              //parent->childrenCtr++;  // e.g., one more child processed

              //if (parent->childrenCtr >= parent->numMoves)
              //  {
              //     No more children to search -> this node is finished.
              //     parent->value already holds the best score found (or alpha).
              //     parent->phase = _PHASE_FINISH_NODE;
              //  }
              //else
              //  {
              //    Still have more moves to test from this parent
              //    parent->phase = _PHASE_NEXT_MOVE;
              //  }
              //
              //
            }

          break;

        case _PHASE_FINISH_NODE:                                    //  Node has a return-value ready for its parent.
          break;
      }

    return false;                                                   //  Indicate that search is ongoing.
  }

/* HEARTBEAT NEGAMAX
   Attempt to look up the given game state in the transposition table.
   This step will either find the transposition or it won't.
   If this step does find the given "node" in the transposition table, either:
     1.) it sets the return-value for the given "node" and set's "node"s phase so that Parent-Update follows, or
     2.) it adjusts "node"s bounds and sets its phase so that evaluation follows. */
void transpoCheck_step(unsigned int gsIndex, NegamaxNode* node)
  {
    TranspoRecord ttRecord;
    unsigned char gamestateByteArray[_GAMESTATE_BYTE_SIZE];         //  Store locally for comparison.
    bool foundTTLookup;
    unsigned int original_hIndex;                                   //  For the unlikely case in which we fill the table and search all the way around.
    unsigned int i;

    for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                       //  Copy unique byte-signature for the current game state to a local buffer.
      gamestateByteArray[i] = node->gs[i];

    node->originalAlpha = node->alpha;                              //  Save alpha as given.
    node->zhash = hash(gamestateByteArray);                         //  Zobrist-hash the game state byte array.
    node->hIndex = hashIndex(node->zhash);                          //  Index modulo size of transposition table.
    original_hIndex = node->hIndex;                                 //  Save the modulo-index to where this entry WANTS to go.

    //  LINEAR PROBING:
    //  We first want to see whether the given game state has already been searched.
    //  So, starting at "hIndex", probe extant TT records until the game-state byte-arrays match.
    //  If you encounter a free space, stop and give up: we have not found a pre-searched state.
    foundTTLookup = false;
    while(!foundTTLookup)
      {
        if(!fetchRecord(node->hIndex, &ttRecord))                   //  We've hit a blank: this state has not been found.
          break;
        else                                                        //  We've hit an occupied record--but is it the one we're looking for?
          {
            i = 0;
            while(i < _GAMESTATE_BYTE_SIZE && gamestateByteArray[i] == ttRecord.gs[i])
              i++;
            if(i == _GAMESTATE_BYTE_SIZE)
              foundTTLookup = true;
            else
              {
                if(node->hIndex == (_TRANSPO_TABLE_SIZE - 1))       //  Wrap around.
                  node->hIndex = 0;
                else
                  node->hIndex++;
                if(node->hIndex == original_hIndex)                 //  If we've come back around to where we started, then just quit: state is unfound.
                  break;
              }
          }
      }
                                                                    //  Attempt to look up the given game state (under Zobrist hash) in the transposition table.
    if(foundTTLookup)                                               //  Found this same game state.
      {
        ttRecord.age = (ttRecord.age > 1) ? ttRecord.age - 1 : 1;   //  This record was useful: decrease its age.
                                                                    //  Save this updated (rejuvenated) record back to the byte array.
        serializeTranspoRecord(&ttRecord, transpositionTableBuffer + 4 + node->hIndex * (8 + _TRANSPO_RECORD_BYTE_SIZE));

        if(ttRecord.depth >= node->depth)                           //  Record ratified from greater depth.
          {
            if(ttRecord.type == NODE_TYPE_PV)                       //  Exact.
              {
                node->value = ttRecord.score;                       //  This node's return value furnished by the transpo lookup.
                node->phase = _PHASE_FINISH_NODE;                   //  Set node's phase to Parent-Update.
                saveNode(node, gsIndex);                            //  Save the updated node.
                return;
              }
            else if(ttRecord.type == NODE_TYPE_CUT)                 //  Lower Bound.
              node->alpha = std::max(node->alpha, ttRecord.score);
            else if(ttRecord.type == NODE_TYPE_ALL)                 //  Upper Bound.
              node->beta = std::min(node->beta, ttRecord.score);

            if(node->alpha >= node->beta)
              {
                node->value = ttRecord.score;                       //  This node's return value furnished by the transpo lookup.
                node->phase = _PHASE_FINISH_NODE;                   //  Set node's phase to Parent-Update.
                saveNode(node, gsIndex);                            //  Save the updated node.
                return;
              }
          }
      }

    //node->phase = _PHASE_EVALUATION;                                //  Set node's phase to Evaluate.
    //saveNode(node, gsIndex);                                        //  Save the updated node.
    return;
  }

/* HEARTBEAT NEGAMAX
   If we are given a terminal game state or if depth has run down to zero (or -1 given quiescence search) then evaluate the state.
   This step tests the value of "node"s "depth".
   If the conditions are right to evaluate, then evaluate the state and save it to "node"s "returnValue" field.
   Sets "node"s phase to Parent-Update if the node was evaluated.
   Set's "node"s phase to Expansion if the node was not evaluated. */
void evaluation_step(unsigned int gsIndex, NegamaxNode* node)
  {
    bool quiet;                                                     //  Quiescence search.
    bool terminal;                                                  //  Leaf-node testing.
    unsigned int i;
                                                                    //  Copy "node"s "gs" to "queryGameStateBuffer"
    for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                       //  for isQuiet(), isTerminal(), and evaluate().
      queryGameStateBuffer[i] = node->gs[i];

    copyQuery2EvalInput();                                          //  Copy "queryGameStateBuffer" to Evaluation Module's "inputBuffer".

    quiet = isQuiet();                                              //  (Ask the Evaluation Module) Is the given game state quiet?
    terminal = isTerminal();                                        //  (Ask the Evaluation Module) Is the given game state terminal?

                                                                    //  Evaluate if: node->depth zero and state is quiet,
                                                                    //           or  node->depth is <= 0,
    if((node->depth == 0 && quiet) || node->depth < 0 || terminal)  //           or  this is a leaf node.
      {
        //  Evaluate... by WHOM? The linear model? A deep network? (Worry about this later.)
        node->value = node->color * evaluate();                     //  Evaluate this node.
        node->phase = _PHASE_PARENT_UPDATE;                         //  Set node's phase to Parent-Update and leave it on top of the stack.
        saveNode(node, gsIndex);                                    //  Save the updated node.
        incrementNodeCtr(1);                                        //  Increment node counter.
        return;
      }

    node->phase = _PHASE_EXPANSION;                                 //  Set node's phase so that it will be expanded on the next heartbeat.
    saveNode(node, gsIndex);                                        //  Save the updated node.
    return;
  }

/* HEARTBEAT NEGAMAX
   Expand the next ply.
   (Re)load the given "node"s game-state byte array into the Negamax Module's query buffer.
   Copy the query buffer contents to the Evaluation Module's input buffer.
   Call the Evaluation Module's "getSortedMoves" function: this creates a byte-array of concatenated child-states, moves, and rough scores.
   Perform additional scoring using the transposition table, killer moves, and history heuristic. Then sort ASCENDING, as suits the STACK MODEL.
   Convert the received data to nodes and append them to the Negamax Module's "negamaxSearchBuffer". */
void expansion_step(unsigned int gsIndex, NegamaxNode* node)
  {
    unsigned int len;                                               //  Number of moves available for the side to move in this state.
    unsigned int negamaxSearchBufferLength;
    //unsigned char gsBuffer[_GAMESTATE_BYTE_SIZE];
    //unsigned char moveBuffer[_MOVE_BYTE_SIZE];
    unsigned char buffer4[4];
    signed int si4;
    signed int scores[_MAX_MOVES];                                  //  SEE scores which we will augment using negamax knowledge.
    unsigned int i, j, auxCtr;
    NegamaxNode child;

    for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                       //  Copy "node"s "gs" to "queryGameStateBuffer" for getSortedMoves().
      queryGameStateBuffer[i] = node->gs[i];

    copyQuery2EvalInput();                                          //  Copy "queryGameStateBuffer" to Evaluation Module's "inputBuffer".

    len = getSortedMoves();                                         //  (Ask the Evaluation Module) Generate SEE-scored list of moves.
                                                                    //  (These are scored, quick-n-cheap, BUT NOT SORTED.)

    copyEvalOutput2AuxBuffer(len);                                  //  Copy from Evaluation Module's output buffer to "auxiliaryBuffer".
    negamaxSearchBufferLength = restoreNegamaxSearchBufferLength(); //  Save the current length, before addition of child nodes.

    //  [Transposition-Table move]
    //  Good captures, promotions, checks
    //  [Killer moves] (quiet moves that recently caused beta cutoffs at this depth)
    //  Other quiet moves, ordered by [history heuristic]
    //  Bad captures (SEE < 0)
/*
    When you sort:
    If given "reverse" is TRUE, then gamestate-move pairs are written in order of ASCENDING ADVANTAGE (worst first).
    If given "reverse" is FALSE, then gamestate-move pairs are written in order of DESCENDING ADVANTAGE (best first).
*/


    auxCtr = 0;                                                     //  Convert each run of (_GAMESTATE_BYTE_SIZE + _MOVE_BYTE_SIZE) bytes in "auxiliaryBuffer"
    for(i = 0; i < len; i++)                                        //  to a NegamaxNode and append them to "negamaxSearchBuffer".
      {
        for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)                   //  Copy child gamestate byte-array to child node.
          child.gs[j] = auxiliaryBuffer[auxCtr++];

        child.parent = gsIndex;                                     //  Set child's parent to the given index into "negamaxSearchBuffer".

        for(j = 0; j < _MOVE_BYTE_SIZE; j++)                        //  Copy the move the brought parent to child into the child node.
          child.parentMove[j] = auxiliaryBuffer[auxCtr++];

        for(j = 0; j < 4; j++)                                      //  Copy the 4 bytes of the move's SEE score from the global buffer.
          buffer4[i] = auxiliaryBuffer[auxCtr++];
        memcpy(&si4, buffer4, 4);                                   //  Force the 4-byte buffer into a signed int.
        scores[i] = si4;                                            //  Save the rough, SEE score to a local array.

        //  Is this state in the transpo table already? HUGE BUMP! +1'000'000
        //  Is this state killer move, slot 0? +80'000
        //  Is this state killer move, slot 1? +70'000
        //  Is this state in the history table for (side, from, to)? +(that score)




        child.bestMove[0] = _NONE;                                  //  Set child's best-move to a blank move.
        child.bestMove[1] = _NONE;
        child.bestMove[2] = 0;
        child.childrenCtr = 0;                                      //  Set child's children-counter to zero.
        child.depth = node->depth - 1;                              //  Set child's depth to parent's depth - 1.
        child.originalAlpha = -node->beta;                          //  Set child's alpha to negative parent's beta.
        child.alpha = -node->beta;
        child.beta = -node->alpha;                                  //  Set child's beta to negative parent's alpha.
        child.color = -node->color;                                 //  Set child's color to negative parent's color.
        child.zhash = 0L;                                           //
        child.hIndex = 0;                                           //
        child.phase = _PHASE_TRANSPO_CHECK;                         //  First thing to attempt: a transpo-table lookup.
        child.flags = 0;                                            //
        child.value = -std::numeric_limits<float>::infinity();      //

        node->childrenCtr++;                                        //  Increase the expanded node's number of children.

        saveNode(&child, negamaxSearchBufferLength++);              //  Write serialized node to head of "negamaxSearchBuffer".
        saveNegamaxSearchBufferLength(negamaxSearchBufferLength);   //  Write number of NegamaxNodes in "negamaxSearchBuffer".
      }

    //node->value = -std::numeric_limits<float>::infinity();          //  Initialize the given node's "value" to Negative Infinity.
    node->phase = _PHASE_PARENT_UPDATE;                             //  Set expanded node's phase to Parent-Update for when it again appears at
                                                                    //  the top of the stack.
    saveNode(node, gsIndex);                                        //  Save the newly expanded node.

    return;
  }

/* HEARTBEAT NEGAMAX
   Fourth step: update the parent node. */
void parentUpdate_step(unsigned int gsIndex, NegamaxNode* node)
  {
    NegamaxNode parent;
    TranspoRecord ttRecord;
    unsigned int size;                                              //  Current size of the transposition table.
    unsigned int len;
    unsigned char i;

    if(gsIndex != node->parent)                                     //  This is only the case for the root node.
      {
        restoreNode(node->parent, &parent);                         //  Recover the parent of the given "node".

        if(-node->value > parent.value)
          {
            parent.value = -node->value;                            //  Update parent's "value".
            for(i = 0; i < _MOVE_BYTE_SIZE; i++)                    //  Update best move found so far.
              parent.bestMove[i] = node->parentMove[i];
          }

        parent.alpha = std::max(parent.alpha, parent.value);        //  Update parent's "alpha".

        len = restoreNegamaxSearchBufferLength();                   //  Retreieve original stack length.

        if(parent.alpha >= parent.beta)                             //  Cut-off: no need to evaluate the remaining children.
          {                                                         //  Pop all of parent's children.
            len -= parent.childrenCtr;                              //  Decrease it by the number of *remaining* children of parent.
            saveNegamaxSearchBufferLength(len);                     //  Set the new, shortened length.
            parent.childrenCtr = 0;                                 //  Blank out parent's children-counter.
          }
        else                                                        //  No cut-off: continue evaluating the remaining children.
          {                                                         //  Pop only this present child.
            len--;
            saveNegamaxSearchBufferLength(len);                     //  Set the new, shortened length.
            parent.childrenCtr--;                                   //  Decrease the parent's child-counter.
          }

        if(parent.childrenCtr == 0)                                 //  However it happened, we are done with all this parent's children.
          {
            parent.phase = _PHASE_PARENT_UPDATE;                    //  Parent node has head from all the children it needs to hear from.
                                                                    //  It can now be used to update its own parent when it (re)appears
                                                                    //  at the top of the stack.
            //  Create a Transposition Table entry for the parent.

            if(parent.value <= parent.originalAlpha)                //  Save as UPPER BOUND (NODE_TYPE_ALL).
              {
                for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
                  ttRecord.gs[i] = parent.gs[i];
                for(i = 0; i < _MOVE_BYTE_SIZE; i++)
                  ttRecord.bestMove[i] = parent.bestMove[i];
                ttRecord.depth = (parent.depth > 0) ? (unsigned char)(parent.depth) : 0;
                ttRecord.score = parent.value;
                ttRecord.type  = NODE_TYPE_ALL;
                ttRecord.age   = 1;
              }
            else if(parent.value >= parent.beta)                    //  Save as LOWER BOUND (NODE_TYPE_CUT).
              {
                for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
                  ttRecord.gs[i] = parent.gs[i];
                for(i = 0; i < _MOVE_BYTE_SIZE; i++)
                  ttRecord.bestMove[i] = parent.bestMove[i];
                ttRecord.depth = (parent.depth > 0) ? (unsigned char)(parent.depth) : 0;
                ttRecord.score = parent.value;
                ttRecord.type  = NODE_TYPE_CUT;
                ttRecord.age   = 1;
              }
            else                                                    //  Save as EXACT (NODE_TYPE_PV).
              {
                for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
                  ttRecord.gs[i] = parent.gs[i];
                for(i = 0; i < _MOVE_BYTE_SIZE; i++)
                  ttRecord.bestMove[i] = parent.bestMove[i];
                ttRecord.depth = (parent.depth > 0) ? (unsigned char)(parent.depth) : 0;
                ttRecord.score = parent.value;
                ttRecord.type  = NODE_TYPE_PV;
                ttRecord.age   = 1;
              }

            size = tableSize();                                     //  First get the current length of the table.
            if(size < _TRANSPO_TABLE_SIZE)                          //  If there is room to add, then we are adding.
              setTableSize(size + 1);                               //  Set the new size of the table.
                                                                    //  Write a serialized version of the new entry directly to the global buffer.
            serializeTranspoRecord(&ttRecord, transpositionTableBuffer + 4 + parent.hIndex * (8 + _TRANSPO_RECORD_BYTE_SIZE));
          }

        saveNode(&parent, node->parent);                            //  Write updated parent to "negamaxSearchBuffer".
      }

    return;
  }

/**************************************************************************************************
 Search-buffer functions  */

/* Read the first 4 bytes from "negamaxSearchBuffer" and return the unsigned int they describe. */
unsigned int restoreNegamaxSearchBufferLength(void)
  {
    unsigned char buffer4[4];
    unsigned int ui4;
    unsigned char i;

    for(i = 0; i < 4; i++)                                          //  Copy the first 4 bytes from the global buffer.
      buffer4[i] = negamaxSearchBuffer[i];
    memcpy(&ui4, buffer4, 4);                                       //  Force the 4-byte buffer into an unsigned int.
    return ui4;                                                     //  Return the unsigned int.
  }

/* Write the given "len" to the first 4 bytes of "negamaxSearchBuffer". */
void saveNegamaxSearchBufferLength(unsigned int len)
  {
    unsigned char buffer4[4];
    unsigned char i;

    memcpy(buffer4, (unsigned char*)(&len), 4);                     //  Force unsigned int into 4-byte unsigned char buffer.
    for(i = 0; i < 4; i++)                                          //  Write 4-byte buffer to "negamaxSearchBuffer".
      negamaxSearchBuffer[i] = buffer4[i];

    return;
  }

/* In "negamaxSearchBuffer", at an offset of "index" * sizeof(NegamaxNode), read the bytes there into the given struct, "node". */
void restoreNode(unsigned int index, NegamaxNode* node)
  {
    unsigned char buffer[_NEGAMAX_NODE_BYTE_SIZE];
    unsigned char buffer4[4];
    unsigned char buffer8[8];
    unsigned int ui4;
    float f4;
    unsigned long long ull8;

    unsigned int i, j;

    for(i = 0; i < _NEGAMAX_NODE_BYTE_SIZE; i++)                    //  Copy a slice of "negamaxSearchBuffer" to the local "buffer".
      buffer[i] = negamaxSearchBuffer[index * _NEGAMAX_NODE_BYTE_SIZE + 4 + i];

    i = 0;                                                          //  Reset "i" to iterate through local buffer.
    for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)                       //  Recover node's gamestate byte array from local buffer.
      node->gs[j] = buffer[i++];

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&ui4, buffer4, 4);                                       //  Force the 4-byte buffer into an unsigned int.
    node->parent = ui4;                                             //  Recover node's parent index from local buffer.

    for(j = 0; j < _MOVE_BYTE_SIZE; j++)                            //  Recover node's parent-move from local buffer.
      node->parentMove[j] = buffer[i++];

    for(j = 0; j < _MOVE_BYTE_SIZE; j++)                            //  Recover node's best-move from local buffer.
      node->bestMove[j] = buffer[i++];

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&ui4, buffer4, 4);                                       //  Force the 4-byte buffer into an unsigned int.
    node->childrenCtr = ui4;                                        //  Recover the node's children-counter from the local buffer.

    node->depth = (signed char)buffer[i++];                         //  Recover the node's depth from the local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    node->originalAlpha = f4;                                       //  Recover node's originalAlpha from local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    node->alpha = f4;                                               //  Recover node's alpha from local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    node->beta = f4;                                                //  Recover node's beta from local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    node->color = f4;                                               //  Recover node's color from local buffer.

    for(j = 0; j < 8; j++)                                          //  Copy 8 bytes from the local buffer.
      buffer8[j] = buffer[i++];
    memcpy(&ull8, buffer8, 8);                                      //  Force the 8-byte buffer into an unsigned long long.
    node->zhash = ull8;                                             //  Recover node's hIndex from local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&ui4, buffer4, 4);                                       //  Force the 4-byte buffer into an unsigned int.
    node->hIndex = ui4;                                             //  Recover node's hIndex from local buffer.

    node->phase = buffer[i++];                                      //  Recover node's phase from local buffer.
    node->flags = buffer[i++];                                      //  Recover node's flags from local buffer.

    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the local buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    node->value = f4;                                               //  Recover node's value from local buffer.

    return;
  }

/* Write bytes for the given struct "node" to "negamaxSearchBuffer" at an offset of "index" * sizeof(NegamaxNode). */
void saveNode(NegamaxNode* node, unsigned int index)
  {
    unsigned char buffer[_NEGAMAX_NODE_BYTE_SIZE];
    unsigned char buffer4[4];
    unsigned char buffer8[8];
    unsigned int ui4;
    float f4;
    unsigned long long ull8;

    unsigned int i, j;

    i = 0;
    for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)                       //  Copy node's gamestate byte array to buffer.
      buffer[i++] = node->gs[j];

    ui4 = node->parent;                                             //  Copy node's parent index to buffer.
    memcpy(buffer4, (unsigned char*)(&ui4), 4);                     //  Force unsigned int into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    for(j = 0; j < _MOVE_BYTE_SIZE; j++)                            //  Copy node's parent-move to buffer.
      buffer[i++] = node->parentMove[j];

    for(j = 0; j < _MOVE_BYTE_SIZE; j++)                            //  Copy node's best-move to buffer.
      buffer[i++] = node->bestMove[j];

    ui4 = node->childrenCtr;                                        //  Copy the node's children-counter to the buffer.
    memcpy(buffer4, (unsigned char*)(&ui4), 4);                     //  Force unsigned int into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    buffer[i++] = (unsigned char)node->depth;                       //  Copy the node's depth to the buffer.

    f4 = node->originalAlpha;                                       //  Copy the node's originalAlpha to the buffer.
    memcpy(buffer4, (unsigned char*)(&f4), 4);                      //  Force float into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    f4 = node->alpha;                                               //  Copy the node's alpha to the buffer.
    memcpy(buffer4, (unsigned char*)(&f4), 4);                      //  Force float into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    f4 = node->beta;                                                //  Copy the node's beta to the buffer.
    memcpy(buffer4, (unsigned char*)(&f4), 4);                      //  Force float into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    f4 = node->color;                                               //  Copy the node's color to the buffer.
    memcpy(buffer4, (unsigned char*)(&f4), 4);                      //  Force float into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    ull8 = node->zhash;                                             //  Copy the node's zhash to the buffer.
    memcpy(buffer8, (unsigned char*)(&ull8), 8);                    //  Force unsigned long long into 8-byte unsigned char buffer.
    for(j = 0; j < 8; j++)                                          //  Append 8-byte buffer to buffer.
      buffer[i++] = buffer8[j];

    ui4 = node->hIndex;                                             //  Copy the node's hIndex to the buffer.
    memcpy(buffer4, (unsigned char*)(&ui4), 4);                     //  Force unsigned int into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    buffer[i++] = node->phase;                                      //  Copy the node's phase to the buffer.
    buffer[i++] = node->flags;                                      //  Copy the node's flags to the buffer.

    f4 = node->value;                                               //  Copy the node's value to the buffer.
    memcpy(buffer4, (unsigned char*)(&f4), 4);                      //  Force float into 4-byte unsigned char buffer.
    for(j = 0; j < 4; j++)                                          //  Append 4-byte buffer to buffer.
      buffer[i++] = buffer4[j];

    for(i = 0; i < _NEGAMAX_NODE_BYTE_SIZE; i++)                    //  Now write "buffer" to an offset location in "negamaxSearchBuffer".
      negamaxSearchBuffer[index * _NEGAMAX_NODE_BYTE_SIZE + 4 + i] = buffer[i];

    return;
  }

/**************************************************************************************************
 Zobrist hashing  */

/* Hash the given byte array "hashInputBuffer". */
unsigned long long hash(unsigned char* hashInputBuffer)
  {
    unsigned long long h = 0L;
    unsigned int index;
    unsigned int i;
    unsigned char j, k, l;
    unsigned char mask;
    unsigned char wKingPos = 0, bKingPos = 0;
    unsigned char buffer8[8];                                       //  Byte array to hold byte array version of unsigned long long.
    unsigned long long ull8;                                        //  The unsigned long long we will actually use to hash.

    wKingPos |= (hashInputBuffer[8] & 128);                         //  Recover white king's position.
    wKingPos |= (hashInputBuffer[9] & 128) >> 1;
    wKingPos |= (hashInputBuffer[10] & 128) >> 2;
    wKingPos |= (hashInputBuffer[11] & 128) >> 3;
    wKingPos |= (hashInputBuffer[12] & 128) >> 4;
    wKingPos |= (hashInputBuffer[13] & 128) >> 5;
    wKingPos |= (hashInputBuffer[14] & 128) >> 6;
    wKingPos |= (hashInputBuffer[15] & 128) >> 7;

    bKingPos |= (hashInputBuffer[8] & 1) << 7;                      //  Recover black king's position.
    bKingPos |= (hashInputBuffer[9] & 1) << 6;
    bKingPos |= (hashInputBuffer[10] & 1) << 5;
    bKingPos |= (hashInputBuffer[11] & 1) << 4;
    bKingPos |= (hashInputBuffer[12] & 1) << 3;
    bKingPos |= (hashInputBuffer[13] & 1) << 2;
    bKingPos |= (hashInputBuffer[14] & 1) << 1;
    bKingPos |= (hashInputBuffer[15] & 1);

    if((hashInputBuffer[0] & 128) == 128)                           //  Hash side to move.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[W_TO_MOVE * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }

    if((hashInputBuffer[1] & 128) == 128)                           //  Hash white's castling data.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[W_KINGSIDE_CASTLE * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    if((hashInputBuffer[2] & 128) == 128)
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[W_QUEENSIDE_CASTLE * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    if((hashInputBuffer[3] & 128) == 128)
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[W_CASTLED * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }

    if((hashInputBuffer[4] & 128) == 128)                           //  Hash black's castling data.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[B_KINGSIDE_CASTLE * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    if((hashInputBuffer[5] & 128) == 128)
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[B_QUEENSIDE_CASTLE * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    if((hashInputBuffer[6] & 128) == 128)
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[B_CASTLED * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }

    if((hashInputBuffer[0] & 1) == 1)                               //  Hash whether a pawn's doulbe move previously occurred in column A.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_A * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[1] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column B.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_B * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[2] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column C.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_C * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[3] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column D.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_D * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[4] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column E.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_E * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[5] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column F.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_F * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[6] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column G.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_G * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }
    else if((hashInputBuffer[7] & 1) == 1)                          //  Hash whether a pawn's doulbe move previously occurred in column H.
      {
        for(l = 0; l < 8; l++)                                      //  Copy 8 bytes from the serial buffer.
          buffer8[l] = zobristHashBuffer[PREV_DOUBLE_COL_H * 8 + l];
        memcpy(&ull8, buffer8, 8);                                  //  Force the 8-byte buffer into an unsigned long long.
        h ^= ull8;
      }

    index = 0;                                                      //  Reset to head of byte array.

    i = WP_A2;                                                      //  Ascend from rows 2 to 7, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash white pawns.
      {
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = BP_A2;                                                      //  Ascend from rows 2 to 7, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash black pawns.
      {
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = WN_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash white knights.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = BN_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash black knights.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = WB_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash white bishops.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = BB_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash black bishops.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = WR_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash white rooks.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = BR_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash black rooks.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = WQ_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash white queen(s).
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

    i = BQ_A1;                                                      //  Ascend from rows 1 to 8, over columns A to H.
    for(j = 0; j < 8; j++)                                          //  Hash black queen(s).
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((hashInputBuffer[index] & mask) == mask)
              {
                for(l = 0; l < 8; l++)                              //  Copy 8 bytes from the serial buffer.
                  buffer8[l] = zobristHashBuffer[i * 8 + l];
                memcpy(&ull8, buffer8, 8);                          //  Force the 8-byte buffer into an unsigned long long.
                h ^= ull8;
              }
            mask = mask >> 1;
            i++;
          }
        index++;
      }

                                                                    //  Hash white king position.
    for(l = 0; l < 8; l++)                                          //  Copy 8 bytes from the serial buffer.
      buffer8[l] = zobristHashBuffer[(WK_A1 + wKingPos) * 8 + l];
    memcpy(&ull8, buffer8, 8);                                      //  Force the 8-byte buffer into an unsigned long long.
    h ^= ull8;
                                                                    //  Hash black king position.
    for(l = 0; l < 8; l++)                                          //  Copy 8 bytes from the serial buffer.
      buffer8[l] = zobristHashBuffer[(BK_A1 + bKingPos) * 8 + l];
    memcpy(&ull8, buffer8, 8);                                      //  Force the 8-byte buffer into an unsigned long long.
    h ^= ull8;

    return h;
  }

