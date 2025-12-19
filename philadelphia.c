/*

sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c emcc -Os -s STANDALONE_WASM -s EXPORTED_FUNCTIONS="['_getInputGameStateBuffer','_getInputMoveBuffer','_getOutputGameStateBuffer','_getOutputMovesBuffer','_isQuiet','_isTerminal','_isSideToMoveInCheck','_nonPawnMaterial','_makeMove','_makeNullMove','_evaluate','_getMoves']" -Wl,--no-entry "philadelphia.c" -o "eval.wasm"

*/

#include "gamestate.h"
#include "philadelphia.h"

#define SEE_SCORE_PAWN              100                             /* Static Exchange Evaluation, rough pawn score. */
#define SEE_SCORE_KNIGHT            300                             /* Static Exchange Evaluation, rough knight score. */
#define SEE_SCORE_BISHOP            325                             /* Static Exchange Evaluation, rough bishop score. */
#define SEE_SCORE_ROOK              500                             /* Static Exchange Evaluation, rough rook score. */
#define SEE_SCORE_QUEEN             900                             /* Static Exchange Evaluation, rough queen score. */
#define SEE_SCORE_KING            10000                             /* Static Exchange Evaluation, rough king score. */
#define MOVE_SORTING_PROMO_BONUS    800                             /* Static Exchange Evaluation, rough promotion bonus. */
#define MOVE_SORTING_CHECK_BONUS     50                             /* Static Exchange Evaluation, rough putting-opponent-in-check bonus. */

/**************************************************************************************************
 Typedefs  */


/**************************************************************************************************
 Prototypes  */

unsigned char* getInputGameStateBuffer(void);
unsigned char* getInputMoveBuffer(void);
unsigned char* getOutputGameStateBuffer(void);
unsigned char* getOutputMovesBuffer(void);

void serializeGameStateToBuffer(GameState*, unsigned char*);
void serializeMoveToBuffer(Move*, unsigned char*);
void deserializeGameState(GameState*);
void deserializeMove(Move*);

bool isQuiet(void);
bool isTerminal(void);
bool isSideToMoveInCheck(void);
unsigned char nonPawnMaterial(void);
void makeMove(void);
void makeNullMove(void);
float evaluate(bool);
unsigned int getMoves(void);
//void quicksort(bool, float*, Move*, unsigned int, unsigned int);
//unsigned int partition(bool, float*, Move*, unsigned int, unsigned int);

/**************************************************************************************************
 Globals  */

unsigned char inputGameStateBuffer[_GAMESTATE_BYTE_SIZE];           //  Global array containing the serialized INPUT game state.

unsigned char inputMoveBuffer[_MOVE_BYTE_SIZE];                     //  Global array containing the serialized INPUT move.

unsigned char outputGameStateBuffer[_GAMESTATE_BYTE_SIZE];          //  Global array containing the serialized OUTPUT game state.

                                                                    //  Global array containing an integer (4 bytes) followed by that number of moves.
                                                                    //  Each move is represented as a byte sub-array encoding:
                                                                    //    _MOVE_BYTE_SIZE  :  (from, to, promo),
                                                                    //    4                :  bytes for signed integer, which is rough score.
unsigned char outputMovesBuffer[4 + _MAX_MOVES * (_MOVE_BYTE_SIZE + 4)];

/**************************************************************************************************
 Functions  */

/* Expose the global array declared here to JavaScript.  */
unsigned char* getInputGameStateBuffer(void)
  {
    return &inputGameStateBuffer[0];
  }

/* Expose the global array declared here to JavaScript.  */
unsigned char* getInputMoveBuffer(void)
  {
    return &inputMoveBuffer[0];
  }

/* Expose the global array declared here to JavaScript.  */
unsigned char* getOutputGameStateBuffer(void)
  {
    return &outputGameStateBuffer[0];
  }

/* Expose the global array declared here to JavaScript.  */
unsigned char* getOutputMovesBuffer(void)
  {
    return &outputMovesBuffer[0];
  }

/* Write the given game state to the given buffer. */
void serializeGameStateToBuffer(GameState* gs, unsigned char* buffer)
  {
    unsigned char i = 0, j, k;
    unsigned char ch, mask;
    unsigned char wKingPos, bKingPos;

    wKingPos = 0;
    while(wKingPos < _NONE && gs->board[wKingPos] != 'K')           //  Locate white king.
      wKingPos++;

    bKingPos = 0;
    while(bKingPos < _NONE && gs->board[bKingPos] != 'k')           //  Locate black king.
      bKingPos++;

    //////////////////////////////////////////////////////////////////  Encode white pawns, castling data, and the previous pawn double-move indicator.
    ch = 0;
    if(gs->whiteToMove)                                             //  Set bit: white to move.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(j * 8, gs) && isPawn(j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column A.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 128) >> 7;                  //  Set bit: previous pawn double-move occurred on column A.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->whiteKingsidePrivilege)                                  //  Set bit: white's kingside castling privilege.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(1 + j * 8, gs) && isPawn(1 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column B.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 64) >> 6;                   //  Set bit: previous pawn double-move occurred on column B.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->whiteQueensidePrivilege)                                 //  Set bit: white's queenside castling privilege.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(2 + j * 8, gs) && isPawn(2 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column C.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 32) >> 5;                   //  Set bit: previous pawn double-move occurred on column C.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->whiteCastled)                                            //  Set bit: white has castled.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(3 + j * 8, gs) && isPawn(3 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column D.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 16) >> 4;                   //  Set bit: previous pawn double-move occurred on column D.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->blackKingsidePrivilege)                                  //  Set bit: black's kingside castling privilege.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(4 + j * 8, gs) && isPawn(4 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column E.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 8) >> 3;                    //  Set bit: previous pawn double-move occurred on column E.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->blackQueensidePrivilege)                                 //  Set bit: black's queenside castling privilege.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(5 + j * 8, gs) && isPawn(5 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column F.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 4) >> 2;                    //  Set bit: previous pawn double-move occurred on column F.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    if(gs->blackCastled)                                            //  Set bit: black has castled.
      ch |= 128;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(6 + j * 8, gs) && isPawn(6 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column G.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 2) >> 1;                    //  Set bit: previous pawn double-move occurred on column G.
    buffer[i++] = ch;                                               //  Write byte.

    ch = 0;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(7 + j * 8, gs) && isPawn(7 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column H.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 1);                         //  Set bit: previous pawn double-move occurred on column H.
    buffer[i++] = ch;                                               //  Write byte.

    //////////////////////////////////////////////////////////////////  Encode black pawns, white king's location, and black king's location.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if(isBlack(j + k * 8, gs) && isPawn(j + k * 8, gs))
              ch |= mask;                                           //  Set bit: black pawn on k-th row of j-th column.
            mask = mask >> 1;
          }
        mask = 128 >> j;
        if((wKingPos & mask) == mask)                               //  Set bit in unsigned char: white king's location.
          ch |= 128;
        if((bKingPos & mask) == mask)                               //  Set bit in unsigned char: black king's location.
          ch |= 1;
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode white knights.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isWhite(j + k * 8, gs) && isKnight(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode black knights.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isBlack(j + k * 8, gs) && isKnight(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode white bishops.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isWhite(j + k * 8, gs) && isBishop(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode black bishops.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isBlack(j + k * 8, gs) && isBishop(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode white rooks.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isWhite(j + k * 8, gs) && isRook(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode black rooks.
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isBlack(j + k * 8, gs) && isRook(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode white queen(s).
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isWhite(j + k * 8, gs) && isQueen(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode black queen(s).
    for(j = 0; j < 8; j++)
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if(isBlack(j + k * 8, gs) && isQueen(j + k * 8, gs))
              ch |= mask;
            mask = mask >> 1;
          }
        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode move counter.
    buffer[i++] = gs->moveCtr;

    return;
  }

/* Write the given move to the given buffer. */
void serializeMoveToBuffer(Move* move, unsigned char* buffer)
  {
    unsigned char i = 0;

    buffer[i++] = move->from;
    buffer[i++] = move->to;
    buffer[i++] = move->promo;

    return;
  }

/* Recover a GameState from the unsigned-char buffer "inputGameStateBuffer". */
void deserializeGameState(GameState* gs)
  {
    unsigned char i, j, k;
    unsigned char ch, mask;
    unsigned char wKingPos = 0, bKingPos = 0;

    for(i = 0; i < _NONE; i++)                                      //  Fill-in/blank-out.
      gs->board[i] = 'e';
    gs->previousDoublePawnMove = 0;

    gs->whiteToMove = ((inputGameStateBuffer[0] & 128) == 128);     //  Recover side to move.
                                                                    //  Recover white's castling data.
    gs->whiteKingsidePrivilege = ((inputGameStateBuffer[1] & 128) == 128);
    gs->whiteQueensidePrivilege = ((inputGameStateBuffer[2] & 128) == 128);
    gs->whiteCastled = ((inputGameStateBuffer[3] & 128) == 128);
                                                                    //  Recover black's castling data.
    gs->blackKingsidePrivilege = ((inputGameStateBuffer[4] & 128) == 128);
    gs->blackQueensidePrivilege = ((inputGameStateBuffer[5] & 128) == 128);
    gs->blackCastled = ((inputGameStateBuffer[6] & 128) == 128);
                                                                    //  Recover en-passant data.
    gs->previousDoublePawnMove |= (inputGameStateBuffer[0] & 1) << 7;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[1] & 1) << 6;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[2] & 1) << 5;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[3] & 1) << 4;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[4] & 1) << 3;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[5] & 1) << 2;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[6] & 1) << 1;
    gs->previousDoublePawnMove |= (inputGameStateBuffer[7] & 1);

    wKingPos |= (inputGameStateBuffer[8] & 128);                    //  Recover white king's position.
    wKingPos |= (inputGameStateBuffer[9] & 128) >> 1;
    wKingPos |= (inputGameStateBuffer[10] & 128) >> 2;
    wKingPos |= (inputGameStateBuffer[11] & 128) >> 3;
    wKingPos |= (inputGameStateBuffer[12] & 128) >> 4;
    wKingPos |= (inputGameStateBuffer[13] & 128) >> 5;
    wKingPos |= (inputGameStateBuffer[14] & 128) >> 6;
    wKingPos |= (inputGameStateBuffer[15] & 128) >> 7;

    bKingPos |= (inputGameStateBuffer[8] & 1) << 7;                 //  Recover black king's position.
    bKingPos |= (inputGameStateBuffer[9] & 1) << 6;
    bKingPos |= (inputGameStateBuffer[10] & 1) << 5;
    bKingPos |= (inputGameStateBuffer[11] & 1) << 4;
    bKingPos |= (inputGameStateBuffer[12] & 1) << 3;
    bKingPos |= (inputGameStateBuffer[13] & 1) << 2;
    bKingPos |= (inputGameStateBuffer[14] & 1) << 1;
    bKingPos |= (inputGameStateBuffer[15] & 1);

    i = 0;                                                          //  Reset to head of byte array.

    for(j = 0; j < 8; j++)                                          //  Recover white pawns.
      {
        ch = 0;
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'P';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black pawns.
      {
        ch = 0;
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'p';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white knights.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'N';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black knights.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'n';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white bishops.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'B';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black bishops.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'b';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white rooks.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'R';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black rooks.
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'r';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white queen(s).
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'Q';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black queen(s).
      {
        ch = 0;
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((inputGameStateBuffer[i] & mask) == mask)
              gs->board[j + k * 8] = 'q';
            mask = mask >> 1;
          }
        i++;
      }

    gs->board[ wKingPos ] = 'K';                                    //  Restore white king.
    gs->board[ bKingPos ] = 'k';                                    //  Restore black king.

    gs->moveCtr = inputGameStateBuffer[i++];                        //  Recover move counter.

    return;
  }

/* Recover a Move from the unsigned-char buffer "inputMoveBuffer". */
void deserializeMove(Move* move)
  {
    move->from = inputMoveBuffer[0];
    move->to = inputMoveBuffer[1];
    move->promo = inputMoveBuffer[2];
    return;
  }

/* Answer the Negamax Module's query, "Is the GameState in the query buffer quiet?" */
bool isQuiet(void)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return quiet(&gs);
  }

/* Answer the Negamax Module's query, "Is the GameState in the query buffer terminal?" */
bool isTerminal(void)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return terminal(&gs);
  }

/* Answer the Negamax Module's query, "Is the side to move in the GameState in the query buffer in check?" */
bool isSideToMoveInCheck(void)
  {
    GameState gs;
    unsigned char i;

    deserialize(&gs);                                               //  Recover GameState from buffer.

    if(gs.whiteToMove)                                              //  White is to move; is white in check?
      {
        i = 0;
        while(i < _NONE && gs.board[i] != 'K')
          i++;
        return inCheckBy(i, 'b', &gs);
      }
                                                                    //  Black is to move; is black in check?
    i = 0;
    while(i < _NONE && gs.board[i] != 'k')
      i++;
    return inCheckBy(i, 'w', &gs);
  }

/* Answer the Negamax Module's query, "How much non-pawn material does the side to move have in the GameState in the query buffer?" */
unsigned char nonPawnMaterial(void)
  {
    GameState gs;
    unsigned char knights = 0;
    unsigned char bishops = 0;
    unsigned char rooks = 0;
    unsigned char queens = 0;
    unsigned char i;

    deserialize(&gs);                                               //  Recover GameState from buffer.

    if(gs.whiteToMove)
      {
        for(i = 0; i < _NONE; i++)
          {
            if(isWhite(i, &gs))
              {
                if(isKnight(i, &gs))
                  knights++;
                else if(isBishop(i, &gs))
                  bishops++;
                else if(isRook(i, &gs))
                  rooks++;
                else if(isQueen(i, &gs))
                  queens++;
              }
          }
      }
    else
      {
        for(i = 0; i < _NONE; i++)
          {
            if(isBlack(i, &gs))
              {
                if(isKnight(i, &gs))
                  knights++;
                else if(isBishop(i, &gs))
                  bishops++;
                else if(isRook(i, &gs))
                  rooks++;
                else if(isQueen(i, &gs))
                  queens++;
              }
          }
      }

    return 3 * (knights + bishops) + 5 * rooks + 9 * queens;
  }

/* Answer the Negamax Module's query, "What GameState results from making the move in the input-move buffer in the game state in the input-gamestate buffer?"
   Writes to "outputGameStateBuffer". */
void makeMove(void)
  {
    GameState gs;
    Move move;

    deserializeGameState(&gs);                                      //  Recover GameState from buffer.
    deserializeMove(&move);                                         //  Recover Move from buffer.

    makeMove(&move, &gs);                                           //  Make the move.

    serializeGameStateToBuffer(&gs, outputGameStateBuffer);         //  Write updated GameState to output-gamestate buffer.

    return;
  }

/* For use by null-move pruning in tree-search.
   Answer the Negamax Module's query, "What GameState results from a null-move in the game state in the input-gamestate buffer?"
   Writes to "outputGameStateBuffer". */
void makeNullMove(void)
  {
    GameState gs;

    deserializeGameState(&gs);                                      //  Recover GameState from buffer.

    makeNullMove(&gs);                                              //  Make the move.

    serializeGameStateToBuffer(&gs, outputGameStateBuffer);         //  Write updated GameState to output-gamestate buffer.

    return;
  }

/* Answer the Negamax Module's query, "What is the evaluation of the GameState in the input-gamestate buffer?" */
float evaluate(bool evaluateForWhite)
  {
    GameState gs;
    deserializeGameState(&gs);                                      //  Recover GameState from buffer.
    return score(&gs, evaluateForWhite);
  }

/* Answer the Negamax Module's query, "What are all the moves that can be made from the GameState in the input-gamestate buffer?"
   Writes to "outputMovesBuffer":
     [4-byte unsigned integer], [_MOVE_BYTE_SIZE bytes of move, 4 bytes of a signed int],
                                [_MOVE_BYTE_SIZE bytes of move, 4 bytes of a signed int],
                                                          . . .
                                [_MOVE_BYTE_SIZE bytes of move, 4 bytes of a signed int] */
unsigned int getMoves()
  {
    GameState gs, child;
    Move moves[_MAX_MOVES];
    unsigned int movesLen = 0;
    signed int score, scores[_MAX_MOVES];                           //  Use fast, cheap heuristics like SEE.

    unsigned char buffer4[4];                                       //  Byte array to hold leading int = number of moves in output buffer.
    unsigned int i, j, k;
    unsigned char whiteKingIndex, blackKingIndex;

    deserializeGameState(&gs);                                      //  Recover GameState from buffer.
    movesLen = getMoves(&gs, moves);                                //  Get moves.

    for(i = 0; i < movesLen; i++)                                   //  Compute a fast-n-cheap score to help the Negamax Module sort its nodes.
      {
        scores[i] = 0;                                              //  Initialize every move to zero.

        if(isCapture(moves + i, &gs))                               //  Move is a capture.
          scores[i] += SEE(moves + i, &gs);                         //  Static Exchange Evaluation can reveal good, equal, or bad captures.
        if((moves + i)->promo != _NO_PROMO)                         //  Move is a promotion.
          scores[i] += MOVE_SORTING_PROMO_BONUS;

        copyGameState(&gs, &child);                                 //  Clone the source state.
        makeMove(moves + i, &child);                                //  Apply the candidate move.
        whiteKingIndex = 0;                                         //  Locate the white king.
        while(whiteKingIndex < _NONE && child.board[whiteKingIndex] != 'K')
          whiteKingIndex++;
        blackKingIndex = 0;                                         //  Locate the black king.
        while(blackKingIndex < _NONE && child.board[blackKingIndex] != 'k')
          blackKingIndex++;
                                                                    //  Move puts opponent in check?
        if( (!child.whiteToMove && inCheckBy(blackKingIndex, 'w', &child)) || (child.whiteToMove && inCheckBy(whiteKingIndex, 'b', &child)) )
          scores[i] += MOVE_SORTING_CHECK_BONUS;
      }

    //quicksort(!reverse, scores, moves, 0, movesLen - 1);            //  Sort according to (shallow) evaluation.

    i = 0;                                                          //  Point to head of output buffer.
    memcpy(buffer4, (unsigned char*)(&movesLen), 4);                //  Force the unsigned integer into a 4-byte temp buffer.
    for(j = 0; j < 4; j++)                                          //  Copy bytes to serial buffer.
      outputMovesBuffer[i++] = buffer4[j];

    for(j = 0; j < movesLen; j++)                                   //  Write moves as bytes to output buffer, following uint total number of moves.
      {
        outputMovesBuffer[i++] = moves[j].from;                     //  Copy move to global byte array.
        outputMovesBuffer[i++] = moves[j].to;
        outputMovesBuffer[i++] = moves[j].promo;

        score = scores[j];
        memcpy(buffer4, (unsigned char*)(&score), 4);               //  Force the SIGNED integer into a 4-byte temp buffer.
        for(k = 0; k < 4; k++)                                      //  Copy local SIGNED score to global output byte array.
          outputMovesBuffer[i++] = buffer4[k];
      }

    return movesLen;
  }

/* Static Exchange Evaluation */
signed int SEE(Move* move, GameState* src)
  {
    GameState gs;
    signed int gains[_NONE];
    unsigned char gainsLen = 0;
    unsigned char target;
    signed int capturedPieceVal;
    char capturingPiece;
    char team;
    Move buffer[_NONE];
    Move chosenMove;
    unsigned char len, i;
    signed int leastVal, val, victimVal, j;
    signed int stopHere, continueExchange;

    copyGameState(src, &gs);
    target = move->to;                                              //  Identify the target.

    capturedPieceVal = SEE_lookup(gs.board[move->to]);              //  Look up value of the captured piece.
    capturingPiece = gs.board[move->from];
    gains[0] = capturedPieceVal;
    makeMove(move, &gs);                                            //  Apply the move to be evaluated.
    team = gs.whiteToMove ? 'w' : 'b';

    while(true)                                                     //  Follow capture chain to the end.
      {
        len = attackersOfSquare(target, team, &gs, buffer);
        if(len == 0)                                                //  No further captures.
          break;

        leastVal = INT_MAX;                                         //  Find the least valuable captor.
        for(i = 0; i < len; i++)
          {
            val = SEE_lookup(gs.board[ buffer[i].from ]);
            if(val < leastVal)
              {
                chosenMove.from = buffer[i].from;
                chosenMove.to = buffer[i].to;
                chosenMove.promo = _NO_PROMO;
                leastVal = val;
              }
          }
        victimVal = SEE_lookup(capturingPiece);                     //  Look up value of the victim.
        gainsLen++;
        gains[gainsLen] = victimVal - gains[gainsLen - 1];

        capturingPiece = gs.board[chosenMove.from];                 //  Identify the capturing piece.
        makeMove(&chosenMove, &gs);                                 //  Make the capture.
        team = gs.whiteToMove ? 'w' : 'b';
      }

    if(gainsLen > 0)
      {
        for(j = gainsLen - 1; j >= 0; j--)
          {
            stopHere = -gains[j];                                   //  It is in side-to-move's interest to stop here.
            continueExchange = gains[j + 1];                        //  It is in side-to-move's interest to continue exchanging.
            gains[j] = (stopHere > continueExchange) ? stopHere : continueExchange;
          }
      }

    return gains[0];
  }

/* Look up a rough material score for each piece. */
signed int SEE_lookup(char piece)
  {
    if(piece == 'P' || piece == 'p')
      return SEE_SCORE_PAWN;
    if(piece == 'N' || piece == 'n')
      return SEE_SCORE_KNIGHT;
    if(piece == 'B' || piece == 'b')
      return SEE_SCORE_BISHOP;
    if(piece == 'R' || piece == 'r')
      return SEE_SCORE_ROOK;
    if(piece == 'Q' || piece == 'q')
      return SEE_SCORE_QUEEN;
    if(piece == 'K' || piece == 'k')
      return SEE_SCORE_KING;
    return 0;
  }
/*
void quicksort(bool desc, float* scores, Move* moves, unsigned int lo, unsigned int hi)
  {
    unsigned int p;

    if(lo < hi)
      {
        p = partition(desc, scores, moves, lo, hi);

        if(p > 0)                                                   //  PREVENT ROLL-OVER TO 0xFFFF.
          quicksort(desc, scores, moves, lo, p - 1);                //  Left side: start quicksort.
        if(p < 65535)                                               //  PREVENT ROLL-OVER TO 0x0000.
          quicksort(desc, scores, moves, p + 1, hi);                //  Right side: start quicksort.
      }

    return;
  }

unsigned int partition(bool desc, float* scores, Move* moves, unsigned int lo, unsigned int hi)
  {
    float pivot = scores[hi];
    unsigned int i = lo;
    unsigned int j;
    float tmpFloat;
    Move tmpMove;
    bool trigger;

    for(j = lo; j < hi; j++)
      {
        if(desc)
          trigger = (scores[j] > pivot);                            //  SORT DESCENDING.
        else
          trigger = (scores[j] < pivot);                            //  SORT ASCENDING.

        if(trigger)
          {
            tmpFloat  = scores[i];                                  //  Swap scores[i] with scores[j].
            scores[i] = scores[j];
            scores[j] = tmpFloat;

            tmpMove.from  = moves[i].from;                          //  tmp gets [i].
            tmpMove.to    = moves[i].to;
            tmpMove.promo = moves[i].promo;

            moves[i].from  = moves[j].from;                         //  [i] gets [j].
            moves[i].to    = moves[j].to;
            moves[i].promo = moves[j].promo;

            moves[j].from  = tmpMove.from;                          //  [j] gets tmp.
            moves[j].to    = tmpMove.to;
            moves[j].promo = tmpMove.promo;

            i++;
          }
      }

    tmpFloat   = scores[i];                                         //  Swap scores[i] with scores[hi].
    scores[i]  = scores[hi];
    scores[hi] = tmpFloat;

    tmpMove.from  = moves[i].from;                                  //  tmp gets [i].
    tmpMove.to    = moves[i].to;
    tmpMove.promo = moves[i].promo;

    moves[i].from  = moves[hi].from;                                //  [i] gets [hi].
    moves[i].to    = moves[hi].to;
    moves[i].promo = moves[hi].promo;

    moves[hi].from  = tmpMove.from;                                 //  [hi] gets tmp.
    moves[hi].to    = tmpMove.to;
    moves[hi].promo = tmpMove.promo;

    return i;
  }

*/