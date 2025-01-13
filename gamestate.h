#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include <ctype.h>
#include <math.h>                                                   /* Needed for INFINITY. */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define _NONE                    64
#define _NO_PROMO                 0
#define _PROMO_KNIGHT             1
#define _PROMO_BISHOP             2
#define _PROMO_ROOK               3
#define _PROMO_QUEEN              4

#define _WHITE_TO_MOVE            0
#define _BLACK_TO_MOVE            1

#define GAME_ONGOING              0
#define GAME_OVER_WHITE_WINS      1
#define GAME_OVER_BLACK_WINS      2
#define GAME_OVER_STALEMATE       3

#define _GAMESTATE_BYTE_SIZE     81                                 /* Number of bytes needed to store a GameState structure. */
#define _MOVE_BYTE_SIZE           3                                 /* Number of bytes needed to store a Move structure. */
#define _MAX_NUM_TARGETS         32                                 /* A (generous) upper bound on how many distinct destinations (not distinct moves)
                                                                       may be available to a player from a single index. */
#define _MAX_MOVES               64                                 /* A (generous) upper bound on how many moves are available to a team in a single turn. */

/**************************************************************************************************
 Typedefs  */

typedef struct GameStateType
  {
    bool whiteToMove;                                               //  True: white to move. False: black to move.
    char board[_NONE];                                              //  Array of characters.

    bool whiteKingsidePrivilege;                                    //  Can white castle kingside?
    bool whiteQueensidePrivilege;                                   //  Can white castle queenside?
    bool whiteCastled;                                              //  Has white already castled, to either side?

    bool blackKingsidePrivilege;                                    //  Can black castle kingside?
    bool blackQueensidePrivilege;                                   //  Can black castle queenside?
    bool blackCastled;                                              //  Has black already castled, to either side?

    unsigned char previousDoublePawnMove;                           //  Bit flagged indicates that the previous move was a pawn's double-move
                                                                    //  in the corresponding column:
                                                                    //  [0] [1] [2] [3] [4] [5] [6] [7]
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   .   .   .   .   .   .   .   .
                                                                    //   A   B   C   D   E   F   G   H
    unsigned char moveCtr;                                          //  The 50-move rule states that a player can claim a draw if no capture has been made
                                                                    //  and no pawn has been moved in the last 50 moves (for this purpose a "move" consists
                                                                    //  of a player completing a turn followed by the opponent completing a turn).
                                                                    //  The purpose of this rule is to prevent a player with no chance of winning from
                                                                    //  continuing to play indefinitely or tiring the opponent.
  } GameState;

typedef struct MoveType                                             //  TOTAL: 3 bytes.
  {
    unsigned char from;                                             //  Index in [0, 64).
    unsigned char to;                                               //  Index in [0, 64).
    unsigned char promo;                                            //  _NO_PROMO = no promotion
  } Move;

/**************************************************************************************************
 Prototypes  */

void copyGameState(GameState*, GameState*);

void makeMove(Move*, GameState*);
char nowToMove(GameState*);
char nextToMove(GameState*);
bool inCheckBy(unsigned char, unsigned char, GameState*);
bool canKingsideCastle(unsigned char, GameState*);
bool canQueensideCastle(unsigned char, GameState*);
unsigned int getMoves(GameState*, Move*);
unsigned int getMovesIndex(unsigned char, GameState*, Move*);

unsigned int getPawnMoves(unsigned char, GameState*, Move*);
unsigned int getPawnAttackable(unsigned char, GameState*, Move*);
unsigned int getPawnEnPassantAttacks(unsigned char, GameState*, Move*);
bool isEnPassantAttack(Move*, GameState*);
unsigned char enPassantVictim(Move*, GameState*);
bool isPawnDoubleMove(unsigned char, unsigned char, GameState*);

unsigned int getKnightMoves(unsigned char, GameState*, Move*);
unsigned int getBishopMoves(unsigned char, GameState*, Move*);
unsigned int getRookMoves(unsigned char, GameState*, Move*);
unsigned int getQueenMoves(unsigned char, GameState*, Move*);
unsigned int getKingMoves(unsigned char, GameState*, Move*);
unsigned int getKingNonCastle(unsigned char, GameState*, Move*);

unsigned char isWin(GameState*);
bool terminal(GameState*);

bool isEmpty(unsigned char, GameState*);
bool isWhite(unsigned char, GameState*);
bool isBlack(unsigned char, GameState*);
bool sameSide(unsigned char, unsigned char, GameState*);
bool opposed(unsigned char, unsigned char, GameState*);
char getTeam(unsigned char, GameState*);
bool isPawn(unsigned char, GameState*);
bool isKnight(unsigned char, GameState*);
bool isBishop(unsigned char, GameState*);
bool isRook(unsigned char, GameState*);
bool isQueen(unsigned char, GameState*);
bool isKing(unsigned char, GameState*);

bool whiteKingsidePrivilege(GameState*);
bool whiteQueensidePrivilege(GameState*);
bool whiteCastled(GameState*);
bool blackKingsidePrivilege(GameState*);
bool blackQueensidePrivilege(GameState*);
bool blackCastled(GameState*);
bool isCastle(Move*, GameState*);
bool isWhiteKingside(Move*, GameState*);
bool isWhiteQueenside(Move*, GameState*);
bool isBlackKingside(Move*, GameState*);
bool isBlackQueenside(Move*, GameState*);

unsigned char uSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char dSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char lSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char rSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char ulSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char urSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char drSet(unsigned char, char*, GameState*, unsigned char*);
unsigned char dlSet(unsigned char, char*, GameState*, unsigned char*);

unsigned char u(unsigned char);
unsigned char d(unsigned char);
unsigned char l(unsigned char);
unsigned char r(unsigned char);
unsigned char ul(unsigned char);
unsigned char ur(unsigned char);
unsigned char dl(unsigned char);
unsigned char dr(unsigned char);
unsigned char row(unsigned char);
unsigned char col(unsigned char);

/**************************************************************************************************
 Globals  */


/**************************************************************************************************
 Functions  */

void copyGameState(GameState* src, GameState* dst)
  {
    unsigned char i;

    dst->whiteToMove = src->whiteToMove;

    for(i = 0; i < _NONE; i++)
      dst->board[i] = src->board[i];

    dst->whiteKingsidePrivilege  = src->whiteKingsidePrivilege;
    dst->whiteQueensidePrivilege = src->whiteQueensidePrivilege;
    dst->whiteCastled            = src->whiteCastled;

    dst->blackKingsidePrivilege  = src->blackKingsidePrivilege;
    dst->blackQueensidePrivilege = src->blackQueensidePrivilege;
    dst->blackCastled            = src->blackCastled;

    dst->previousDoublePawnMove = src->previousDoublePawnMove;
    dst->moveCtr = src->moveCtr;

    return;
  }

/**************************************************************************************************
 Move generation and Application  */

void makeMove(Move* move, GameState* gs)
  {
    gs->previousDoublePawnMove = 0;                                 //  Zero this out, unless appropriate to flag a bit.

    if(isBlackKingside(move, gs))                                   //  Black Kingside-Castle
      {
        gs->board[60] = 'e';
        gs->board[61] = 'r';
        gs->board[62] = 'k';
        gs->board[63] = 'e';
        gs->blackKingsidePrivilege = false;                         //  Black cannot Kingside.
        gs->blackQueensidePrivilege = false;                        //  Black cannot Queenside.
        gs->blackCastled = true;                                    //  Black has castled.
      }
    else if(isBlackQueenside(move, gs))                             //  Black Queenside-Castle
      {
        gs->board[60] = 'e';
        gs->board[59] = 'r';
        gs->board[58] = 'k';
        gs->board[57] = 'e';
        gs->board[56] = 'e';
        gs->blackKingsidePrivilege = false;                         //  Black cannot Kingside.
        gs->blackQueensidePrivilege = false;                        //  Black cannot Queenside.
        gs->blackCastled = true;                                    //  Black has castled.
      }
    else if(isWhiteKingside(move, gs))                              //  White Kingside-Castle
      {
        gs->board[4] = 'e';
        gs->board[5] = 'R';
        gs->board[6] = 'K';
        gs->board[7] = 'e';
        gs->whiteKingsidePrivilege = false;                         //  White cannot Kingside.
        gs->whiteQueensidePrivilege = false;                        //  White cannot Queenside.
        gs->whiteCastled = true;                                    //  White has castled.
      }
    else if(isWhiteQueenside(move, gs))                             //  White Queenside-Castle
      {
        gs->board[4] = 'e';
        gs->board[3] = 'R';
        gs->board[2] = 'K';
        gs->board[1] = 'e';
        gs->board[0] = 'e';
        gs->whiteKingsidePrivilege = false;                         //  White cannot Kingside.
        gs->whiteQueensidePrivilege = false;                        //  White cannot Queenside.
        gs->whiteCastled = true;                                    //  White has castled.
      }
    else if(isEnPassantAttack(move, gs))                            //  En-passant capture
      {
        gs->board[ enPassantVictim(move, gs) ] = 'e';
        gs->board[move->to] = gs->board[move->from];
        gs->board[move->from] = 'e';

        gs->moveCtr = 0;                                            //  Capture resets the 50-move counter.
      }
    else
      {
        if(isKing(move->from, gs) && isWhite(move->from, gs))       //  White King moved: castling rights lost.
          {
            gs->whiteKingsidePrivilege = false;                     //  White cannot Kingside.
            gs->whiteQueensidePrivilege = false;                    //  White cannot Queenside.
          }
                                                                    //  White King's Rook moved: Kingside rights lost.
        else if(isRook(move->from, gs) && isWhite(move->from, gs) && move->from == 7)
          gs->whiteKingsidePrivilege = false;                       //  White cannot Kingside.
                                                                    //  White Queen's Rook moved: Queenside rights lost.
        else if(isRook(move->from, gs) && isWhite(move->from, gs) && move->from == 0)
          gs->whiteQueensidePrivilege = false;                      //  White cannot Queenside.
        else if(isKing(move->from, gs) && isBlack(move->from, gs))  //  Black King moved: castling rights lost.
          {
            gs->blackKingsidePrivilege = false;                     //  Black cannot Kingside.
            gs->blackQueensidePrivilege = false;                    //  Black cannot Queenside.
          }
                                                                    //  Black King's Rook moved: Kingside rights lost.
        else if(isRook(move->from, gs) && isBlack(move->from, gs) && move->from == 63)
          gs->blackKingsidePrivilege = false;                       //  Black cannot Kingside.
                                                                    //  Black Queen's Rook moved: Queenside rights lost.
        else if(isRook(move->from, gs) && isBlack(move->from, gs) && move->from == 56)
          gs->blackQueensidePrivilege = false;                      //  Black cannot Queenside.
                                                                    //  Pawn promotion
        if(isPawn(move->from, gs) && move->promo != _NO_PROMO && (row(move->to) == 7 || row(move->to) == 0))
          {
            if(isWhite(move->from, gs))
              {
                switch(move->promo)
                  {
                    case _PROMO_KNIGHT: gs->board[move->to] = 'N';  break;
                    case _PROMO_BISHOP: gs->board[move->to] = 'B';  break;
                    case _PROMO_ROOK:   gs->board[move->to] = 'R';  break;
                    case _PROMO_QUEEN:  gs->board[move->to] = 'Q';  break;
                  }
              }
            else
              {
                switch(move->promo)
                  {
                    case _PROMO_KNIGHT: gs->board[move->to] = 'n';  break;
                    case _PROMO_BISHOP: gs->board[move->to] = 'b';  break;
                    case _PROMO_ROOK:   gs->board[move->to] = 'r';  break;
                    case _PROMO_QUEEN:  gs->board[move->to] = 'q';  break;
                  }
              }
            gs->board[move->from] = 'e';

            gs->moveCtr = 0;                                        //  Pawn move resets the 50-move counter.
          }
        else                                                        //  Any other case.
          {
            if(isPawnDoubleMove(move->from, move->to, gs))          //  Save last move IFF last move was a pawn double-move!
              {
                switch(col(move->from))
                  {
                    case 0: gs->previousDoublePawnMove = 128;       //  Double move occurred in Column A.
                            break;
                    case 1: gs->previousDoublePawnMove =  64;       //  Double move occurred in Column B.
                            break;
                    case 2: gs->previousDoublePawnMove =  32;       //  Double move occurred in Column C.
                            break;
                    case 3: gs->previousDoublePawnMove =  16;       //  Double move occurred in Column D.
                            break;
                    case 4: gs->previousDoublePawnMove =   8;       //  Double move occurred in Column E.
                            break;
                    case 5: gs->previousDoublePawnMove =   4;       //  Double move occurred in Column F.
                            break;
                    case 6: gs->previousDoublePawnMove =   2;       //  Double move occurred in Column G.
                            break;
                    case 7: gs->previousDoublePawnMove =   1;       //  Double move occurred in Column H.
                            break;
                  }
              }

            if(isPawn(move->from, gs) || !isEmpty(move->to, gs))    //  Pawn move or capture reset the 50-move counter.
              gs->moveCtr = 0;
            else                                                    //  Otherwise, increase the counter.
              gs->moveCtr++;

            gs->board[move->to] = gs->board[move->from];
            gs->board[move->from] = 'e';
          }
      }

    gs->whiteToMove = !gs->whiteToMove;                             //  Flip flag.

    return;
  }

/* Return a character indicating who is to move now. */
char nowToMove(GameState* gs)
  {
    return (gs->whiteToMove) ? 'w' : 'b';
  }

/* Return a character indicating who is next to move (once the side to move has played). */
char nextToMove(GameState* gs)
  {
    return (gs->whiteToMove) ? 'b' : 'w';
  }

/* 'team' is holding 'index' in check. */
bool inCheckBy(unsigned char index, unsigned char team, GameState* gs)
  {
    bool ret = false;
    unsigned char enemytargets[256];                                //  Generous upper bound attacks every square four times.
    unsigned int enemyStrikeCtr = 0;
    unsigned char i;
    unsigned int j, len = 0;
    Move tmp[_NONE];                                                //  Assumes generous upper bound of all squares being reachable.

    for(i = 0; i < _NONE; i++)
      {
        if((isWhite(i, gs) && team == 'w') || (isBlack(i, gs) && team == 'b'))
          {
            if(isPawn(i, gs))
              len = getPawnAttackable(i, gs, tmp);
            else if(isKnight(i, gs))
              len = getKnightMoves(i, gs, tmp);
            else if(isBishop(i, gs))
              len = getBishopMoves(i, gs, tmp);
            else if(isRook(i, gs))
              len = getRookMoves(i, gs, tmp);
            else if(isQueen(i, gs))
              len = getQueenMoves(i, gs, tmp);
            else
              len = getKingNonCastle(i, gs, tmp);

            for(j = 0; j < len; j++)
              enemytargets[enemyStrikeCtr + j] = tmp[j].to;

            enemyStrikeCtr += len;                                  //  Count up strikes
          }
      }

    j = 0;
    while(j < enemyStrikeCtr && enemytargets[j] != index)
      j++;

    if(j < enemyStrikeCtr)
      ret = true;

    return ret;
  }

/* This means, "Can I castle RIGHT NOW?" Not, "Do I still have Kingside rights?" */
bool canKingsideCastle(unsigned char team, GameState* gs)
  {
    bool c = false;
    if(team == 'b' && gs->blackKingsidePrivilege && isRook(63, gs) && isBlack(63, gs) && !inCheckBy(60, 'w', gs) && !inCheckBy(61, 'w', gs) && !inCheckBy(62, 'w', gs) && isEmpty(61, gs) && isEmpty(62, gs))
      c = true;
    else if(gs->whiteKingsidePrivilege && isRook(7, gs) && isWhite(7, gs) && !inCheckBy(4, 'b', gs) && !inCheckBy(5, 'b', gs) && !inCheckBy(6, 'b', gs) && isEmpty(5, gs) && isEmpty(6, gs))
      c = true;
    return c;
  }

/* This means, "Can I castle RIGHT NOW?" Not, "Do I still have Queenside rights?" */
bool canQueensideCastle(unsigned char team, GameState* gs)
  {
    bool c = false;
    if(team == 'b' && gs->blackQueensidePrivilege && isRook(56, gs) && isBlack(56, gs) && !inCheckBy(60, 'w', gs) && !inCheckBy(59, 'w', gs) && !inCheckBy(58, 'w', gs) && isEmpty(59, gs) && isEmpty(58, gs) && isEmpty(57, gs))
      c = true;
    else if(gs->whiteQueensidePrivilege && isRook(0, gs) && isWhite(0, gs) && !inCheckBy(4, 'b', gs) && !inCheckBy(3, 'b', gs) && !inCheckBy(2, 'b', gs) && isEmpty(3, gs) && isEmpty(2, gs) && isEmpty(1, gs))
      c = true;
    return c;
  }

/* THIS FUNCTION FILTERS FOR CHECK!!
   Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getMoves(GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    Move potentialmoves[_NONE];                                     //  Assumes generous upper bound of 64 moves per piece.
    unsigned int potentialmovesCtr = 0;
    unsigned int i;
    unsigned char index;

    for(index = 0; index < _NONE; index++)
      {
        if((gs->whiteToMove && isWhite(index, gs)) || (!gs->whiteToMove && isBlack(index, gs)))
          {
            potentialmovesCtr = getMovesIndex(index, gs, potentialmoves);
            if(potentialmovesCtr > 0)
              {
                for(i = 0; i < potentialmovesCtr; i++)
                  {
                    buffer[movesCtr].from = potentialmoves[i].from;
                    buffer[movesCtr].to = potentialmoves[i].to;
                    buffer[movesCtr].promo = potentialmoves[i].promo;
                    movesCtr++;
                  }
              }
          }
      }

    return movesCtr;
  }

/* THIS FUNCTION FILTERS FOR CHECK!!
   Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getMovesIndex(unsigned char index, GameState* gs, Move* buffer)
  {
    Move potentialmoves[_NONE];                                     //  Assumes a generous upper bound of all squares being reachable from a single index.
    unsigned int potentialmovesCtr = 0;
    unsigned int movesCtr = 0;
    unsigned int i;
    unsigned char j;
    GameState tmp;

    if(!isEmpty(index, gs))
      {
        if(isPawn(index, gs))
          potentialmovesCtr = getPawnMoves(index, gs, potentialmoves);
        else if(isKnight(index, gs))
          potentialmovesCtr = getKnightMoves(index, gs, potentialmoves);
        else if(isBishop(index, gs))
          potentialmovesCtr = getBishopMoves(index, gs, potentialmoves);
        else if(isRook(index, gs))
          potentialmovesCtr = getRookMoves(index, gs, potentialmoves);
        else if(isQueen(index, gs))
          potentialmovesCtr = getQueenMoves(index, gs, potentialmoves);
        else
          potentialmovesCtr = getKingMoves(index, gs, potentialmoves);

        if(isWhite(index, gs))                                      //  Piece is white, check for checks on the King by Black
          {
            for(i = 0; i < potentialmovesCtr; i++)                  //  For every move, make that move, then test the resultant board
              {
                copyGameState(gs, &tmp);                            //  Copy the board
                makeMove(potentialmoves + i, &tmp);                 //  Apply the candidate move

                j = 0;                                              //  Locate the king on the new board
                while(j < _NONE && tmp.board[j] != 'K')
                  j++;

                if(!inCheckBy(j, 'b', &tmp))                        //  If king not in check, then move is allowed
                  {
                    buffer[movesCtr].from = potentialmoves[i].from;
                    buffer[movesCtr].to = potentialmoves[i].to;
                    buffer[movesCtr].promo = potentialmoves[i].promo;
                    movesCtr++;
                  }
              }
          }
        else                                                        //  Piece is black, check for checks on the King by White
          {
            for(i = 0; i < potentialmovesCtr; i++)                  //  For every move, make that move, then test the resultant board
              {
                copyGameState(gs, &tmp);                            //  Copy the board
                makeMove(potentialmoves + i, &tmp);                 //  Apply the candidate move

                j = 0;                                              //  Locate the king on the new board
                while(j < _NONE && tmp.board[j] != 'k')
                  j++;

                if(!inCheckBy(j, 'w', &tmp))                        //  If king not in check, then move is allowed
                  {
                    buffer[movesCtr].from = potentialmoves[i].from;
                    buffer[movesCtr].to = potentialmoves[i].to;
                    buffer[movesCtr].promo = potentialmoves[i].promo;
                    movesCtr++;
                  }
              }
          }
      }

    return movesCtr;
  }

/**************************************************************************************************
 Pawn functions  */

/* Pawn moves are all moves a
   pawn may make, including attacks, double moves and en passant captures. */
unsigned int getPawnMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char i, len = 0;
    Move tmp[1];                                                    //  There may only be one en-passant attack available per pawn at a time.

    if(isWhite(index, gs))
      {
                                                                    //  White's double Pawn move
        if(row(index) == 1 && u(u(index)) < _NONE && isEmpty(u(index), gs) && isEmpty(u(u(index)), gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = u(u(index));
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(u(index) < _NONE && isEmpty(u(index), gs))               //  White Pawn forward, possibly to promotion
          {
            if(row(u(index)) == 7)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = u(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = u(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = u(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = u(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = u(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
                                                                    //  White Pawn attack forward-left, possibly to promotion
        if(ul(index) < _NONE && row(ul(index)) == row(index) + 1 && col(ul(index)) == col(index) - 1 && isBlack(ul(index), gs))
          {
            if(row(ul(index)) == 7)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ul(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ul(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ul(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ul(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ul(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
                                                                    //  White Pawn attack forward-right, possibly to promotion
        if(ur(index) < _NONE && row(ur(index)) == row(index) + 1 && col(ur(index)) == col(index) + 1 && isBlack(ur(index), gs))
          {
            if(row(ur(index)) == 7)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ur(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ur(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ur(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ur(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = ur(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
        len = getPawnEnPassantAttacks(index, gs, tmp);
        for(i = 0; i < len; i++)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmp[i].to;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }
    else
      {
                                                                    //  Black's double Pawn move
        if(row(index) == 6 && d(d(index)) < _NONE && isEmpty(d(index), gs) && isEmpty(d(d(index)), gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = d(d(index));
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(d(index) < _NONE && isEmpty(d(index), gs))               //  Black Pawn forward, possibly to promotion
          {
            if(row(d(index)) == 0)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = d(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = d(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = d(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = d(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = d(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
                                                                    //  Black Pawn attack forward-left, possibly to promotion
        if(dl(index) < _NONE && row(dl(index)) == row(index) - 1 && col(dl(index)) == col(index) - 1 && isWhite(dl(index), gs))
          {
            if(row(dl(index)) == 0)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dl(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dl(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dl(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dl(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dl(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
                                                                    //  Black Pawn attack forward-right, possibly to promotion
        if(dr(index) < _NONE && row(dr(index)) == row(index) - 1 && col(dr(index)) == col(index) + 1 && isWhite(dr(index), gs))
          {
            if(row(dr(index)) == 0)
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dr(index);
                buffer[movesCtr].promo = _PROMO_KNIGHT;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dr(index);
                buffer[movesCtr].promo = _PROMO_BISHOP;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dr(index);
                buffer[movesCtr].promo = _PROMO_ROOK;
                movesCtr++;
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dr(index);
                buffer[movesCtr].promo = _PROMO_QUEEN;
                movesCtr++;
              }
            else
              {
                buffer[movesCtr].from = index;
                buffer[movesCtr].to = dr(index);
                buffer[movesCtr].promo = _NO_PROMO;
                movesCtr++;
              }
          }
        len = getPawnEnPassantAttacks(index, gs, tmp);
        for(i = 0; i < len; i++)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmp[i].to;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* This function only tests that the forward diagonals of "index" do not fall off the board.
   This function does not consider whether the attackable squares are occupied,
   does not consider pawn promotion, and does not consider en-passant attacks. */
unsigned int getPawnAttackable(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned char movesCtr = 0;

    if(isWhite(index, gs))
      {
        if(ul(index) < _NONE)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = ul(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(ur(index) < _NONE)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = ur(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }
    else if(isBlack(index, gs))
      {
        if(dl(index) < _NONE)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = dl(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(dr(index) < _NONE)
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = dr(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getPawnEnPassantAttacks(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(gs->previousDoublePawnMove > 0 && isPawn(index, gs))         //  Did a pawn double-move previously occur at all, and is the piece at "index" a pawn?
      {
        switch(gs->previousDoublePawnMove)
          {
            case 128:                                               //  Previous pawn double-move occurred in column A.
                       if(col(index) == 1)                          //  "index" is in the column next to column A, where the double move occurred.
                         {
                           if(isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 64:                                                //  Previous pawn double-move occurred in column B.
                       if(col(index) == 0 || col(index) == 2)       //  "index" is in the column next to column B, where the double move occurred.
                         {
                           if(col(index) == 0 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 2 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 0 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 2 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 32:                                                //  Previous pawn double-move occurred in column C.
                       if(col(index) == 1 || col(index) == 3)       //  "index" is in the column next to column C, where the double move occurred.
                         {
                           if(col(index) == 1 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 3 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 1 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 3 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 16:                                                //  Previous pawn double-move occurred in column D.
                       if(col(index) == 2 || col(index) == 4)       //  "index" is in the column next to column D, where the double move occurred.
                         {
                           if(col(index) == 2 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 4 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 2 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 4 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 8:                                                 //  Previous pawn double-move occurred in column E.
                       if(col(index) == 3 || col(index) == 5)       //  "index" is in the column next to column E, where the double move occurred.
                         {
                           if(col(index) == 3 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 5 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 3 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 5 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 4:                                                 //  Previous pawn double-move occurred in column F.
                       if(col(index) == 4 || col(index) == 6)       //  "index" is in the column next to column F, where the double move occurred.
                         {
                           if(col(index) == 4 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 6 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 4 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 6 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 2:                                                 //  Previous pawn double-move occurred in column G.
                       if(col(index) == 5 || col(index) == 7)       //  "index" is in the column next to column G, where the double move occurred.
                         {
                           if(col(index) == 5 && isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 7 && isWhite(index, gs) && row(index) == 4 && isBlack(l(index), gs) && isPawn(l(index), gs) && isEmpty(ul(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ul(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 5 && isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(col(index) == 7 && isBlack(index, gs) && row(index) == 3 && isWhite(l(index), gs) && isPawn(l(index), gs) && isEmpty(dl(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dl(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
            case 1:                                                 //  Previous pawn double-move occurred in column H.
                       if(col(index) == 6)                          //  "index" is in the column next to column H, where the double move occurred.
                         {
                           if(isWhite(index, gs) && row(index) == 4 && isBlack(r(index), gs) && isPawn(r(index), gs) && isEmpty(ur(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = ur(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                           else if(isBlack(index, gs) && row(index) == 3 && isWhite(r(index), gs) && isPawn(r(index), gs) && isEmpty(dr(index), gs))
                             {
                               buffer[movesCtr].from = index;
                               buffer[movesCtr].to = dr(index);
                               buffer[movesCtr].promo = _NO_PROMO;
                               movesCtr++;
                             }
                         }
                       break;
          }
      }

    return movesCtr;
  }

/* Given the GameState "gs", does the Move "move" describe an en-passant capture? */
bool isEnPassantAttack(Move* move, GameState* gs)
  {
                                                                    //  Did any double move occur at all,
                                                                    //  and does the move either leave from row 4 and arrive in row 5,
                                                                    //                        or leave from row 3 and arrive in row 2?
                                                                    //  En-passant captures made by white only occur from row 4 to row 5, and
                                                                    //  en-passant captures made by black only occur from row 3 to row 2.
    if(gs->previousDoublePawnMove > 0 && ( (row(move->from) == 4 && row(move->to) == 5) ||
                                           (row(move->from) == 3 && row(move->to) == 2) ))
      {
        switch(gs->previousDoublePawnMove)                          //  In which column did the pawn double-move previously occur?
          {
            case 128:                                               //  Pawn double-move previously occurred in column A.
                                                                    //  Move takes a pawn to an empty square in column A.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 0)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && isBlack(l(move->from), gs) && move->to == ul(move->from))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && isWhite(l(move->from), gs) && move->to == dl(move->from))
                            return true;
                        }
                      break;
            case 64:                                                //  Pawn double-move previously occurred in column B.
                                                                    //  Move takes a pawn to an empty square in column B.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 1)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 32:                                                //  Pawn double-move previously occurred in column C.
                                                                    //  Move takes a pawn to an empty square in column C.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 2)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 16:                                                //  Pawn double-move previously occurred in column D.
                                                                    //  Move takes a pawn to an empty square in column D.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 3)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 8:                                                 //  Pawn double-move previously occurred in column E.
                                                                    //  Move takes a pawn to an empty square in column E.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 4)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 4:                                                 //  Pawn double-move previously occurred in column F.
                                                                    //  Move takes a pawn to an empty square in column F.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 5)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 2:                                                 //  Pawn double-move previously occurred in column G.
                                                                    //  Move takes a pawn to an empty square in column G.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 6)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && ( (isBlack(l(move->from), gs) && move->to == ul(move->from)) ||
                                                          (isBlack(r(move->from), gs) && move->to == ur(move->from)) ))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && ( (isWhite(l(move->from), gs) && move->to == dl(move->from)) ||
                                                          (isWhite(r(move->from), gs) && move->to == dr(move->from)) ))
                            return true;
                        }
                      break;
            case 1:                                                 //  Pawn double-move previously occurred in column H.
                                                                    //  Move takes a pawn to an empty square in column H.
                      if(isPawn(move->from, gs) && isEmpty(move->to, gs) && col(move->to) != col(move->from) && col(move->to) == 7)
                        {
                                                                    //  White captures black en passant.
                          if(isWhite(move->from, gs) && isBlack(r(move->from), gs) && move->to == ur(move->from))
                            return true;
                                                                    //  Black captures white en passant.
                          if(isBlack(move->from, gs) && isWhite(r(move->from), gs) && move->to == dr(move->from))
                            return true;
                        }
                      break;
          }
      }

    return false;
  }

/* If white has captured en passant, then the captured black pawn is below it.
   If black has captured en passant, then the captured white pawn is above it. */
unsigned char enPassantVictim(Move* move, GameState* gs)
  {
    return (isWhite(move->from, gs)) ? d(move->to) : u(move->to);
  }

/* Do the given values a (from) and b (to) and the given GameState indicate a double pawn move? */
bool isPawnDoubleMove(unsigned char a, unsigned char b, GameState* gs)
  {
    return (isPawn(a, gs) && ((isWhite(a, gs) && row(a) == 1 && row(b) == 3) || (isBlack(a, gs) && row(a) == 6 && row(b) == 4)));
  }

/**************************************************************************************************
 Non-Pawn pieces' functions  */

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getKnightMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(ul(u(index)) < _NONE && !sameSide(ul(u(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(u(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ur(u(index)) < _NONE && !sameSide(ur(u(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(u(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(ur(r(index)) < _NONE && !sameSide(ur(r(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(r(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ul(l(index)) < _NONE && !sameSide(ul(l(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(l(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(dl(d(index)) < _NONE && !sameSide(dl(d(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(d(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dr(d(index)) < _NONE && !sameSide(dr(d(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(d(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(dr(r(index)) < _NONE && !sameSide(dr(r(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(r(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dl(l(index)) < _NONE && !sameSide(dl(l(index)), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(l(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getBishopMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "B";
    else
      flags = "W";

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getRookMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "B";
    else
      flags = "W";

    len = uSet(index, flags, gs, tmpBuff);                          //  Up.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getQueenMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "B";
    else
      flags = "W";

    len = uSet(index, flags, gs, tmpBuff);                          //  Up.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-Right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-Right.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left.
    for(i = 0; i < len; i++)
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = tmpBuff[i];
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getKingMoves(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(u(index) < _NONE && !sameSide(u(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = u(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ur(index) < _NONE && !sameSide(ur(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(r(index) < _NONE && !sameSide(r(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = r(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dr(index) < _NONE && !sameSide(dr(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(d(index) < _NONE && !sameSide(d(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = d(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dl(index) < _NONE && !sameSide(dl(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(l(index) < _NONE && !sameSide(l(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = l(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ul(index) < _NONE && !sameSide(ul(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(isWhite(index, gs))
      {
        if(canKingsideCastle('w', gs))
          {
            buffer[movesCtr].from = 4;
            buffer[movesCtr].to = 6;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(canQueensideCastle('w', gs))
          {
            buffer[movesCtr].from = 4;
            buffer[movesCtr].to = 2;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }
    else
      {
        if(canKingsideCastle('b', gs))
          {
            buffer[movesCtr].from = 60;
            buffer[movesCtr].to = 62;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(canQueensideCastle('b', gs))
          {
            buffer[movesCtr].from = 60;
            buffer[movesCtr].to = 58;
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getKingNonCastle(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(u(index) < _NONE && !sameSide(u(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = u(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ur(index) < _NONE && !sameSide(ur(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(r(index) < _NONE && !sameSide(r(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = r(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dr(index) < _NONE && !sameSide(dr(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(d(index) < _NONE && !sameSide(d(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = d(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dl(index) < _NONE && !sameSide(dl(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(l(index) < _NONE && !sameSide(l(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = l(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ul(index) < _NONE && !sameSide(ul(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/**************************************************************************************************
 End-State Testing  */

/*  Given a board array, return
     GAME_ONGOING         if the state is not a win for either player.
     GAME_OVER_WHITE_WINS if the state is a win for White.
     GAME_OVER_BLACK_WINS if the state is a win for Black.
     GAME_OVER_STALEMATE  if the state is a stalemate. */
unsigned char isWin(GameState* gs)
  {
    Move moves[_NONE];                                              //  Generous upper-bound assumption that every square could be reachable.
    unsigned int len;
    unsigned char i;
    unsigned char kpos = 0;
    unsigned char wMatNonK = 0, bMatNonK = 0;                       //  Counts of pieces other than Kings.

    if(gs->moveCtr == 100)                                          //  Twice 50.
      return GAME_OVER_STALEMATE;

    len = getMoves(gs, moves);                                      //  Get moves for side to move.

    for(i = 0; i < _NONE; i++)                                      //  Count up all pieces that are not a King.
      {
        if(!isEmpty(i, gs) && !isKing(i, gs))
          {
            if(isWhite(i, gs))
              wMatNonK++;
            else
              bMatNonK++;
          }
      }

    if(len == 0)                                                    //  Game is over if side to move cannot move.
      {
        if(nowToMove(gs) == 'w')
          {
            while(kpos < _NONE && gs->board[kpos] != 'K')
              kpos++;
          }
        else
          {
            while(kpos < _NONE && gs->board[kpos] != 'k')
              kpos++;
          }

        if(nowToMove(gs) == 'w')
          {
            if(inCheckBy(kpos, 'b', gs))
              return GAME_OVER_BLACK_WINS;
            return GAME_OVER_STALEMATE;
          }
        else
          {
            if(inCheckBy(kpos, 'w', gs))
              return GAME_OVER_WHITE_WINS;
            return GAME_OVER_STALEMATE;
          }
      }
    else if(wMatNonK == 0 && bMatNonK == 0)                         //  Game is over if only Kings remain.
      return GAME_OVER_STALEMATE;
    else if(gs->moveCtr == 50)                                      //  Game is over if moveCtr reaches 50.
      return GAME_OVER_STALEMATE;

    return GAME_ONGOING;
  }

bool terminal(GameState* gs)
  {
    unsigned char win;

    win = isWin(gs);

    return (win != GAME_ONGOING);
  }

/**************************************************************************************************
 Identities and Tests  */

/*  Is the given index i vacant? */
bool isEmpty(unsigned char i, GameState* gs)
  {
    return (gs->board[i] == 'e');
  }

/*  Is the given index i occupied by a Black piece? */
bool isBlack(unsigned char i, GameState* gs)
  {
    return (gs->board[i] >= 'a' && gs->board[i] <= 'z' && gs->board[i] != 'e');
  }

/*  Is the given index i occupied by a White piece? */
bool isWhite(unsigned char i, GameState* gs)
  {
    return (gs->board[i] >= 'A' && gs->board[i] <= 'Z');
  }

/*  Is index i the same as index j
    in terms of both being White or both being Black or both being Empty? */
bool sameSide(unsigned char i, unsigned char j, GameState* gs)
  {
    return ((isWhite(i, gs) && isWhite(j, gs)) || (isBlack(i, gs) && isBlack(j, gs)));
  }

/*  More specific than same(), this function asks,
    "Are i and j on opposite teams?" */
bool opposed(unsigned char i, unsigned char j, GameState* gs)
  {
    return ((isWhite(i, gs) && isBlack(j, gs)) || (isBlack(i, gs) && isWhite(j, gs)));
  }

/* Return a character indicating which team 'index' belongs to. */
char getTeam(unsigned char index, GameState* gs)
  {
    if(isWhite(index, gs))
      return 'w';
    if(isBlack(index, gs))
      return 'b';
    return 'e';
  }

/*  Is the given index i occupied by a Pawn? */
bool isPawn(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'P' || gs->board[i] == 'p');
    return false;
  }

/*  Is the given index i occupied by a Knight? */
bool isKnight(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'N' || gs->board[i] == 'n');
    return false;
  }

/*  Is the given index i occupied by a Bishop? */
bool isBishop(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'B' || gs->board[i] == 'b');
    return false;
  }

/*  Is the given index i occupied by a Rook? */
bool isRook(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'R' || gs->board[i] == 'r');
    return false;
  }

/*  Is the given index i occupied by a Queen? */
bool isQueen(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'Q' || gs->board[i] == 'q');
    return false;
  }

/*  Is the given index i occupied by an King? */
bool isKing(unsigned char i, GameState* gs)
  {
    if(i < _NONE)
      return (gs->board[i] == 'K' || gs->board[i] == 'k');
    return false;
  }

bool whiteKingsidePrivilege(GameState* gs)
  {
    return gs->whiteKingsidePrivilege;
  }

bool whiteQueensidePrivilege(GameState* gs)
  {
    return gs->whiteQueensidePrivilege;
  }

bool blackKingsidePrivilege(GameState* gs)
  {
    return gs->blackKingsidePrivilege;
  }

bool blackQueensidePrivilege(GameState* gs)
  {
    return gs->blackQueensidePrivilege;
  }

bool whiteCastled(GameState* gs)
  {
    return gs->whiteCastled;
  }

bool blackCastled(GameState* gs)
  {
    return gs->blackCastled;
  }

/* Does the given Move describe a castling by either side, given the GameState 'gs'? */
bool isCastle(Move* move, GameState* gs)
  {
    return (isWhiteKingside(move, gs) || isBlackKingside(move, gs) || isWhiteQueenside(move, gs) || isBlackQueenside(move, gs));
  }

/* Does the given Move describe a kingside castling by white, given the GameState 'gs'? */
bool isWhiteKingside(Move* move, GameState* gs)
  {
    return (isWhite(move->from, gs) && isKing(move->from, gs) && move->from == 4 && move->to == 6 && gs->whiteKingsidePrivilege);
  }

/* Does the given Move describe a queenside castling by white, given the GameState 'gs'? */
bool isWhiteQueenside(Move* move, GameState* gs)
  {
    return (isWhite(move->from, gs) && isKing(move->from, gs) && move->from == 4 && move->to == 2 && gs->whiteQueensidePrivilege);
  }

/* Does the given Move describe a kingside castling by black, given the GameState 'gs'? */
bool isBlackKingside(Move* move, GameState* gs)
  {
    return (isBlack(move->from, gs) && isKing(move->from, gs) && move->from == 60 && move->to == 62 && gs->blackKingsidePrivilege);
  }

/* Does the given Move describe a queenside castling by black, given the GameState 'gs'? */
bool isBlackQueenside(Move* move, GameState* gs)
  {
    return (isBlack(move->from, gs) && isKing(move->from, gs) && move->from == 60 && move->to == 58 && gs->blackQueensidePrivilege);
  }

/**************************************************************************************************
 Set builders
   index:  starting position
   Flags:  w = pass through White
           W = stop and include White (as one would when attacking White)
           b = pass through Black
           B = stop and include Black (as one would when attacking Black)
   buffer: provided by the parent function, the set built ends up here, with length returned */

/* Build list of upward squares (e.g. 0, 8, 16, 24, ... ). */
unsigned char uSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = u(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = u(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = u(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = u(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of downward squares (e.g. 56, 48, 40, 32, ... ). */
unsigned char dSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = d(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = d(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = d(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = d(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of leftward squares (e.g. 39, 38, 37, 36, ... ). */
unsigned char lSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = l(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = l(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = l(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = l(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of rightward squares (e.g. 32, 33, 34, 35, ... ). */
unsigned char rSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = r(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = r(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = r(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = r(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of up-leftward squares (e.g. 7, 14, 21, 28, ... ). */
unsigned char ulSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = ul(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = ul(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = ul(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = ul(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of up-rightward squares (e.g. 0, 9, 18, 27, ... ). */
unsigned char urSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = ur(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = ur(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = ur(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = ur(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of down-rightward squares (e.g. 56, 49, 42, 35, ... ). */
unsigned char drSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = dr(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = dr(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = dr(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = dr(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/* Build list of down-leftward squares (e.g. 63, 54, 45, 36, ... ). */
unsigned char dlSet(unsigned char index, char* fstr, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char dst = dl(index);

    bool w = false;                                                 //  Scan flag-string.
    bool W = false;
    bool b = false;
    bool B = false;
    unsigned char i = 0;
    if((unsigned char)strlen(fstr) > 0)
      {
        while(fstr[i] != '\0')
          {
            if(fstr[i] == 'w')
              w = true;
            else if(fstr[i] == 'W')
              W = true;
            else if(fstr[i] == 'b')
              b = true;
            else if(fstr[i] == 'B')
              B = true;
            i++;
          }
      }

    while(dst < _NONE)
      {
        if(isEmpty(dst, gs))
          {
            buffer[len] = dst;
            len++;
            dst = dl(dst);
          }
        else if(isWhite(dst, gs))
          {
            if(w)                                                   //  Pass through White.
              {
                buffer[len] = dst;
                len++;
                dst = dl(dst);
              }
            else if(W)                                              //  Stop and include White.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
        else if(isBlack(dst, gs))
          {
            if(b)                                                   //  Pass through Black.
              {
                buffer[len] = dst;
                len++;
                dst = dl(dst);
              }
            else if(B)                                              //  Stop and include Black.
              {
                buffer[len] = dst;
                len++;
                break;
              }
            else
              break;
          }
      }

    return len;
  }

/**************************************************************************************************
 Board logic  */

/*  Return the index UP from the given i. */
unsigned char u(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i + 8) == row(i) + 1)
          return i + 8;
      }
    return _NONE;
  }

/*  Return the index DOWN from the given i. */
unsigned char d(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i - 8) == row(i) - 1 && row(i) != _NONE)
          return i - 8;
      }
    return _NONE;
  }

/*  Return the index LEFT from the given i. */
unsigned char l(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i - 1) == row(i))
          return i - 1;
      }
    return _NONE;
  }

/*  Return the index RIGHT from the given i. */
unsigned char r(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i + 1) == row(i))
          return i + 1;
      }
    return _NONE;
  }

/*  Return the index UP-LEFT from the given i. */
unsigned char ul(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i + 7) == row(i) + 1)
          return i + 7;
      }
    return _NONE;
  }

/*  Return the index UP-RIGHT from the given i. */
unsigned char ur(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i + 9) == row(i) + 1)
          return i + 9;
      }
    return _NONE;
  }

/*  Return the index DOWN-LEFT from the given i. */
unsigned char dl(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i - 9) == row(i) - 1 && row(i) != _NONE)
          return i - 9;
      }
    return _NONE;
  }

/*  Return the index DOWN-RIGHT from the given i. */
unsigned char dr(unsigned char i)
  {
    if(i < _NONE)
      {
        if(row(i - 7) == row(i) - 1 && row(i) != _NONE)
          return i - 7;
      }
    return _NONE;
  }

/*  Compute the COLUMN in which given index is included. */
unsigned char col(unsigned char i)
  {
    if(i < _NONE)
      return i & 7;                                                 //  i & 7 == i % 8 because 8 is a power of 2.
    return _NONE;
  }

/*  Compute the ROW in which given index is included. */
unsigned char row(unsigned char i)
  {
    if(i < _NONE)
      return (i - (i & 7)) / 8;                                     //  i & 7 == i % 8 because 8 is a power of 2.
    return _NONE;
  }

#endif
