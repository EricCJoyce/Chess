/*

Game logic module for the human player.

sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c emcc -Os -s STANDALONE_WASM -s EXPORTED_FUNCTIONS="['_getCurrentState','_getMovesBuffer','_sideToMove_client','_isWhite_client','_isBlack_client','_isEmpty_client','_isPawn_client','_isKnight_client','_isBishop_client','_isRook_client','_isQueen_client','_isKing_client','_getMovesIndex_client','_makeMove_client','_isTerminal_client','_isWin_client','_draw']" -Wl,--no-entry "gamelogic.c" -o "gamelogic.wasm"

*/

#include "gamestate.h"

/**************************************************************************************************
 Typedefs  */


/**************************************************************************************************
 Prototypes  */

__attribute__((import_module("env"), import_name("_printRow"))) void printRow(char a, char b, char c, char d, char e, char f, char g, char h);
__attribute__((import_module("env"), import_name("_printGameStateData"))) void printGameStateData(bool wToMove,
                                                                                                  bool wKingside, bool wQueenside, bool wCastled,
                                                                                                  bool bKingside, bool bQueenside, bool bCastled,
                                                                                                  unsigned char previousDoubleMoveColumn, unsigned char moveCtr);
unsigned char* getCurrentState(void);
unsigned char* getMovesBuffer(void);
void serialize(GameState*);
void deserialize(GameState*);

unsigned char sideToMove_client(void);
bool isWhite_client(unsigned char);
bool isBlack_client(unsigned char);
bool isEmpty_client(unsigned char);
bool isPawn_client(unsigned char);
bool isKnight_client(unsigned char);
bool isBishop_client(unsigned char);
bool isRook_client(unsigned char);
bool isQueen_client(unsigned char);
bool isKing_client(unsigned char);

void getMovesIndex_client(unsigned char);
void makeMove_client(unsigned char, unsigned char, unsigned char);
bool isTerminal_client(void);
unsigned char isWin_client(void);

void draw(void);

/**************************************************************************************************
 Globals  */

unsigned char currentState[_GAMESTATE_BYTE_SIZE];                   //  Global array containing the serialized game state.
unsigned char movesBuffer[_MAX_NUM_TARGETS + 4];                    //  Global array containing an number (4 bytes) followed by
                                                                    //  the number of unique destinations (not necessarily the
                                                                    //  number of unique moves) available, followed by that many indices.

/**************************************************************************************************
 Functions  */

/* Expose the global array declared here to JavaScript.  */
unsigned char* getCurrentState(void)
  {
    return &currentState[0];
  }

/* Expose the global array declared here to JavaScript.  */
unsigned char* getMovesBuffer(void)
  {
    return &movesBuffer[0];
  }

/* Pack a GameState into the unsigned-char buffer "currentState". */
void serialize(GameState* gs)
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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

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
    currentState[i++] = ch;                                         //  Write byte.

    ch = 0;
    mask = 64;
    for(j = 1; j < 7; j++)
      {
        if(isWhite(7 + j * 8, gs) && isPawn(7 + j * 8, gs))
          ch |= mask;                                               //  Set bit: white pawn on j-th row of column H.
        mask = mask >> 1;
      }
    ch |= (gs->previousDoublePawnMove & 1);                         //  Set bit: previous pawn double-move occurred on column H.
    currentState[i++] = ch;                                         //  Write byte.

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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
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
        currentState[i++] = ch;                                     //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  Encode move counter.
    currentState[i++] = gs->moveCtr;

    return;
  }

/* Recover a GameState from the unsigned-char buffer "currentState". */
void deserialize(GameState* gs)
  {
    unsigned char i, j, k;
    unsigned char mask;
    unsigned char wKingPos = 0, bKingPos = 0;

    for(i = 0; i < _NONE; i++)                                      //  Fill-in/blank-out.
      gs->board[i] = 'e';
    gs->previousDoublePawnMove = 0;

    gs->whiteToMove = ((currentState[0] & 128) == 128);             //  Recover side to move.

    gs->whiteKingsidePrivilege = ((currentState[1] & 128) == 128);  //  Recover white's castling data.
    gs->whiteQueensidePrivilege = ((currentState[2] & 128) == 128);
    gs->whiteCastled = ((currentState[3] & 128) == 128);

    gs->blackKingsidePrivilege = ((currentState[4] & 128) == 128);  //  Recover black's castling data.
    gs->blackQueensidePrivilege = ((currentState[5] & 128) == 128);
    gs->blackCastled = ((currentState[6] & 128) == 128);

    gs->previousDoublePawnMove |= (currentState[0] & 1) << 7;       //  Recover en-passant data.
    gs->previousDoublePawnMove |= (currentState[1] & 1) << 6;
    gs->previousDoublePawnMove |= (currentState[2] & 1) << 5;
    gs->previousDoublePawnMove |= (currentState[3] & 1) << 4;
    gs->previousDoublePawnMove |= (currentState[4] & 1) << 3;
    gs->previousDoublePawnMove |= (currentState[5] & 1) << 2;
    gs->previousDoublePawnMove |= (currentState[6] & 1) << 1;
    gs->previousDoublePawnMove |= (currentState[7] & 1);

    wKingPos |= (currentState[8] & 128);                            //  Recover white king's position.
    wKingPos |= (currentState[9] & 128) >> 1;
    wKingPos |= (currentState[10] & 128) >> 2;
    wKingPos |= (currentState[11] & 128) >> 3;
    wKingPos |= (currentState[12] & 128) >> 4;
    wKingPos |= (currentState[13] & 128) >> 5;
    wKingPos |= (currentState[14] & 128) >> 6;
    wKingPos |= (currentState[15] & 128) >> 7;

    bKingPos |= (currentState[8] & 1) << 7;                         //  Recover black king's position.
    bKingPos |= (currentState[9] & 1) << 6;
    bKingPos |= (currentState[10] & 1) << 5;
    bKingPos |= (currentState[11] & 1) << 4;
    bKingPos |= (currentState[12] & 1) << 3;
    bKingPos |= (currentState[13] & 1) << 2;
    bKingPos |= (currentState[14] & 1) << 1;
    bKingPos |= (currentState[15] & 1);

    i = 0;                                                          //  Reset to head of byte array.

    for(j = 0; j < 8; j++)                                          //  Recover white pawns.
      {
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'P';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black pawns.
      {
        mask = 64;
        for(k = 1; k < 7; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'p';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white knights.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'N';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black knights.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'n';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white bishops.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'B';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black bishops.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'b';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white rooks.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'R';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black rooks.
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'r';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover white queen(s).
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'Q';
            mask = mask >> 1;
          }
        i++;
      }

    for(j = 0; j < 8; j++)                                          //  Recover black queen(s).
      {
        mask = 128;
        for(k = 0; k < 8; k++)
          {
            if((currentState[i] & mask) == mask)
              gs->board[j + k * 8] = 'q';
            mask = mask >> 1;
          }
        i++;
      }

    gs->board[ wKingPos ] = 'K';                                    //  Restore white king.
    gs->board[ bKingPos ] = 'k';                                    //  Restore black king.

    gs->moveCtr = currentState[i++];                                //  Recover move counter.

    return;
  }

/* Answer the client-side question, Whose turn is it? */
unsigned char sideToMove_client(void)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return (gs.whiteToMove) ? _WHITE_TO_MOVE : _BLACK_TO_MOVE;
  }

bool isWhite_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isWhite(index, &gs);
  }

bool isBlack_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isBlack(index, &gs);
  }

bool isEmpty_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isEmpty(index, &gs);
  }

bool isPawn_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isPawn(index, &gs);
  }

bool isKnight_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isKnight(index, &gs);
  }

bool isBishop_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isBishop(index, &gs);
  }

bool isRook_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isRook(index, &gs);
  }

bool isQueen_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isQueen(index, &gs);
  }

bool isKing_client(unsigned char index)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isKing(index, &gs);
  }

bool isTerminal_client(void)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return terminal(&gs);
  }

/* Returns unsigned char in {GAME_ONGOING         = 0,
                             GAME_OVER_WHITE_WINS = 1,
                             GAME_OVER_BLACK_WINS = 2,
                             GAME_OVER_STALEMATE  = 3}. */
unsigned char isWin_client(void)
  {
    GameState gs;
    deserialize(&gs);                                               //  Recover GameState from buffer.
    return isWin(&gs);
  }

/* Given an index, recover the game state from the global buffer "currentState", and compute the moves available to the piece at "index."
   The number of moves is written (as a 4-byte unsigned int) to movesBuffer[0, 3], then every following byte in "movesBuffer" will contain a destination.
   This function is intended to answer queries from the human player. */
void getMovesIndex_client(unsigned char index)
  {
    GameState gs;
    Move moves[_MAX_NUM_TARGETS];                                   //  Generous upper bound assumes that a single piece could reach half of all squares.
    unsigned int len, i = 0, j;
    unsigned int ctr;
    unsigned char buffer4[4];
    unsigned char indices[_MAX_NUM_TARGETS];

    deserialize(&gs);                                               //  Recover GameState from buffer.
    len = getMovesIndex(index, &gs, moves);

    ctr = 0;
    for(i = 0; i < len; i++)                                        //  Iterate through moves for index and identify unique destination indices.
      {
        j = 0;
        while(j < ctr && indices[j] != moves[i].to)
          j++;
        if(j == ctr)
          indices[ctr++] = moves[i].to;
      }

    i = 0;                                                          //  Reset. 'i' now iterates into 'movesBuffer'.
    memcpy(buffer4, (unsigned char*)(&ctr), 4);                     //  Force the unsigned integer into a 4-byte temp buffer.
    for(j = 0; j < 4; j++)                                          //  Copy bytes to serial buffer.
      movesBuffer[i++] = buffer4[j];

    for(len = 0; len < ctr; len++)
      movesBuffer[i++] = indices[len];

    return;
  }

/* Update "currentState" according to the given move data (if those data are indeed valid!) */
void makeMove_client(unsigned char from, unsigned char to, unsigned char promo)
  {
    GameState gs;
    Move moves[_NONE];                                              //  Generous assumption that every square is reachable.
    Move move;
    unsigned int len, i;

    deserialize(&gs);                                               //  Recover GameState from buffer.
    len = getMovesIndex(from, &gs, moves);                          //  Make sure that this move is legal.
    i = 0;                                                          //  Otherwise, ignore it. Cheaters lose their turns!
    while(i < len && !(moves[i].from == from && moves[i].to == to && moves[i].promo == promo))
      i++;
    if(i < len)
      {
        move.from = from;
        move.to = to;
        move.promo = promo;
        makeMove(&move, &gs);
      }

    movesBuffer[0] = 0;                                             //  Blank out the number of moves in the buffer.
    movesBuffer[1] = 0;
    movesBuffer[2] = 0;
    movesBuffer[3] = 0;

    serialize(&gs);                                                 //  Write updated GameState back to buffer.

    return;
  }

/* Draw the board to the JavaScript console.
   r n b q k b n r
   p p p p p p p p
   . . . . . . . .
   . . . . . . . .
   . . . . . . . .
   . . . . . . . .
   P P P P P P P P
   R N B Q K B N R */
void draw(void)
  {
    GameState gs;
    signed char y;

    deserialize(&gs);                                               //  Recover GameState from buffer.

    for(y = 7; y >= 0; y--)
      printRow(gs.board[y * 8], gs.board[y * 8 + 1], gs.board[y * 8 + 2], gs.board[y * 8 + 3], gs.board[y * 8 + 4], gs.board[y * 8 + 5], gs.board[y * 8 + 6], gs.board[y * 8 + 7]);

    printGameStateData(gs.whiteToMove, gs.whiteKingsidePrivilege, gs.whiteQueensidePrivilege, gs.whiteCastled,
                                       gs.blackKingsidePrivilege, gs.blackQueensidePrivilege, gs.blackCastled,
                       gs.previousDoublePawnMove, gs.moveCtr);
    return;
  }
