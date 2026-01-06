#ifndef __PHILADELPHIA_H
#define __PHILADELPHIA_H

#include "gamestate.h"
                                                                    //  Opening game:
                                                                    //  Weights determined by Linear Regression.
                                                                    //  python3 linear_regression.py -iter 10 -model linear -scale 0.01
#define W0_MATERIAL                                        0.00034016086
#define W0_MOBILITY                                        0.017304078
#define W0_ATTACKS                                         0.004035855
#define W0_COVERAGE                                        0.0001
#define W0_PAWNSTRUCTURE                                   0.019106872
#define W0_DEVELOPMENT                                     0.015437191
#define W0_PIECEEVAL                                       0.005476671
#define W0_CENTERCONTROL                                   0.0050614076
#define W0_VULNERABILITY                                   0.0001
#define W0_TRAPPED                                         0.0001
#define W0_PINS                                            0.0014096461
                                                                    //  Middle game:
                                                                    //  Weights determined by Linear Regression.
                                                                    //  python3 linear_regression.py -iter 10 -model linear -scale 0.01
#define W1_MATERIAL                                        0.0001
#define W1_MOBILITY                                        0.18922871
#define W1_ATTACKS                                         0.008072786
#define W1_COVERAGE                                        0.023730814
#define W1_PAWNSTRUCTURE                                   0.21443987
#define W1_DEVELOPMENT                                     0.01667429
#define W1_PIECEEVAL                                       0.041631334
#define W1_CENTERCONTROL                                   0.024489462
#define W1_VULNERABILITY                                   0.0001
#define W1_TRAPPED                                         0.0001
#define W1_PINS                                            0.102702424
                                                                    //  Endgame:
                                                                    //  Weights determined by Linear Regression.
                                                                    //  python3 linear_regression.py -iter 10 -model linear -scale 0.01
#define W2_MATERIAL                                        0.0032175416
#define W2_MOBILITY                                        0.0001
#define W2_ATTACKS                                         0.05343467
#define W2_COVERAGE                                        0.16151458
#define W2_PAWNSTRUCTURE                                   0.24528006
#define W2_DEVELOPMENT                                     0.45203942
#define W2_PIECEEVAL                                       0.01284852
#define W2_CENTERCONTROL                                   0.110759884
#define W2_VULNERABILITY                                   0.006354887
#define W2_TRAPPED                                         0.0001
#define W2_PINS                                            0.0001

#define PAWN                                             100.0
#define KNIGHT                                           300.0
#define BISHOP                                           330.0
#define ROOK                                             500.0
#define QUEEN                                            900.0

#define OPENING_GAME                                       0
#define MIDDLE_GAME                                        1
#define END_GAME                                           2

#define ISOLATED_PAWN_PENALTY                             -4.0
#define DOUBLED_PAWN_PENALTY                              -1.0      /* Will be counted twice per occurrence, so effectively = -2 */
#define BACKWARD_PAWN_PENALTY                             -2.0
#define CONNECTED_PAWN_BONUS                               1.0
#define PASSED_PAWN_BONUS                                  3.0
#define CANDIDATE_PAWN_BONUS                               2.0
#define UNSTOPPABLE_PAWN_BONUS                             6.0
#define SENTRY_PAWN_BONUS                                  1.0

#define PAWN_OCCUPY_CENTER_BONUS                           1.0
#define PAWN_ATTACK_CENTER_BONUS                           2.0
#define PAWN_BLOCKADE_BONUS                                1.0
#define PAWN_STUCK_ON_INITIAL_PENALTY                     -4.0

#define KNIGHT_PAWN_DIMINISH                              -5.0
#define KNIGHT_OUTPOST_BONUS                               7.0
#define KNIGHT_TRAPPED_PENALTY                           -15.0
#define KNIGHT_BLOCKS_C_PAWN_PENALTY                      -4.0
#define KNIGHT_MOBILITY_PAWN_CONTROL_PENALTY              -2.0
#define KNIGHT_BISHOP_DEFENSE_BONUS                        4.0
#define BISHOP_PAIR_BONUS                                 15.0      /* Counted twice, once per bishop */
#define BISHOP_BAD_PENALTY                                -9.0
#define BISHOP_COLOR_WEAK_PENALTY                         -2.0      /* Multiplied by severity of color weakness */
#define BISHOP_FIANCHETTO_BONUS                           10.0
#define BISHOP_FIANCHETTO_AGAINST_SAME_COLOR_BISHOP_BONUS  3.0
#define BISHOP_UNDEVELOPED_PENALTY                         4.0      /* Positive because it is designed to UNDO a penalty */
#define BISHOP_PAWN_TRAPPED_CORNER_PENALTY               -10.0
#define BISHOP_KNIGHT_DEFENSE_BONUS                        4.0

#define UNDEFENDED_MINOR_PIECE_PENALTY                    -5.0

#define ROOK_PAWN_INCREASE                                 3.0
#define ROOK_OPEN_FILE_BONUS                               7.0
#define ROOK_7TH_RANK_BONUS                                6.0
#define ROOK_8TH_RANK_BONUS                                3.0
#define ROOK_TARRASCH_BONUS                                6.0
#define ROOK_BEHIND_UNCASTLED_KING_PENALTY                -4.0
#define ROOK_ENEMY_QUEEN_FILE_BONUS                        5.0
#define ROOK_MUTUAL_DEFENSE_BONUS                          8.0

#define QUEEN_OVERLAPPING_ATTACK_BONUS                     4.0
#define QUEEN_KING_XRAY_BONUS                              1.0

#define KING_CASTLED_BONUS                                10.0
#define KING_LOST_CASTLE_RIGHTS_PENALTY                  -10.0
#define KING_PAWN_SHIELD_IMMEDIATE                         2.0
#define KING_PAWN_SHIELD_ONE_DISTANT                       1.0
#define KING_STORMING_PAWN_PENALTY                         0.5
#define KING_TROPISM_PAWN_WEIGHT                           0.0
#define KING_TROPISM_KNIGHT_WEIGHT                         0.5
#define KING_TROPISM_BISHOP_WEIGHT                         1.0
#define KING_TROPISM_ROOK_WEIGHT                           1.0
#define KING_TROPISM_QUEEN_WEIGHT                          2.0
#define KING_TROPISM_KING_WEIGHT                           0.0
#define KING_ZONE_ATTACKED_BY_PAWN                         2.0
#define KING_ZONE_ATTACKED_BY_KNIGHT                       1.5
#define KING_ZONE_ATTACKED_BY_BISHOP                       1.5
#define KING_ZONE_ATTACKED_BY_ROOK                         3.5
#define KING_ZONE_ATTACKED_BY_QUEEN                        5.0
#define KING_ZONE_ATTACKED_BY_KING                         2.5
#define KING_ENDGAME_CENTRALITY                            2.0

#define VULNERABLE_DISCOUNT                                0.8      /* Coefficient for piece value for pieces left en prise.
                                                                       (Effectively, though not yet actually, lost.) */
#define ASYMMETRICAL_ATTACK_BONUS                          4.0      /* Bonus for attacking the opponent in a way that they cannot attack back */

#define ABSOLUTE_PIN_BONUS                                 6.0      /* Bonuses for various types of pins */
#define PARTIAL_PIN_BONUS                                  2.0
#define RELATIVE_PIN_BONUS                                 4.0
#define PARTIAL_RELATIVE_PIN_BONUS                         2.0
#define TRAPPED_PIECE_PENALTY                             -5.0      /* Penalty per trapped piece */
#define TRAPPED_PIECE_RATIO                                0.5

/**************************************************************************************************
 Typedefs  */


/**************************************************************************************************
 Prototypes  */

unsigned int getMovesForTeam(bool, GameState*, Move*);
unsigned int getPawnAttacksTeam(bool, GameState*, Move*);
unsigned int getPawnTargetsTeam(bool, GameState*, Move*);

float score(GameState*, bool);

float material(unsigned char*, unsigned char, GameState*);
float materialLookup(unsigned char, GameState*);

float mobility(Move*, unsigned int, GameState*);

float attacks(Move*, unsigned int, Move*, unsigned int, GameState*);

float coverage(Move*, unsigned int, GameState*);
unsigned int getCoverage(bool, GameState*, Move*);
unsigned int getCoverageIndex(unsigned char, GameState*, Move*);
unsigned int getPawnCoverage(unsigned char, GameState*, Move*);
unsigned int getKnightCoverage(unsigned char, GameState*, Move*);
unsigned int getBishopCoverage(unsigned char, GameState*, Move*);
unsigned int getRookCoverage(unsigned char, GameState*, Move*);
unsigned int getQueenCoverage(unsigned char, GameState*, Move*);
unsigned int getKingCoverage(unsigned char, GameState*, Move*);

unsigned int getScope(bool, GameState*, Move*);
unsigned int getScopeIndex(unsigned char, GameState*, Move*);
unsigned int getBishopScope(unsigned char, GameState*, Move*);
unsigned int getRookScope(unsigned char, GameState*, Move*);
unsigned int getQueenScope(unsigned char, GameState*, Move*);

unsigned int getXRay(bool, GameState*, Move*);
unsigned int getXRayIndex(unsigned char, GameState*, Move*);
unsigned int getBishopXRay(unsigned char, GameState*, Move*);
unsigned int getRookXRay(unsigned char, GameState*, Move*);
unsigned int getQueenXRay(unsigned char, GameState*, Move*);

float pawnstructure(unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);
bool isIsolatedPawn(unsigned char, unsigned char, GameState*);
bool isBackwardPawn(unsigned char, unsigned char, Move*, unsigned char, GameState*);
bool isConnectedPawn(unsigned char, unsigned char, GameState*);
bool isPassedPawn(unsigned char, unsigned char, GameState*);
bool isCandidatePassedPawn(unsigned char, unsigned char, GameState*);
bool isUnstoppablePawn(unsigned char, unsigned char, Move*, unsigned char, Move*, unsigned char, GameState*);
bool isSentryPawn(unsigned char, GameState*);

float development(bool, GameState* gs);

float pieceeval(unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int,
                unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, GameState*);
float pawnEval(unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, GameState*);
float pawnBlockade(unsigned char, GameState*);
float pawnBlocked_DE(unsigned char, Move*, unsigned int, GameState*);
float knightEval(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);
float knightDecreasePawnVal(GameState*);
float knightOutpost(unsigned char, Move*, unsigned int, Move*, unsigned int, GameState*);
float knightTrapped(unsigned char);
float knightBlockingCPawn(unsigned char, GameState*);
float knightMobility(unsigned char, Move*, unsigned int, Move*, unsigned int);
float knightBishopDefense(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float knightUndefended(unsigned char, Move*, unsigned int);
float bishopEval(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float bishopPair(unsigned char, unsigned char*, unsigned char, GameState*);
float badBishop(unsigned char, Move*, unsigned int, GameState*);
float bishopColorWeakness(unsigned char*, unsigned char, GameState*);
unsigned char darkSquares(unsigned char*);
unsigned char lightSquares(unsigned char*);
float Fianchetto(unsigned char, unsigned char*, unsigned char, GameState*);
float bishopReturn(unsigned char, GameState*);
float bishopTrapped(unsigned char, Move*, unsigned int);
float bishopKnightDefense(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float bishopUndefended(unsigned char, Move*, unsigned int);
float rookEval(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, unsigned char*, unsigned char, GameState*);
float rookIncreasePawnVal(GameState*);
float rookOpenFile(unsigned char, GameState*);
float rookHighRank(unsigned char, GameState*);
float Tarrasch(unsigned char index, unsigned char*, unsigned char, unsigned char*, unsigned char, GameState*);
float rookTrappedUncastled(unsigned char, GameState*);
float rookEnemyQueen(unsigned char, GameState*);
float rookMutualDefense(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float queenEval(unsigned char index, unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, GameState*);
float queenEarlyDevelopment(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float queenKingTropism(unsigned char, Move*, unsigned int, GameState*);
unsigned char bfs(unsigned char*, unsigned char, unsigned char, unsigned char);
float kingEval(unsigned char, unsigned char*, unsigned char, Move*, unsigned int, GameState*);
float kingCastlingRights(unsigned char, GameState*);
float kingPawnShield(unsigned char, GameState*);
float kingPawnStorm(unsigned char, unsigned char*, unsigned char, GameState*);
float kingTropism(unsigned char, unsigned char*, unsigned char, GameState*);
float kingZoneAttacks(unsigned char, Move*, unsigned int, GameState*);

float centercontrol(bool, Move*, unsigned int, Move*, unsigned int);
void buildCenterMap(bool, unsigned char*);

float vulnerability(Move*, unsigned int, GameState*);

float trapped(Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);

float pins(unsigned char*, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);
float awardPinPatternBishopRook(unsigned char, unsigned char, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);
float awardPinPatternQueen(unsigned char, unsigned char, unsigned char, Move*, unsigned int, Move*, unsigned int, Move*, unsigned int, GameState*);

bool isOpenFile(unsigned char, GameState*);
bool isSemiOpenFile(unsigned char, GameState*);

unsigned char getWhite(GameState*, unsigned char*);
unsigned char getBlack(GameState*, unsigned char*);
unsigned char getPawns(bool, GameState*, unsigned char*);
unsigned char getCol(unsigned char, unsigned char*);
unsigned char getRow(unsigned char, unsigned char*);

unsigned char phase(GameState*);
bool quiet(GameState*);

/**************************************************************************************************
 Globals  */


/**************************************************************************************************
 Team Moves  */

/* THIS FUNCTION FILTERS FOR CHECK!!
   Differs from gamestate.h getMoves() because you may specify a team not necessarily now to move. */
unsigned int getMovesForTeam(bool white, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    Move potentialmoves[_NONE];                                     //  Assumes generous upper bound of 64 moves per piece.
    unsigned int potentialmovesCtr = 0;
    unsigned int i;
    unsigned char index;

    for(index = 0; index < _NONE; index++)
      {
        if((white && isWhite(index, gs)) || (!white && isBlack(index, gs)))
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
   Collect actual pawn attacks available to indicated team. */
unsigned int getPawnAttacksTeam(bool white, GameState* gs, Move* buffer)
  {
    Move potentialmoves[2];                                         //  At most, a single pawn could attack two squares.
    unsigned int potentialmovesCtr = 0;
    unsigned int movesCtr = 0, i;
    unsigned char index, j;
    GameState tmp;

    for(index = 0; index < _NONE; index++)
      {
        if(isPawn(index, gs) && ((white && isWhite(index, gs)) || (!white && isBlack(index, gs))))
          {
                                                                    //  getPawnAttackable() only collects diagonals that do not fall off the board.
            potentialmovesCtr = getPawnAttackable(index, gs, potentialmoves);

            if(white)                                               //  Test white pawn-attackables.
              {
                for(i = 0; i < potentialmovesCtr; i++)              //  For every move, make that move, then test the resultant board.
                  {
                    if(isBlack(potentialmoves[i].to, gs))           //  Is this actually a white-pawn attack?
                      {
                        copyGameState(gs, &tmp);                    //  Copy the board.
                        makeMove(potentialmoves + i, &tmp);         //  Apply the candidate move.
                        j = 0;                                      //  Locate the white king on the new board.
                        while(j < _NONE && tmp.board[j] != 'K')
                          j++;
                        if(!inCheckBy(j, 'b', &tmp))                //  If it does not leave the king in check, then the pawn-attack is allowed.
                          {
                            buffer[movesCtr].from = potentialmoves[i].from;
                            buffer[movesCtr].to = potentialmoves[i].to;
                            buffer[movesCtr].promo = potentialmoves[i].promo;
                            movesCtr++;
                          }
                      }
                  }
              }
            else                                                    //  Test black pawn-attackables.
              {
                for(i = 0; i < potentialmovesCtr; i++)              //  For every move, make that move, then test the resultant board.
                  {
                    if(isWhite(potentialmoves[i].to, gs))           // Is this actually a black-pawn attack?
                      {
                        copyGameState(gs, &tmp);                    //  Copy the board.
                        makeMove(potentialmoves + i, &tmp);         //  Apply the candidate move.
                        j = 0;                                      //  Locate the black king on the new board.
                        while(j < _NONE && tmp.board[j] != 'k')
                          j++;
                        if(!inCheckBy(j, 'w', &tmp))                //  If it does not leave the king in check, then the pawn-attack is allowed.
                          {
                            buffer[movesCtr].from = potentialmoves[i].from;
                            buffer[movesCtr].to = potentialmoves[i].to;
                            buffer[movesCtr].promo = potentialmoves[i].promo;
                            movesCtr++;
                          }
                      }
                  }
              }
          }
      }

    return movesCtr;
  }

/* THIS FUNCTION FILTERS FOR CHECK!!
   Collect squares theoretically attackable by pawns belonging to the indicated team.
   These ay include actual available attacks. */
unsigned int getPawnTargetsTeam(bool white, GameState* gs, Move* buffer)
  {
    Move potentialmoves[2];                                         //  At most, a single pawn could attack two squares.
    unsigned int potentialmovesCtr = 0;
    unsigned int movesCtr = 0, i;
    unsigned char index, j;
    GameState tmp;

    for(index = 0; index < _NONE; index++)
      {
        if(isPawn(index, gs) && ((white && isWhite(index, gs)) || (!white && isBlack(index, gs))))
          {
                                                                    //  getPawnAttackable() only collects diagonals that do not fall off the board.
            potentialmovesCtr = getPawnAttackable(index, gs, potentialmoves);

            if(white)                                               //  Test white pawn-attackables.
              {
                for(i = 0; i < potentialmovesCtr; i++)              //  For every move, make that move, then test the resultant board.
                  {
                    copyGameState(gs, &tmp);                        //  Copy the board.
                    makeMove(potentialmoves + i, &tmp);             //  Apply the candidate move.
                    j = 0;                                          //  Locate the white king on the new board.
                    while(j < _NONE && tmp.board[j] != 'K')
                      j++;
                    if(!inCheckBy(j, 'b', &tmp))                    //  If it does not leave the king in check, then the pawn-attack is allowed.
                      {
                        buffer[movesCtr].from = potentialmoves[i].from;
                        buffer[movesCtr].to = potentialmoves[i].to;
                        buffer[movesCtr].promo = potentialmoves[i].promo;
                        movesCtr++;
                      }
                  }
              }
            else                                                    //  Test black pawn-attackables.
              {
                for(i = 0; i < potentialmovesCtr; i++)              //  For every move, make that move, then test the resultant board.
                  {
                    copyGameState(gs, &tmp);                        //  Copy the board.
                    makeMove(potentialmoves + i, &tmp);             //  Apply the candidate move.
                    j = 0;                                          //  Locate the black king on the new board.
                    while(j < _NONE && tmp.board[j] != 'k')
                      j++;
                    if(!inCheckBy(j, 'w', &tmp))                    //  If it does not leave the king in check, then the pawn-attack is allowed.
                      {
                        buffer[movesCtr].from = potentialmoves[i].from;
                        buffer[movesCtr].to = potentialmoves[i].to;
                        buffer[movesCtr].promo = potentialmoves[i].promo;
                        movesCtr++;
                      }
                  }
              }
          }
      }

    return movesCtr;
  }

/**************************************************************************************************
 Evaluation  */

/* Meaning, if evaluateForWhite == true, then advantage for White is understood to be positive.
   If evaluateForWhite == false, then advantage for Black is understood to be positive. */
float score(GameState* gs, bool evaluateForWhite)
  {
    float h = 0.0;
    unsigned char win;
    unsigned char gamePhase;
    unsigned int i;

    unsigned char whiteMaterial[16];                                //  Indices of all white material.
    unsigned char whiteMaterialLength;
    unsigned char blackMaterial[16];                                //  Indices of all black material.
    unsigned char blackMaterialLength;

    Move whiteMoves[_MAX_MOVES];
    unsigned int whiteMovesLength;
    Move blackMoves[_MAX_MOVES];
    unsigned int blackMovesLength;

    Move whitePawnAttacks[_MAX_MOVES];                              //  Actual pawn attacks only, not pawn-attackable squares.
    unsigned int whitePawnAttacksLength;
    Move blackPawnAttacks[_MAX_MOVES];
    unsigned int blackPawnAttacksLength;

    Move whitePawnTargets[_MAX_MOVES];                              //  Pawn-attackable squares, not necessarily actual attacks.
    unsigned int whitePawnTargetsLength;
    Move blackPawnTargets[_MAX_MOVES];
    unsigned int blackPawnTargetsLength;

    Move whiteCoverage[_MAX_MOVES];                                 //  Ally-occupied squares that are theoretically attackable.
    unsigned int whiteCoverageLength;
    Move blackCoverage[_MAX_MOVES];
    unsigned int blackCoverageLength;

    Move whitePawnCoverage[_MAX_MOVES];                             //  Coverage, but by pawns only.
    unsigned int whitePawnCoverageLength;
    Move blackPawnCoverage[_MAX_MOVES];
    unsigned int blackPawnCoverageLength;

    Move whiteScope[_MAX_MOVES];                                    //  Scope is all squares theoretically reachable: empty, ally-occupied, enemy-occupied.
    unsigned int whiteScopeLength;
    Move blackScope[_MAX_MOVES];
    unsigned int blackScopeLength;

    Move whiteXRay[_MAX_MOVES];                                     //  X-Ray attacks are enemy-occupied squares if we could pass through allies (and empties).
    unsigned int whiteXRayLength;
    Move blackXRay[_MAX_MOVES];
    unsigned int blackXRayLength;

    win = isWin(gs);
    if(win != GAME_ONGOING)
      {
        if((win == GAME_OVER_BLACK_WINS && !evaluateForWhite) || (win == GAME_OVER_WHITE_WINS && evaluateForWhite))
          return INFINITY;
        else if(win == GAME_OVER_STALEMATE)
          return 0.0;
        else
          return -INFINITY;
      }

    gamePhase = phase(gs);

    //////////////////////////////////////////////////////////////////  Compute the following only ONCE
    whiteMaterialLength = getWhite(gs, whiteMaterial);              //  unsigned chars
    blackMaterialLength = getBlack(gs, blackMaterial);              //  unsigned chars

    whiteMovesLength = getMovesForTeam(true, gs, whiteMoves);       //  Moves
    blackMovesLength = getMovesForTeam(false, gs, blackMoves);      //  Moves
                                                                    //  Pawn Attack Moves
    whitePawnAttacksLength = getPawnAttacksTeam(true, gs, whitePawnAttacks);
                                                                    //  Pawn Attack Moves
    blackPawnAttacksLength = getPawnAttacksTeam(false, gs, blackPawnAttacks);
                                                                    //  Pawn Target Moves
    whitePawnTargetsLength = getPawnTargetsTeam(true, gs, whitePawnTargets);
                                                                    //  Pawn Target Moves
    blackPawnTargetsLength = getPawnTargetsTeam(false, gs, blackPawnTargets);

    whiteCoverageLength = getCoverage(true, gs, whiteCoverage);     //  Coverage Moves
    blackCoverageLength = getCoverage(false, gs, blackCoverage);    //  Coverage Moves

    whitePawnCoverageLength = 0;                                    //  Coverage only by pawns
    for(i = 0; i < whiteCoverageLength; i++)                        //  Count up pawn coverage.
      {
        if(isPawn(whiteCoverage[i].from, gs))
          {
            whitePawnCoverage[whitePawnCoverageLength].from = whiteCoverage[i].from;
            whitePawnCoverage[whitePawnCoverageLength].to = whiteCoverage[i].to;
            whitePawnCoverage[whitePawnCoverageLength].promo = whiteCoverage[i].promo;
            whitePawnCoverageLength++;
          }
      }
    blackPawnCoverageLength = 0;
    for(i = 0; i < blackCoverageLength; i++)
      {
        if(isPawn(blackCoverage[i].from, gs))
          {
            blackPawnCoverage[whitePawnCoverageLength].from = blackCoverage[i].from;
            blackPawnCoverage[whitePawnCoverageLength].to = blackCoverage[i].to;
            blackPawnCoverage[whitePawnCoverageLength].promo = blackCoverage[i].promo;
            blackPawnCoverageLength++;
          }
      }

    whiteScopeLength = getScope(true, gs, whiteScope);              //  Scope
    blackScopeLength = getScope(false, gs, blackScope);

    whiteXRayLength = getXRay(true, gs, whiteXRay);                 //  X-Ray attacks
    blackXRayLength = getXRay(false, gs, blackXRay);

    if(evaluateForWhite)  ////////////////////////////////////////////  WHITE
      {
        switch(gamePhase)
          {
            case OPENING_GAME: h += material(whiteMaterial, whiteMaterialLength, gs) * W0_MATERIAL;
                               h -= material(blackMaterial, blackMaterialLength, gs) * W0_MATERIAL;

                               h += mobility(whiteMoves, whiteMovesLength, gs) * W0_MOBILITY;
                               h -= mobility(blackMoves, blackMovesLength, gs) * W0_MOBILITY;

                               h += attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W0_ATTACKS;
                               h -= attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W0_ATTACKS;

                               h += coverage(whiteCoverage, whiteCoverageLength, gs) * W0_COVERAGE;
                               h -= coverage(blackCoverage, blackCoverageLength, gs) * W0_COVERAGE;

                               h += pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W0_PAWNSTRUCTURE;
                               h -= pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W0_PAWNSTRUCTURE;

                               h += development(true, gs) * W0_DEVELOPMENT;
                               h -= development(false, gs) * W0_DEVELOPMENT;

                               h += pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                              blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W0_PIECEEVAL;
                               h -= pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W0_PIECEEVAL;

                               h += centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W0_CENTERCONTROL;
                               h -= centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W0_CENTERCONTROL;

                               h += vulnerability(whiteMoves, whiteMovesLength, gs) * W0_VULNERABILITY;
                               h -= vulnerability(blackMoves, blackMovesLength, gs) * W0_VULNERABILITY;

                               h += trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W0_TRAPPED;
                               h -= trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W0_TRAPPED;

                               h += pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W0_PINS;
                               h -= pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W0_PINS;
                               break;
            case MIDDLE_GAME:  h += material(whiteMaterial, whiteMaterialLength, gs) * W1_MATERIAL;
                               h -= material(blackMaterial, blackMaterialLength, gs) * W1_MATERIAL;

                               h += mobility(whiteMoves, whiteMovesLength, gs) * W1_MOBILITY;
                               h -= mobility(blackMoves, blackMovesLength, gs) * W1_MOBILITY;

                               h += attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W1_ATTACKS;
                               h -= attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W1_ATTACKS;

                               h += coverage(whiteCoverage, whiteCoverageLength, gs) * W1_COVERAGE;
                               h -= coverage(blackCoverage, blackCoverageLength, gs) * W1_COVERAGE;

                               h += pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W1_PAWNSTRUCTURE;
                               h -= pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W1_PAWNSTRUCTURE;

                               h += development(true, gs) * W1_DEVELOPMENT;
                               h -= development(false, gs) * W1_DEVELOPMENT;

                               h += pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                              blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W1_PIECEEVAL;
                               h -= pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W1_PIECEEVAL;

                               h += centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W1_CENTERCONTROL;
                               h -= centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W1_CENTERCONTROL;

                               h += vulnerability(whiteMoves, whiteMovesLength, gs) * W1_VULNERABILITY;
                               h -= vulnerability(blackMoves, blackMovesLength, gs) * W1_VULNERABILITY;

                               h += trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W1_TRAPPED;
                               h -= trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W1_TRAPPED;

                               h += pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W1_PINS;
                               h -= pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W1_PINS;
                               break;
            case END_GAME:     h += material(whiteMaterial, whiteMaterialLength, gs) * W2_MATERIAL;
                               h -= material(blackMaterial, blackMaterialLength, gs) * W2_MATERIAL;

                               h += mobility(whiteMoves, whiteMovesLength, gs) * W2_MOBILITY;
                               h -= mobility(blackMoves, blackMovesLength, gs) * W2_MOBILITY;

                               h += attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W2_ATTACKS;
                               h -= attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W2_ATTACKS;

                               h += coverage(whiteCoverage, whiteCoverageLength, gs) * W2_COVERAGE;
                               h -= coverage(blackCoverage, blackCoverageLength, gs) * W2_COVERAGE;

                               h += pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W2_PAWNSTRUCTURE;
                               h -= pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W2_PAWNSTRUCTURE;

                               h += development(true, gs) * W2_DEVELOPMENT;
                               h -= development(false, gs) * W2_DEVELOPMENT;

                               h += pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                              blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W2_PIECEEVAL;
                               h -= pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W2_PIECEEVAL;

                               h += centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W2_CENTERCONTROL;
                               h -= centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W2_CENTERCONTROL;

                               h += vulnerability(whiteMoves, whiteMovesLength, gs) * W2_VULNERABILITY;
                               h -= vulnerability(blackMoves, blackMovesLength, gs) * W2_VULNERABILITY;

                               h += trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W2_TRAPPED;
                               h -= trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W2_TRAPPED;

                               h += pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W2_PINS;
                               h -= pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W2_PINS;
                               break;
          }
      }
    else  ////////////////////////////////////////////////////////////  BLACK
      {
        switch(gamePhase)
          {
            case OPENING_GAME: h += material(blackMaterial, blackMaterialLength, gs) * W0_MATERIAL;
                               h -= material(whiteMaterial, whiteMaterialLength, gs) * W0_MATERIAL;

                               h += mobility(blackMoves, blackMovesLength, gs) * W0_MOBILITY;
                               h -= mobility(whiteMoves, whiteMovesLength, gs) * W0_MOBILITY;

                               h += attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W0_ATTACKS;
                               h -= attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W0_ATTACKS;

                               h += coverage(blackCoverage, blackCoverageLength, gs) * W0_COVERAGE;
                               h -= coverage(whiteCoverage, whiteCoverageLength, gs) * W0_COVERAGE;

                               h += pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W0_PAWNSTRUCTURE;
                               h -= pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W0_PAWNSTRUCTURE;

                               h += development(false, gs) * W0_DEVELOPMENT;
                               h -= development(true, gs) * W0_DEVELOPMENT;

                               h += pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W0_PIECEEVAL;
                               h -= pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                              blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W0_PIECEEVAL;

                               h += centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W0_CENTERCONTROL;
                               h -= centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W0_CENTERCONTROL;

                               h += vulnerability(blackMoves, blackMovesLength, gs) * W0_VULNERABILITY;
                               h -= vulnerability(whiteMoves, whiteMovesLength, gs) * W0_VULNERABILITY;

                               h += trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W0_TRAPPED;
                               h -= trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W0_TRAPPED;

                               h += pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W0_PINS;
                               h -= pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W0_PINS;
                               break;
            case MIDDLE_GAME:  h += material(blackMaterial, blackMaterialLength, gs) * W1_MATERIAL;
                               h -= material(whiteMaterial, whiteMaterialLength, gs) * W1_MATERIAL;

                               h += mobility(blackMoves, blackMovesLength, gs) * W1_MOBILITY;
                               h -= mobility(whiteMoves, whiteMovesLength, gs) * W1_MOBILITY;

                               h += attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W1_ATTACKS;
                               h -= attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W1_ATTACKS;

                               h += coverage(blackCoverage, blackCoverageLength, gs) * W1_COVERAGE;
                               h -= coverage(whiteCoverage, whiteCoverageLength, gs) * W1_COVERAGE;

                               h += pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W1_PAWNSTRUCTURE;
                               h -= pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W1_PAWNSTRUCTURE;

                               h += development(false, gs) * W1_DEVELOPMENT;
                               h -= development(true, gs) * W1_DEVELOPMENT;

                               h += pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W1_PIECEEVAL;
                               h -= pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                             blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W1_PIECEEVAL;

                               h += centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W1_CENTERCONTROL;
                               h -= centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W1_CENTERCONTROL;

                               h += vulnerability(blackMoves, blackMovesLength, gs) * W1_VULNERABILITY;
                               h -= vulnerability(whiteMoves, whiteMovesLength, gs) * W1_VULNERABILITY;

                               h += trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W1_TRAPPED;
                               h -= trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W1_TRAPPED;

                               h += pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W1_PINS;
                               h -= pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W1_PINS;
                               break;
            case END_GAME:     h += material(blackMaterial, blackMaterialLength, gs) * W2_MATERIAL;
                               h -= material(whiteMaterial, whiteMaterialLength, gs) * W2_MATERIAL;

                               h += mobility(blackMoves, blackMovesLength, gs) * W2_MOBILITY;
                               h -= mobility(whiteMoves, whiteMovesLength, gs) * W2_MOBILITY;

                               h += attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, gs) * W2_ATTACKS;
                               h -= attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, gs) * W2_ATTACKS;

                               h += coverage(blackCoverage, blackCoverageLength, gs) * W2_COVERAGE;
                               h -= coverage(whiteCoverage, whiteCoverageLength, gs) * W2_COVERAGE;

                               h += pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength,
                                                  whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W2_PAWNSTRUCTURE;
                               h -= pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength,
                                                  blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W2_PAWNSTRUCTURE;

                               h += development(false, gs) * W2_DEVELOPMENT;
                               h -= development(true, gs) * W2_DEVELOPMENT;

                               h += pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength,
                                              whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, gs) * W2_PIECEEVAL;
                               h -= pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength,
                                             blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, gs) * W2_PIECEEVAL;

                               h += centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) * W2_CENTERCONTROL;
                               h -= centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) * W2_CENTERCONTROL;

                               h += vulnerability(blackMoves, blackMovesLength, gs) * W2_VULNERABILITY;
                               h -= vulnerability(whiteMoves, whiteMovesLength, gs) * W2_VULNERABILITY;

                               h += trapped(blackMoves, blackMovesLength,
                                            whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, gs) * W2_TRAPPED;
                               h -= trapped(whiteMoves, whiteMovesLength,
                                            blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, gs) * W2_TRAPPED;

                               h += pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength,
                                         blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, gs) * W2_PINS;
                               h -= pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength,
                                         whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, gs) * W2_PINS;
                               break;
          }
      }

    return h;
  }

/**************************************************************************************************
 Material
   Use Reinfeld values to evaluate material advantage  */
float material(unsigned char* posTeam, unsigned char posLen, GameState* gs)
  {
    unsigned char i;
    float h = 0.0;

    for(i = 0; i < posLen; i++)
      h += materialLookup(posTeam[i], gs);

    return h;
  }

float materialLookup(unsigned char i, GameState* gs)
  {
    if(isPawn(i, gs))
      return PAWN;
    if(isKnight(i, gs))
      return KNIGHT;
    if(isBishop(i, gs))
      return BISHOP;
    if(isRook(i, gs))
      return ROOK;
    if(isQueen(i, gs))
      return QUEEN;
    return 0.0;
  }

/**************************************************************************************************
 Mobility
   Do not score moves which step into an attack exclude Queen moves (they skew the score)  */
float mobility(Move* posMoves, unsigned int posMovesLen, GameState* gs)
  {
    unsigned int i;
    float h = 0.0;

    for(i = 0; i < posMovesLen; i++)
      {
        if(!isQueen(posMoves[i].from, gs))
          h += 1.0;
      }

    return h;
  }

/**************************************************************************************************
 Attacks
   Reward attacks on opponent pieces which cannot threaten back
   (we do not want to encourage Mexican standoffs, say, between two bishops, staring at each other) */
float attacks(Move* posMoves, unsigned int posMovesLen, Move* negMoves, unsigned int negMovesLen, GameState* gs)
  {
    float h = 0.0;
    unsigned int i, j;

    for(i = 0; i < posMovesLen; i++)                                //  Add bonuses for positive team.
      {
        if(!isEmpty(posMoves[i].to, gs))                            //  If move [i] is an attack,
          {                                                         //  then look for a reciprocal attack among the opponent's moves.
            j = 0;
            while(j < negMovesLen && negMoves[j].to != posMoves[i].from)
              j++;
            if(j == negMovesLen)                                    //  If we've NOT found a reciprocal attack, then award a bonus.
              h += ASYMMETRICAL_ATTACK_BONUS;
          }
      }

    return h;
  }

/**************************************************************************************************
 Coverage: defined as ally-occupied squares that are (theoretically) attackable.
   Award points for general coverage. */
float coverage(Move* posCover, unsigned int posCoverLen, GameState* gs)
  {
    float h = 0.0;
    unsigned int i;
    float coveredVal;                                               //  Value of the piece covered by another.
    float avengerVal;                                               //  The one to capture back if you're captured.

    for(i = 0; i < posCoverLen; i++)
      {
        coveredVal = materialLookup(posCover[i].to, gs);
        avengerVal = materialLookup(posCover[i].from, gs);

        if(coveredVal < avengerVal)                                 //  e.g. queen covering pawn.
          h += 2.0;
        else if(coveredVal == avengerVal)                           //  Mutual defense.
          h += 3.0;
        else                                                        //  e.g. pawn covering queen (not very good).
          h += 1.0;
      }

    return h;
  }

/* THIS FUNCTION FILTERS FOR CHECK!!
   Return number of moves. Actual Move objects stored in given buffer.
   NOTICE that all these coverage functions test that the piece being covered is NOT the king.
   It makes no sense to cover the king, and not accouting for this will mess up the inCheckBy() test that filters moves. */
unsigned int getCoverage(bool white, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    Move potentialmoves[_NONE];                                     //  Assumes generous upper bound of 64 moves per piece.
    unsigned int potentialmovesCtr = 0;
    unsigned int i;
    unsigned char index;

    for(index = 0; index < _NONE; index++)
      {
        if((white && isWhite(index, gs)) || (!white && isBlack(index, gs)))
          {
            potentialmovesCtr = getCoverageIndex(index, gs, potentialmoves);
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
unsigned int getCoverageIndex(unsigned char index, GameState* gs, Move* buffer)
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
          potentialmovesCtr = getPawnCoverage(index, gs, potentialmoves);
        else if(isKnight(index, gs))
          potentialmovesCtr = getKnightCoverage(index, gs, potentialmoves);
        else if(isBishop(index, gs))
          potentialmovesCtr = getBishopCoverage(index, gs, potentialmoves);
        else if(isRook(index, gs))
          potentialmovesCtr = getRookCoverage(index, gs, potentialmoves);
        else if(isQueen(index, gs))
          potentialmovesCtr = getQueenCoverage(index, gs, potentialmoves);
        else
          potentialmovesCtr = getKingCoverage(index, gs, potentialmoves);

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

/* This function tests that the forward diagonals of "index" do not fall off the board
   AND that the forward diagonal is occupied by a friendly piece. */
unsigned int getPawnCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned char movesCtr = 0;

    if(isWhite(index, gs))
      {
        if(ul(index) < _NONE && isWhite(ul(index), gs) && !isKing(ul(index), gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = ul(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(ur(index) < _NONE && isWhite(ur(index), gs) && !isKing(ur(index), gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = ur(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }
    else if(isBlack(index, gs))
      {
        if(dl(index) < _NONE && isBlack(dl(index), gs) && !isKing(dl(index), gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = dl(index);
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
        if(dr(index) < _NONE && isBlack(dr(index), gs) && !isKing(dr(index), gs))
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
unsigned int getKnightCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(ul(u(index)) < _NONE && sameSide(ul(u(index)), index, gs) && !isKing(ul(u(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(u(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ur(u(index)) < _NONE && sameSide(ur(u(index)), index, gs) && !isKing(ur(u(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(u(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(ur(r(index)) < _NONE && sameSide(ur(r(index)), index, gs) && !isKing(ur(r(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(r(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ul(l(index)) < _NONE && sameSide(ul(l(index)), index, gs) && !isKing(ul(l(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(l(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(dl(d(index)) < _NONE && sameSide(dl(d(index)), index, gs) && !isKing(dl(d(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(d(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dr(d(index)) < _NONE && sameSide(dr(d(index)), index, gs) && !isKing(dr(d(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(d(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    if(dr(r(index)) < _NONE && sameSide(dr(r(index)), index, gs) && !isKing(dr(r(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(r(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dl(l(index)) < _NONE && sameSide(dl(l(index)), index, gs) && !isKing(dl(l(index)), gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(l(index));
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getBishopCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "W";                                                  //  Coverage: stop and include white.
    else
      flags = "B";                                                  //  Coverage: stop and include black.

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getRookCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "W";                                                  //  Coverage: stop and include white.
    else
      flags = "B";                                                  //  Coverage: stop and include black.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getQueenCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "W";                                                  //  Coverage: stop and include white.
    else
      flags = "B";                                                  //  Coverage: stop and include black.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(sameSide(index, tmpBuff[i], gs) && !isKing(tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getKingCoverage(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;

    if(u(index) < _NONE && sameSide(u(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = u(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ur(index) < _NONE && sameSide(ur(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ur(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(r(index) < _NONE && sameSide(r(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = r(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dr(index) < _NONE && sameSide(dr(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dr(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(d(index) < _NONE && sameSide(d(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = d(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(dl(index) < _NONE && sameSide(dl(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = dl(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(l(index) < _NONE && sameSide(l(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = l(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }
    if(ul(index) < _NONE && sameSide(ul(index), index, gs))
      {
        buffer[movesCtr].from = index;
        buffer[movesCtr].to = ul(index);
        buffer[movesCtr].promo = _NO_PROMO;
        movesCtr++;
      }

    return movesCtr;
  }

/**************************************************************************************************
 Scope: defined as all squares theoretically reachable (allies, enemies, empties).  */

/* THIS FUNCTION FILTERS FOR CHECK!!
   Return number of moves. Actual Move objects stored in given buffer.
   NOTICE that all these scope functions test that the piece being attacked is NOT the ally king. */
unsigned int getScope(bool white, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    Move potentialmoves[_NONE];                                     //  Assumes generous upper bound of 64 moves per piece.
    unsigned int potentialmovesCtr = 0;
    unsigned int i;
    unsigned char index;

    for(index = 0; index < _NONE; index++)
      {
        if((white && isWhite(index, gs)) || (!white && isBlack(index, gs)))
          {
            potentialmovesCtr = getScopeIndex(index, gs, potentialmoves);
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
unsigned int getScopeIndex(unsigned char index, GameState* gs, Move* buffer)
  {
    Move potentialmoves[_NONE];                                     //  Assumes a generous upper bound of all squares being reachable from a single index.
    unsigned int potentialmovesCtr = 0;
    unsigned int movesCtr = 0;
    unsigned int i;
    unsigned char j;
    GameState tmp;

    if(!isEmpty(index, gs))
      {
        if(isBishop(index, gs))
          potentialmovesCtr = getBishopScope(index, gs, potentialmoves);
        else if(isRook(index, gs))
          potentialmovesCtr = getRookScope(index, gs, potentialmoves);
        else if(isQueen(index, gs))
          potentialmovesCtr = getQueenScope(index, gs, potentialmoves);

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

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getBishopScope(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "WB";                                                 //  Scope: stop and include white or black.
    else
      flags = "WB";                                                 //  Scope: stop and include white or black.

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getRookScope(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "WB";                                                 //  Scope: stop and include white or black.
    else
      flags = "WB";                                                 //  Scope: stop and include white or black.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getQueenScope(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "WB";                                                 //  Scope: stop and include white or black.
    else
      flags = "WB";                                                 //  Scope: stop and include white or black.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(!(isKing(tmpBuff[i], gs) && sameSide(index, tmpBuff[i], gs)))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/**************************************************************************************************
 X-Ray: defined as enemy-occupied squares attackable if you could go through occupied squares. */

/* THIS FUNCTION FILTERS FOR CHECK!!
   Return number of moves. Actual Move objects stored in given buffer.
   NOTICE that all these X-ray functions test that the piece being attacked is NOT the ally king. */
unsigned int getXRay(bool white, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    Move potentialmoves[_NONE];                                     //  Assumes generous upper bound of 64 moves per piece.
    unsigned int potentialmovesCtr = 0;
    unsigned int i;
    unsigned char index;

    for(index = 0; index < _NONE; index++)
      {
        if((white && isWhite(index, gs)) || (!white && isBlack(index, gs)))
          {
            potentialmovesCtr = getXRayIndex(index, gs, potentialmoves);
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
unsigned int getXRayIndex(unsigned char index, GameState* gs, Move* buffer)
  {
    Move potentialmoves[_NONE];                                     //  Assumes a generous upper bound of all squares being reachable from a single index.
    unsigned int potentialmovesCtr = 0;
    unsigned int movesCtr = 0;
    unsigned int i;
    unsigned char j;
    GameState tmp;

    if(!isEmpty(index, gs))
      {
        if(isBishop(index, gs))
          potentialmovesCtr = getBishopXRay(index, gs, potentialmoves);
        else if(isRook(index, gs))
          potentialmovesCtr = getRookXRay(index, gs, potentialmoves);
        else if(isQueen(index, gs))
          potentialmovesCtr = getQueenXRay(index, gs, potentialmoves);

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

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getBishopXRay(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "wB";                                                 //  X-Ray: pass through white, stop and include black.
    else
      flags = "bW";                                                 //  X-Ray: pass through black, stop and include white.

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getRookXRay(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "wB";                                                 //  X-Ray: pass through white, stop and include black.
    else
      flags = "bW";                                                 //  X-Ray: pass through black, stop and include white.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/* Return number of moves. Actual Move objects stored in given buffer. */
unsigned int getQueenXRay(unsigned char index, GameState* gs, Move* buffer)
  {
    unsigned int movesCtr = 0;
    unsigned char len = 0;
    unsigned char i;
    unsigned char tmpBuff[8];                                       //  Upper bound in any direction is 8 on an 8 x 8 board.
    char* flags;

    if(isWhite(index, gs))
      flags = "wB";                                                 //  X-Ray: pass through white, stop and include black.
    else
      flags = "bW";                                                 //  X-Ray: pass through black, stop and include white.

    len = uSet(index, flags, gs, tmpBuff);                          //  Up
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dSet(index, flags, gs, tmpBuff);                          //  Down
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = lSet(index, flags, gs, tmpBuff);                          //  Left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = rSet(index, flags, gs, tmpBuff);                          //  Right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = ulSet(index, flags, gs, tmpBuff);                         //  Up-left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = urSet(index, flags, gs, tmpBuff);                         //  Up-right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = dlSet(index, flags, gs, tmpBuff);                         //  Down-left
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    len = drSet(index, flags, gs, tmpBuff);                         //  Down-right
    for(i = 0; i < len; i++)
      {
        if(opposed(index, tmpBuff[i], gs))
          {
            buffer[movesCtr].from = index;
            buffer[movesCtr].to = tmpBuff[i];
            buffer[movesCtr].promo = _NO_PROMO;
            movesCtr++;
          }
      }

    return movesCtr;
  }

/**************************************************************************************************
 Pawn Structure
 Scores various pawn features such as doubled, isolated, passed, etc.

 "Pawns are the soul of chess."
             -- Francois-Andre Danican Philidor

 And that is why this function is so complicated. */
float pawnstructure(unsigned char* posTeam, unsigned char posLen,
                    Move* posPawnCoverage, unsigned int posPawnCoverageLen,

                    Move* negMove, unsigned int negMoveLen,
                    Move* negPawnAttackedSq, unsigned int negPawnAttSqLen,

                    GameState* gs)
  {
    float h = 0.0;

    // Convert to (COL, ROW) pairs
    // e.g. 0 1 2 3 4 5 6 7
    //    7 . . . . . . . .
    //    6 p . p . . . p .
    //    5 . . . p . . . p
    //    4 . P . . . . . P
    //    3 . P P . P . P .
    //    2 . . . . . . . .
    //    1 . . . . . . . .  White = [(1, 3), (1, 4), (2, 3), (4, 3), (6, 3)] => [1, 3, 1, 4, 2, 3, 4, 3, 6, 3]
    //    0 . . . . . . . .  Black = [(0, 6), (2, 6), (3, 5), (6, 6), (7, 5)] => [0, 6, 2, 6, 3, 5, 6, 6, 7, 5]

    unsigned char posPawns[16];                                     //  Will have 2 coordinates for each squares containing an ally pawn
    unsigned char posPawnLen = 0;
                                                                    //  For counting doubled pawns
    unsigned char AcolCtr = 0, BcolCtr = 0, CcolCtr = 0, DcolCtr = 0,
                  EcolCtr = 0, FcolCtr = 0, GcolCtr = 0, HcolCtr = 0;
    unsigned int i;

    for(i = 0; i < posLen; i++)                                     //  Find only ally pawns, save as pairs of unsigned chars
      {
        if(isPawn(posTeam[i], gs))
          {
            posPawns[posPawnLen    ] = col(posTeam[i]);
            posPawns[posPawnLen + 1] = row(posTeam[i]);
            posPawnLen += 2;
          }
      }

    for(i = 0; i < posPawnLen; i += 2)                              // For all ally pawns
      {
        if(isIsolatedPawn(posPawns[i], posPawns[i + 1], gs))
          h += ISOLATED_PAWN_PENALTY;

        switch(posPawns[i])                                         //  A pawn is doubled if it occupies another pawn's column
          {
            case 0:  AcolCtr++;  break;
            case 1:  BcolCtr++;  break;
            case 2:  CcolCtr++;  break;
            case 3:  DcolCtr++;  break;
            case 4:  EcolCtr++;  break;
            case 5:  FcolCtr++;  break;
            case 6:  GcolCtr++;  break;
            case 7:  HcolCtr++;  break;
          }

        if(isBackwardPawn(posPawns[i], posPawns[i + 1], posPawnCoverage, posPawnCoverageLen, gs))
          h += BACKWARD_PAWN_PENALTY;

        if(isConnectedPawn(posPawns[i], posPawns[i + 1], gs))
          h += CONNECTED_PAWN_BONUS;

        if(isPassedPawn(posPawns[i], posPawns[i + 1], gs))
          h += PASSED_PAWN_BONUS;

        if(isCandidatePassedPawn(posPawns[i], posPawns[i + 1], gs))
          h += CANDIDATE_PAWN_BONUS;

        if(isUnstoppablePawn(posPawns[i], posPawns[i + 1],
                             negMove, negMoveLen,
                             negPawnAttackedSq, negPawnAttSqLen,
                             gs))
          h += UNSTOPPABLE_PAWN_BONUS;

        if(isSentryPawn(posPawns[i], gs))
          h += SENTRY_PAWN_BONUS;
      }

    if(AcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(BcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(CcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(DcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(EcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(FcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(GcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;
    if(HcolCtr > 1)
      h += DOUBLED_PAWN_PENALTY;

    return h;
  }

/* A pawn is isolated if it has no ally pawn in an adjacent column */
bool isIsolatedPawn(unsigned char pawnCol, unsigned char pawnRow, GameState* gs)
  {
    unsigned char col[8];
    unsigned char i;
    bool white;
    bool ret = true;

    white = isWhite(pawnRow * 8 + pawnCol, gs);

    if(pawnCol == 0)
      {
        getCol(1, col);
        for(i = 0; i < 8; i++)
          {
            if(isPawn(col[i], gs) && ((isWhite(col[i], gs) && white) || (isBlack(col[i], gs) && !white)))
              ret = false;
          }
      }
    else if(pawnCol == 7)
      {
        getCol(6, col);
        for(i = 0; i < 8; i++)
          {
            if(isPawn(col[i], gs) && ((isWhite(col[i], gs) && white) || (isBlack(col[i], gs) && !white)))
              ret = false;
          }
      }
    else
      {
        getCol(pawnCol - 1, col);
        for(i = 0; i < 8; i++)
          {
            if(isPawn(col[i], gs) && ((isWhite(col[i], gs) && white) || (isBlack(col[i], gs) && !white)))
              ret = false;
          }
        getCol(pawnCol + 1, col);
        for(i = 0; i < 8; i++)
          {
            if(isPawn(col[i], gs) && ((isWhite(col[i], gs) && white) || (isBlack(col[i], gs) && !white)))
              ret = false;
          }
      }

    return ret;
  }

/* A pawn is backward if it is undefended and can only advance into an enemy pawn's attack
   e.g.   0 1 2 3 4 5 6 7
        7 . . . . . . . .   r . b q k b . r
        6 . . . . . . . .   p p . . . p p p
        5 . .[p]. . . . .   . . n[p]. n . .
        4 . . . p . . . .   . N . . p . . .
        3 . . . P . . . .   . . . . P . . .
        2 . . . . . . . .   . . N . . . . .
        1 . . . . . . . .   P P P . . P P P
        0 . . . . . . . .   R . B Q K B . R  */
bool isBackwardPawn(unsigned char pawnCol, unsigned char pawnRow, Move* posPawnCoverage, unsigned char posPawnCoverageLen, GameState* gs)
  {
    unsigned char i = 0;
    unsigned char index = pawnRow * 8 + pawnCol;
    bool lBackward = false;
    bool rBackward = false;
    bool covered = false;
    bool ret = false;

    if(isWhite(index, gs))
      {
        if(u(index) < _NONE && isEmpty(u(index), gs))               //  Can the pawn advance?
          {
                                                                    //  Backward situation on right?
            if(u(ur(index)) < _NONE && opposed(index, u(ur(index)), gs) && isPawn(u(ur(index)), gs)
                                    && sameSide(index, ur(index), gs) && isPawn(ur(index), gs))
              rBackward = true;
                                                                    //  Backward situation on left?
            if(u(ul(index)) < _NONE && opposed(index, u(ul(index)), gs) && isPawn(u(ul(index)), gs)
                                    && sameSide(index, ul(index), gs) && isPawn(ul(index), gs))
              lBackward = true;
                                                                    //  Is the square into which this pawn will move already
                                                                    //  targeted by an enemy pawn which is blocked (sentry)?
            while(i < posPawnCoverageLen && posPawnCoverage[i].to != u(index))
              i++;

            if(i < posPawnCoverageLen)
              covered = true;

            ret = (!covered && (lBackward || rBackward));
          }
      }
    else
      {
        if(d(index) < _NONE && isEmpty(d(index), gs))               //  Can the pawn advance?
          {
                                                                    //  Backward situation on right?
            if(d(dr(index)) < _NONE && opposed(index, d(dr(index)), gs) && isPawn(d(dr(index)), gs)
                                    && sameSide(index, dr(index), gs) && isPawn(dr(index), gs))
              rBackward = true;
                                                                    //  Backward situation on left?
            if(d(dl(index)) < _NONE && opposed(index, d(dl(index)), gs) && isPawn(d(dl(index)), gs)
                                    && sameSide(index, dl(index), gs) && isPawn(dl(index), gs))
              lBackward = true;
                                                                    //  Is the square into which this pawn will move already
                                                                    //  targeted by an enemy pawn which is blocked (sentry)?
            while(i < posPawnCoverageLen && posPawnCoverage[i].to != d(index))
              i++;

            if(i < posPawnCoverageLen)
              covered = true;

            ret = (!covered && (lBackward || rBackward));
          }
      }

    return ret;
  }

/* A pawn is connected if it stands next to, defends or is defended by an ally pawn */
bool isConnectedPawn(unsigned char pawnCol, unsigned char pawnRow, GameState* gs)
  {
    unsigned char index = pawnRow * 8 + pawnCol;

    if(pawnCol == 0)
      {
        return ( (sameSide(index, r(index), gs) && isPawn(r(index), gs)) ||
                 (sameSide(index, ur(index), gs) && isPawn(ur(index), gs)) ||
                 (sameSide(index, dr(index), gs) && isPawn(dr(index), gs)) );
      }
    else if(pawnCol == 7)
      {
        return ( (sameSide(index, l(index), gs) && isPawn(l(index), gs)) ||
                 (sameSide(index, ul(index), gs) && isPawn(ul(index), gs)) ||
                 (sameSide(index, dl(index), gs) && isPawn(dl(index), gs)) );
      }
    else
      {
        return ( (sameSide(index, r(index), gs) && isPawn(r(index), gs)) ||
                 (sameSide(index, ur(index), gs) && isPawn(ur(index), gs)) ||
                 (sameSide(index, dr(index), gs) && isPawn(dr(index), gs)) ||
                 (sameSide(index, l(index), gs) && isPawn(l(index), gs)) ||
                 (sameSide(index, ul(index), gs) && isPawn(ul(index), gs)) ||
                 (sameSide(index, dl(index), gs) && isPawn(dl(index), gs)) );
      }
  }

/* A pawn is passed if no opposing pawns stand ahead of it in its column or any adjacent column
   e.g.   0  1  2  3  4  5  6  7
        7 .  .  .  .  .  .  .  .
        6 .  .  .  .  .  .  .  .
        5 .  .  .  .  .  .  .  p
        4 . [P] .  . [P] p  .  P
        3 .  . [P][p] .  P  P  .
        2 .  .  .  .  .  .  .  .
        1 .  .  .  .  .  .  .  .
        0 .  .  .  .  .  .  .  .  */
bool isPassedPawn(unsigned char pawnCol, unsigned char pawnRow, GameState* gs)
  {
    unsigned char i;
    unsigned char index = pawnRow * 8 + pawnCol;

    if(isWhite(index, gs))
      {
        if(pawnCol == 0)
          {
            i = u(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            i = ur(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            return true;
          }
        else if(pawnCol == 7)
          {
            i = u(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            i = ul(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            return true;
          }
        else
          {
            i = u(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            i = ur(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            i = ul(index);
            while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
              i = u(i);

            if(i < _NONE)
              return false;

            return true;
          }
      }
    else
      {
        if(pawnCol == 0)
          {
            i = d(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            i = dr(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            return true;
          }
        else if(pawnCol == 7)
          {
            i = d(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            i = dl(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            return true;
          }
        else
          {
            i = d(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            i = dr(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            i = dl(index);
            while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
              i = d(i);

            if(i < _NONE)
              return false;

            return true;
          }
      }
  }

/* A pawn is candidate-passed if it is on a semi-open file
   and could become a passed pawn through advancing and pawn exchanges with the enemy guard.
   e.g.   0 1 2 3 4 5 6 7
        7 . . . . . . . .
        6 p . . . . . . .
        5 . . . . . . . .
        4 P[P]. . . p[p]p
        3 . . . . . . . .
        2 . . . . . P . P
        1 . . . . . . . .
        0 . . . . . . . .  */
bool isCandidatePassedPawn(unsigned char pawnCol, unsigned char pawnRow, GameState* gs)
  {
    bool lClearFile = false;
    bool rClearFile = false;
    bool lAllyAndOpposed = false;
    bool rAllyAndOpposed = false;
    unsigned char index = pawnRow * 8 + pawnCol;
    unsigned char i;

    if(isSemiOpenFile(index, gs))
      {
        if(isWhite(index, gs))
          {
            if(pawnCol == 0)
              {
                i = ur(index);
                while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
                  i = u(i);
                rAllyAndOpposed = (i < _NONE && isWhite(r(index), gs) && isPawn(r(index), gs));
                lClearFile = true;
              }
            else if(pawnCol == 7)
              {
                i = ul(index);
                while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
                  i = u(i);
                lAllyAndOpposed = (i < _NONE && isWhite(l(index), gs) && isPawn(l(index), gs));
                rClearFile = true;
              }
            else
              {
                i = ur(index);
                while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
                  i = u(i);
                rAllyAndOpposed = (i < _NONE && isWhite(r(index), gs) && isPawn(r(index), gs));
                rClearFile = (i == _NONE);

                i = ul(index);
                while(i < _NONE && !(isBlack(i, gs) && isPawn(i, gs)))
                  i = u(i);
                lAllyAndOpposed = (i < _NONE && isWhite(l(index), gs) && isPawn(l(index), gs));
                lClearFile = (i == _NONE);
              }
          }
        else
          {
            if(pawnCol == 0)
              {
                i = dr(index);
                while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
                  i = d(i);
                rAllyAndOpposed = (i < _NONE && isBlack(r(index), gs) && isPawn(r(index), gs));
                lClearFile = true;
              }
            else if(pawnCol == 7)
              {
                i = dl(index);
                while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
                  i = d(i);
                lAllyAndOpposed = (i < _NONE && isBlack(l(index), gs) && isPawn(l(index), gs));
                rClearFile = true;
              }
            else
              {
                i = dr(index);
                while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
                  i = d(i);
                rAllyAndOpposed = (i < _NONE && isBlack(r(index), gs) && isPawn(r(index), gs));
                rClearFile = (i == _NONE);

                i = dl(index);
                while(i < _NONE && !(isWhite(i, gs) && isPawn(i, gs)))
                  i = d(i);
                lAllyAndOpposed = (i < _NONE && isBlack(l(index), gs) && isPawn(l(index), gs));
                lClearFile = (i == _NONE);
              }
          }
      }

    return ((lClearFile && rClearFile) ||
            (lClearFile && rAllyAndOpposed) ||
            (lAllyAndOpposed && rClearFile) ||
            (lAllyAndOpposed && rAllyAndOpposed));
  }

/* A pawn is unstoppable if it is sure to advance to promotion,
   given the other pieces on the board and their distance to the pawn.
   e.g.   0 1 2 3 4 5 6 7
        7 . . . +-----+ .
        6 . . . | . . | .
        5 . . . | . . | .
        4 . . . P-----+ .
        3 . . . . . . . k
        2 . . . . . . . .
        1 . . . . . . . .
        0 . . . . . . . .  */
bool isUnstoppablePawn(unsigned char pawnCol, unsigned char pawnRow, Move* negMove, unsigned char negMoveLen, Move* negPawnAttackedSq, unsigned char negPawnAttSqLen, GameState* gs)
  {
    unsigned char index = pawnRow * 8 + pawnCol;
    unsigned char c[8];
    signed char i;
    unsigned char j;
    bool hit = false;

    if(isSemiOpenFile(index, gs))
      {
        getCol(index, c);                                           //  Get column for this pawn
        i = 0;
        while(i < 8 && c[i] != index)                               //  Find pawn's index in column
          i++;

        if(isWhite(index, gs))
          {
            while(i < 8)                                            //  White pawns move UP
              {
                j = 0;
                while(j < negMoveLen && negMove[j].to != c[i])      //  Find farther squares among enemy attacks
                  j++;

                if(j < negMoveLen)                                  //  Pawn must cross attacked square: NOT unstoppable
                  {
                    hit = true;
                    break;
                  }

                j = 0;
                while(j < negPawnAttSqLen && negPawnAttackedSq[j].to != c[i])
                  j++;

                if(j < negPawnAttSqLen)                             //  Pawn must cross attacked square: NOT unstoppable
                  {
                    hit = true;
                    break;
                  }

                i++;
              }
          }
        else
          {
            while(i >= 0)                                           //  Black pawns move DOWN
              {
                j = 0;
                while(j < negMoveLen && negMove[j].to != c[i])      //  Find farther squares among enemy attacks
                  j++;

                if(j < negMoveLen)                                  //  Pawn must cross attacked square: NOT unstoppable
                  {
                    hit = true;
                    break;
                  }

                j = 0;
                while(j < negPawnAttSqLen && negPawnAttackedSq[j].to != c[i])
                  j++;

                if(j < negPawnAttSqLen)                             //  Pawn must cross attacked square: NOT unstoppable
                  {
                    hit = true;
                    break;
                  }

                i--;
              }
          }
      }
    else                                                            //  Pawn is blocked: NOT unstoppable
      return false;

    return !hit;
  }

/* A pawn is sentry if it stands in the same column as an enemy pawn (blocking advancement) */
bool isSentryPawn(unsigned char pawnCol, GameState* gs)
  {
    return (!isSemiOpenFile(pawnCol, gs) && !isOpenFile(pawnCol, gs));
  }

/**************************************************************************************************
 Development  */
float development(bool white, GameState* gs)
  {
    unsigned char D = 0;                                            //  D = Number of minor pieces NOT on their original squares.
    unsigned char U = 0;                                            //  U = 0 if the queen has neither been moved nor captured
                                                                    //    = number of undeveloped pieces (major & minor) if the queen has been moved
                                                                    //      but not captured.
    float C = 0.0;                                                  //  C = 2 if the opponent's queen is still on the board
                                                                    //    = (1 - P/4) if the opponent's queen is not still on the board,
                                                                    //      where P is the number of opposing rooks and minor pieces which have been captured
    float k = 0.0;                                                  //  k = 0 if the player has castled
                                                                    //    = 1/3 if the player has lost queen side castling rights
                                                                    //    = 2/3 if the player has lost king side castling rights
                                                                    //    = 1 if the player has lost all castling rights
    unsigned char i;
    float P = 0.0;
    unsigned char nCtr = 0, bCtr = 0, rCtr = 0;

    if(white)
      {
        if(gs->board[1] != 'N')                                     //  Knight NOT on its original sqaure
          D++;
        if(gs->board[6] != 'N')                                     //  Knight NOT on its original sqaure
          D++;
        if(gs->board[2] != 'B')                                     //  Bishop NOT on its original square
          D++;
        if(gs->board[5] != 'B')                                     //  Bishop NOT on its original square
          D++;

        i = 0;                                                      //  Find White Queen on board
        while(i < _NONE && gs->board[i] != 'Q')
          i++;
        if(i == _NONE || gs->board[3] == 'Q')
          U = 0;
        else if(i < _NONE && gs->board[3] != 'Q')
          {
            if(gs->board[0] == 'R')
              U++;
            if(gs->board[1] == 'N')
              U++;
            if(gs->board[2] == 'B')
              U++;
            if(gs->board[5] == 'B')
              U++;
            if(gs->board[6] == 'N')
              U++;
            if(gs->board[7] == 'R')
              U++;
          }

        i = 0;                                                      //  Find Black Queen
        while(i < _NONE && gs->board[i] != 'q')
          i++;
        if(i < _NONE)
          C = 2.0;
        else
          {
            P = 0.0;
            i = 0;
            while(i < _NONE)
              {
                if(gs->board[i] == 'n')
                  nCtr++;
                if(gs->board[i] == 'b')
                  bCtr++;
                if(gs->board[i] == 'r')
                  rCtr++;
                i++;
              }
            if((2 - nCtr) > 0)
              P += (float)(2 - nCtr);
            if((2 - bCtr) > 0)
              P += (float)(2 - bCtr);
            if((2 - rCtr) > 0)
              P += (float)(2 - rCtr);
            C = 1.0 - P / 4.0;
          }

        if(!whiteCastled(gs))                                       //  Castling privileges
          {
            if(whiteKingsidePrivilege(gs) && whiteQueensidePrivilege(gs))
              k = 1.0;
            else if(whiteKingsidePrivilege(gs))
              k = 2.0 / 3.0;
            else if(whiteQueensidePrivilege(gs))
              k = 1.0 / 3.0;
          }
      }
    else
      {
        if(gs->board[57] != 'n')                                    //  Knight NOT on its original sqaure
          D++;
        if(gs->board[62] != 'n')                                    //  Knight NOT on its original sqaure
          D++;
        if(gs->board[58] != 'b')                                    //  Bishop NOT on its original square
          D++;
        if(gs->board[61] != 'b')                                    //  Bishop NOT on its original square
          D++;

        i = 0;                                                      //  Find Black Queen on board
        while(i < _NONE && gs->board[i] != 'q')
          i++;
        if(i == _NONE || gs->board[59] == 'q')
          U = 0;
        else if(i < _NONE && gs->board[59] != 'q')
          {
            if(gs->board[56] == 'r')
              U++;
            if(gs->board[57] == 'n')
              U++;
            if(gs->board[58] == 'b')
              U++;
            if(gs->board[61] == 'b')
              U++;
            if(gs->board[62] == 'n')
              U++;
            if(gs->board[63] == 'r')
              U++;
          }

        i = 0;                                                      //  Find White Queen
        while(i < _NONE && gs->board[i] != 'Q')
          i++;
        if(i < _NONE)
          C = 2.0;
        else
          {
            P = 0.0;
            i = 0;
            while(i < _NONE)
              {
                if(gs->board[i] == 'N')
                  nCtr++;
                if(gs->board[i] == 'B')
                  bCtr++;
                if(gs->board[i] == 'R')
                  rCtr++;
                i++;
              }
            if((2 - nCtr) > 0)
              P += (float)(2 - nCtr);
            if((2 - bCtr) > 0)
              P += (float)(2 - bCtr);
            if((2 - rCtr) > 0)
              P += (float)(2 - rCtr);
            C = 1.0 - P / 4.0;
          }

        if(!blackCastled(gs))                                       //  Castling privileges
          {
            if(blackKingsidePrivilege(gs) && blackQueensidePrivilege(gs))
              k = 1.0;
            else if(blackKingsidePrivilege(gs))
              k = 2.0 / 3.0;
            else if(blackQueensidePrivilege(gs))
              k = 1.0 / 3.0;
          }
      }

    return (D / 3.0 - U / 4.0 - k * C) * 10.0;

    return 0.0;
  }

/**************************************************************************************************
 Piece Evaluations   */

float pieceeval(unsigned char* posTeam, unsigned char posLen,
                Move* posMoves, unsigned int posMovesLen,
                Move* posCoverage, unsigned int posCoverageLen,
                Move* posPawnCoverage, unsigned int posPawnCoverageLen,
                Move* posPawnTargets, unsigned int posPawnTargetsLen,
                Move* posScope, unsigned int posScopeLen,
                Move* posXRay, unsigned int posXRayLen,

                unsigned char* negTeam, unsigned char negLen,
                Move* negMoves, unsigned int negMovesLen,

                Move* negPawnTargets, unsigned int negPawnTargetsLen,

                GameState* gs)
  {
    float h = 0.0;
    unsigned char i;

    for(i = 0; i < posLen; i++)
      {
        if(isPawn(posTeam[i], gs))
          h += pawnEval(posTeam, posLen,
                        posMoves, posMovesLen,
                        posPawnTargets, posPawnTargetsLen,
                        gs);
        else if(isKnight(posTeam[i], gs))
          h += knightEval(posTeam[i],
                          posTeam, posLen,
                          posMoves, posMovesLen,
                          posCoverage, posCoverageLen,
                          posPawnCoverage, posPawnCoverageLen,
                          posPawnTargets, posPawnTargetsLen,
                          negPawnTargets, negPawnTargetsLen,
                          gs);
        else if(isBishop(posTeam[i], gs))
          h += bishopEval(posTeam[i],
                          posTeam, posLen,
                          posCoverage, posCoverageLen,
                          posPawnCoverage, posPawnCoverageLen,
                          posScope, posScopeLen,
                          negTeam, negLen,
                          negPawnTargets, negPawnTargetsLen,
                          gs);
        else if(isRook(posTeam[i], gs))
          h += rookEval(posTeam[i],
                        posTeam, posLen,
                        posCoverage, posCoverageLen,
                        negTeam, negLen,
                        gs);
        else if(isQueen(posTeam[i], gs))
          h += queenEval(posTeam[i],
                         posTeam, posLen,
                         posMoves, posMovesLen,
                         posXRay, posXRayLen,
                         gs);
        else
          h += kingEval(posTeam[i],
                        negTeam, negLen,
                        negMoves, negMovesLen,
                        gs);
      }

    return h;
  }

/*  Pawn Evaluations  *****************************************************************************/
/* Award points and issue penalties for individual pawn attributes.
   These attributes are separate from Pawn Structure */
float pawnEval(unsigned char* posTeam, unsigned char posLen,
               Move* posMoves, unsigned int posMovesLen,
               Move* posTeamPawnTargets, unsigned int posPawnTargetsLen,
               GameState* gs)
  {
    float h = 0.0;
    unsigned int i;
    unsigned char centerMap[_NONE];

    if(isWhite(posTeam[0], gs))                                     //  Fill in map.
      buildCenterMap(true, centerMap);
    else
      buildCenterMap(false, centerMap);

    for(i = 0; i < posLen; i++)                                     //  Pawns OCCUPYING the center
      {
        if(isPawn(posTeam[i], gs))
          {
            if(centerMap[posTeam[i]] > 6)
              h += PAWN_OCCUPY_CENTER_BONUS;
                                                                    //  Pawn blockades
            h += pawnBlockade(posTeam[i], gs);
                                                                    //  Blocked D and E pawns
            h += pawnBlocked_DE(posTeam[i], posMoves, posMovesLen, gs);
          }
      }

    for(i = 0; i < posPawnTargetsLen; i++)                          //  Pawns ATTACKING the center
      {
        if( centerMap[ posTeamPawnTargets[i].to ] > 6 )
          h += PAWN_ATTACK_CENTER_BONUS;
      }

    return h;
  }

/* Blockade of stops */
float pawnBlockade(unsigned char i, GameState* gs)
  {
    float h = 0.0;

    if(isSemiOpenFile(i, gs) && !isOpenFile(i, gs))
      {
        if( (isWhite(i, gs) && row(i) > 3) || (isBlack(i, gs) && row(i) < 4) )
          h += PAWN_BLOCKADE_BONUS;
      }

    return h;
  }

/* Penalty for d and e pawns blocked on their initial squares */
float pawnBlocked_DE(unsigned char i,
                     Move* posMoves, unsigned int posMovesLen,
                     GameState* gs)
  {
    float h = 0.0;
    unsigned int ctr1, ctr2;

    if(isWhite(i, gs))
      {
        if(i == 11)                                                 //  Considering pawn on D2: can it reach either D3 or D4?
          {
            ctr1 = 0;
            while(ctr1 < posMovesLen && !(posMoves[ctr1].from == 11 && posMoves[ctr1].to == 19))
              ctr1++;

            ctr2 = 0;
            while(ctr2 < posMovesLen && !(posMoves[ctr2].from == 11 && posMoves[ctr2].to == 27))
              ctr2++;

            if(ctr1 == posMovesLen && ctr2 == posMovesLen)          //  If these moves were not found, then the pawn is somehow blocked
              h += PAWN_STUCK_ON_INITIAL_PENALTY;
          }
        else if(i == 12)                                            //  Considering pawn on E2: can it reach either E3 or E4?
          {
            ctr1 = 0;
            while(ctr1 < posMovesLen && !(posMoves[ctr1].from == 12 && posMoves[ctr1].to == 20))
              ctr1++;

            ctr2 = 0;
            while(ctr2 < posMovesLen && !(posMoves[ctr2].from == 12 && posMoves[ctr2].to == 28))
              ctr2++;

            if(ctr1 == posMovesLen && ctr2 == posMovesLen)          //  If these moves were not found, then the pawn is somehow blocked
              h += PAWN_STUCK_ON_INITIAL_PENALTY;
          }
      }
    else
      {
        if(i == 51)                                                 //  Considering pawn on D7: can it reach either D6 or D5?
          {
            ctr1 = 0;
            while(ctr1 < posMovesLen && !(posMoves[ctr1].from == 51 && posMoves[ctr1].to == 43))
              ctr1++;

            ctr2 = 0;
            while(ctr2 < posMovesLen && !(posMoves[ctr2].from == 51 && posMoves[ctr2].to == 35))
              ctr2++;

            if(ctr1 == posMovesLen && ctr2 == posMovesLen)          //  If these moves were not found, then the pawn is somehow blocked
              h += PAWN_STUCK_ON_INITIAL_PENALTY;
          }
        else if(i == 52)
          {
            ctr1 = 0;
            while(ctr1 < posMovesLen && !(posMoves[ctr1].from == 52 && posMoves[ctr1].to == 44))
              ctr1++;

            ctr2 = 0;
            while(ctr2 < posMovesLen && !(posMoves[ctr2].from == 52 && posMoves[ctr2].to == 36))
              ctr2++;

            if(ctr1 == posMovesLen && ctr2 == posMovesLen)          //  If these moves were not found, then the pawn is somehow blocked
              h += PAWN_STUCK_ON_INITIAL_PENALTY;
          }
      }

    return h;
  }

/*  Knight Evaluations  ***************************************************************************/
/* Award points and issue penalties for individual knight attributes. */
float knightEval(unsigned char index,
                 unsigned char* posTeam, unsigned char posLen,
                 Move* posMoves, unsigned int posMovesLen,
                 Move* posCoverage, unsigned int posCoverageLen,
                 Move* posPawnCoverage, unsigned int posPawnCoverageLen,
                 Move* posTeamPawnTargets, unsigned int posPawnTargetsLen,
                 Move* negTeamPawnTargets, unsigned int negPawnTargetsLen,
                 GameState* gs)
  {
    float h = 0.0;

    h += knightDecreasePawnVal(gs);
    h += knightOutpost(index,
                       posTeamPawnTargets, posPawnTargetsLen,
                       negTeamPawnTargets, negPawnTargetsLen,
                       gs);
    h += knightTrapped(index);
    h += knightBlockingCPawn(index, gs);
    h += knightMobility(index,
                        posMoves, posMovesLen,
                        negTeamPawnTargets, negPawnTargetsLen);
    h += knightBishopDefense(index,
                             posTeam, posLen,
                             posPawnCoverage, posPawnCoverageLen,
                             gs);
    h += knightUndefended(index, posCoverage, posCoverageLen);

    return h;
  }

/* Knights decrease in value as Pawns diminish */
float knightDecreasePawnVal(GameState* gs)
  {
    unsigned char wP = 0, bP = 0;
    unsigned char i;
    for(i = 0; i < _NONE; i++)
      {
        if(isPawn(i, gs))
          {
            if(isWhite(i, gs))
              wP++;
            else
              bP++;
          }
      }

    return (16.0 - (float)wP - (float)bP) * KNIGHT_PAWN_DIMINISH;
  }

/* Knight in an outpost is defended by a friendly pawn, unattacked by an enemy pawn, and on the 4th rank */
float knightOutpost(unsigned char index,
                    Move* posTeamPawnTargets, unsigned int posPawnTargetsLen,
                    Move* negTeamPawnTargets, unsigned int negPawnTargetsLen,
                    GameState* gs)
  {
    float h = 0.0;
    unsigned int i = 0, j = 0;

    while(i < posPawnTargetsLen && posTeamPawnTargets[i].to != index)
      i++;

    while(j < negPawnTargetsLen && negTeamPawnTargets[j].to != index)
      j++;

    if(i < posPawnTargetsLen && j == negPawnTargetsLen)
      {
        if( (isWhite(index, gs) && row(index) > 3) || (isBlack(index, gs) && row(index) < 4) )
          h += KNIGHT_OUTPOST_BONUS;
      }

    return h;
  }

/* Knight trapped on A8/H8/A7/H7 or A1/H1/A2/H2 (where scope < 4) */
float knightTrapped(unsigned char index)
  {
    if(index == 0 || index == 8 ||
       index == 7 || index == 15 ||
       index == 56 || index == 48 ||
       index == 63 || index == 55)
      return KNIGHT_TRAPPED_PENALTY;
    return 0.0;
  }

/* Penalty for blocking a c pawn in "closed" openings:
   (Crafty defines it as follows: white knight on c3, white pawns on c2 and d4, no white pawn on e4) */
float knightBlockingCPawn(unsigned char index, GameState* gs)
  {
    if(isWhite(index, gs))
      {
        if( index == 18 &&
            isPawn(10, gs) && isWhite(10, gs) &&
            isPawn(27, gs) && isWhite(27, gs) &&
           !(isPawn(28, gs) && isWhite(28, gs)) )
          return KNIGHT_BLOCKS_C_PAWN_PENALTY;
      }
    else
      {
        if( index == 42 &&
            isPawn(50, gs) && isBlack(50, gs) &&
            isPawn(35, gs) && isBlack(35, gs) &&
           !(isPawn(36, gs) && isBlack(36, gs)) )
          return KNIGHT_BLOCKS_C_PAWN_PENALTY;
      }

    return 0.0;
  }

/* Penalize knight-mobile squares controlled by enemy pawns */
float knightMobility(unsigned char index,
                      Move* posMoves, unsigned int posMovesLen,
                      Move* negTeamPawnTargets, unsigned int negPawnTargetsLen)
  {
    unsigned int i, j;
    unsigned char mobCtr = 0;

    for(i = 0; i < posMovesLen; i++)
      {
        j = 0;                                                      //  Search enemy-pawn-attacked squares for this i-th knight's move
        while(j < negPawnTargetsLen && negTeamPawnTargets[j].to != posMoves[i].to)
          j++;

        if(posMoves[i].from == index && j < negPawnTargetsLen)
          mobCtr++;
      }

    return (float)mobCtr * KNIGHT_MOBILITY_PAWN_CONTROL_PENALTY;
  }

/* Marginal bonus for a bishop defended by a pawn--yes, as SCORED BY A KNIGHT */
float knightBishopDefense(unsigned char index,
                          unsigned char* posTeam, unsigned char posLen,
                          Move* posPawnCoverage, unsigned int posPawnCoverageLen,
                          GameState* gs)
  {
    float h = 0.0;
    unsigned char i;
    unsigned int j;

    for(i = 0; i < posLen; i++)
      {
        if(isBishop(posTeam[i], gs))
          {
            j = 0;
            while(j < posPawnCoverageLen && posPawnCoverage[j].to != posTeam[i])
              j++;

            if(j < posPawnCoverageLen)
              h += KNIGHT_BISHOP_DEFENSE_BONUS;
          }
      }

    return h;
  }

/* Penalty for an undefended minor piece */
float knightUndefended(unsigned char index, Move* posCoverage, unsigned int posCoverageLen)
  {
    unsigned int i = 0;

    while(i < posCoverageLen && posCoverage[i].to != index)
      i++;

    if(i == posCoverageLen)
      return UNDEFENDED_MINOR_PIECE_PENALTY;

    return 0.0;
  }

/*  Bishop Evaluations  ***************************************************************************/
/* Award points and issue penalties for individual bishop attributes. */
float bishopEval(unsigned char index,
                 unsigned char* posTeam, unsigned char posLen,
                 Move* posCoverage, unsigned int posCoverageLen,
                 Move* posPawnCoverage, unsigned int posPawnCoverageLen,
                 Move* posScope, unsigned int posScopeLen,
                 unsigned char* negTeam, unsigned char negLen,
                 Move* negPawnAttacks, unsigned int negPawnAttacksLen,
                 GameState* gs)
  {
    float h = 0.0;

    h += bishopPair(index, posTeam, posLen, gs);
    h += badBishop(index, posScope, posScopeLen, gs);
    h += bishopColorWeakness(posTeam, posLen, gs);
    h += Fianchetto(index, negTeam, negLen, gs);
    h += bishopReturn(index, gs);
    h += bishopTrapped(index, negPawnAttacks, negPawnAttacksLen);
    h += bishopKnightDefense(index,
                             posTeam, posLen,
                             posPawnCoverage, posPawnCoverageLen, gs);
    h += bishopUndefended(index, posCoverage, posCoverageLen);

    return h;
  }

/* Called on assumption that index has already been identified as a bishop */
float bishopPair(unsigned char index, unsigned char* posTeam, unsigned char posLen, GameState* gs)
  {
    float h = 0.0;
    unsigned char i = 0;

    while(i < posLen)
      {
        if(posTeam[i] != index && isBishop(posTeam[i], gs))
          {
            h += BISHOP_PAIR_BONUS;                                 //  Counted twice, once per bishop
            break;
          }
        i++;
      }

    return h;
  }

/* Bad bishop: a bishop whose mobility is restricted by ally pawns */
float badBishop(unsigned char index,
                Move* posScope, unsigned int posScopeLen,
                GameState* gs)
  {
    float h = 0.0;
    unsigned int i;

    for(i = 0; i < posScopeLen; i++)
      {
        if(posScope[i].from == index && isPawn(posScope[i].to, gs) && sameSide(index, posScope[i].to, gs) )
          h += BISHOP_BAD_PENALTY;
      }

    return h;
  }

/* Color weakness */
float bishopColorWeakness(unsigned char* posTeam, unsigned char posLen, GameState* gs)
  {
    unsigned char darkSq[32];
    unsigned char i, j;
    unsigned char darkCtr = 0, lightCtr = 0;

    darkSquares(darkSq);

    for(i = 0; i < posLen; i++)
      {
        if(isPawn(posTeam[i], gs) || isBishop(posTeam[i], gs))
          {
            j = 0;
            while(j < 32 && darkSq[j] != posTeam[i])
              j++;

            if(j < 32)
              darkCtr++;
            else
              lightCtr++;
          }
      }

    return (float)abs(darkCtr - lightCtr) * BISHOP_COLOR_WEAK_PENALTY;
  }

unsigned char darkSquares(unsigned char* sq)
  {
    sq[0] = 0;   sq[1] = 2;   sq[2] = 4;   sq[3] = 6;
    sq[4] = 9;   sq[5] = 11;  sq[6] = 13;  sq[7] = 15;
    sq[8] = 16;  sq[9] = 18;  sq[10] = 20; sq[11] = 22;
    sq[12] = 25; sq[13] = 27; sq[14] = 29; sq[15] = 31;
    sq[16] = 32; sq[17] = 34; sq[18] = 36; sq[19] = 38;
    sq[20] = 41; sq[21] = 43; sq[22] = 45; sq[23] = 47;
    sq[24] = 48; sq[25] = 50; sq[26] = 52; sq[27] = 54;
    sq[28] = 57; sq[29] = 59; sq[30] = 61; sq[31] = 63;
    return 32;
  }

unsigned char lightSquares(unsigned char* sq)
  {
    sq[0] = 1;   sq[1] = 3;   sq[2] = 5;   sq[3] = 7;
    sq[4] = 8;   sq[5] = 10;  sq[6] = 12;  sq[7] = 14;
    sq[8] = 17;  sq[9] = 19;  sq[10] = 21; sq[11] = 23;
    sq[12] = 24; sq[13] = 26; sq[14] = 28; sq[15] = 30;
    sq[16] = 33; sq[17] = 35; sq[18] = 37; sq[19] = 39;
    sq[20] = 40; sq[21] = 42; sq[22] = 44; sq[23] = 46;
    sq[24] = 49; sq[25] = 51; sq[26] = 53; sq[27] = 55;
    sq[28] = 56; sq[29] = 58; sq[30] = 60; sq[31] = 62;
    return 32;
  }

/* Fianchetto */
float Fianchetto(unsigned char index, unsigned char* negTeam, unsigned char negLen, GameState* gs)
  {
    float h = 0.0;
    unsigned char i, j;
    unsigned char darkSq[32];
    unsigned char lightSq[32];
    bool found;

    darkSquares(darkSq);
    lightSquares(lightSq);

    if(isWhite(index, gs))
      {
        if(index == 14)
          {
            if( isWhite(21, gs) && isKnight(21, gs) &&
               ( (isWhite(22, gs) && isPawn(22, gs) ) || (isWhite(30, gs) && isPawn(30, gs)) ) )
              h += BISHOP_FIANCHETTO_BONUS;

            found = false;                                          //  (May have to find both enemy bishops)
            for(i = 0; i < negLen; i++)
              {
                if(isBishop(negTeam[i], gs))
                  {
                    j = 0;
                    while(j < 32 && lightSq[j] != negTeam[i])
                      j++;

                    if(j < 32)
                      found = true;
                  }
                if(found)
                  break;
              }

            if(found)                                               //  White Bishop against opponent's white Bishop
              h += BISHOP_FIANCHETTO_AGAINST_SAME_COLOR_BISHOP_BONUS;
          }
        else if(index == 9)
          {
            if( isWhite(18, gs) && isKnight(18, gs) &&
               ( (isWhite(17, gs) && isPawn(17, gs) ) || (isWhite(25, gs) && isPawn(25, gs)) ) )
              h += BISHOP_FIANCHETTO_BONUS;

            found = false;                                          //  (May have to find both enemy bishops)
            for(i = 0; i < negLen; i++)
              {
                if(isBishop(negTeam[i], gs))
                  {
                    j = 0;
                    while(j < 32 && darkSq[j] != negTeam[i])
                      j++;

                    if(j < 32)
                      found = true;
                  }
                if(found)
                  break;
              }

            if(found)                                               //  Black Bishop against opponent's black Bishop
              h += BISHOP_FIANCHETTO_AGAINST_SAME_COLOR_BISHOP_BONUS;
          }
      }
    else
      {
        if(index == 49)
          {
            if( isBlack(42, gs) && isKnight(42, gs) &&
               ( (isBlack(41, gs) && isPawn(41, gs) ) || (isBlack(33, gs) && isPawn(33, gs)) ) )
              h += BISHOP_FIANCHETTO_BONUS;

            found = false;                                          //  (May have to find both enemy bishops)
            for(i = 0; i < negLen; i++)
              {
                if(isBishop(negTeam[i], gs))
                  {
                    j = 0;
                    while(j < 32 && lightSq[j] != negTeam[i])
                      j++;

                    if(j < 32)
                      found = true;
                  }
                if(found)
                  break;
              }

            if(found)                                               //  White Bishop against opponent's white Bishop
              h += BISHOP_FIANCHETTO_AGAINST_SAME_COLOR_BISHOP_BONUS;
          }
        else if(index == 54)
          {
            if( isBlack(45, gs) && isKnight(45, gs) &&
               ( (isBlack(46, gs) && isPawn(46, gs) ) || (isBlack(38, gs) && isPawn(38, gs)) ) )
              h += BISHOP_FIANCHETTO_BONUS;

            found = false;                                          //  (May have to find both enemy bishops)
            for(i = 0; i < negLen; i++)
              {
                if(isBishop(negTeam[i], gs))
                  {
                    j = 0;
                    while(j < 32 && darkSq[j] != negTeam[i])
                      j++;

                    if(j < 32)
                      found = true;
                  }
                if(found)
                  break;
              }

            if(found)                                               //  Black Bishop against opponent's black Bishop
              h += BISHOP_FIANCHETTO_AGAINST_SAME_COLOR_BISHOP_BONUS;
          }
      }

    return h;
  }

/* Returning bishop: a bishop returned to its original square after allowing the king to castle in
   the same direction. Like this: 1.e4 c5 2.Nf3 Nc6 3.Bb5 d6 4.0-0 Bd7 5. c3 Nf6 6.Re1 a6 7.Bf1.
   It would be unwise to penalize such a bishop as "undeveloped." */
float bishopReturn(unsigned char index, GameState* gs)
  {
    float h = 0.0;

    if(isWhite(index, gs))
      {
        if(!whiteCastled(gs) && (index == 5 || index == 2))
          h += BISHOP_UNDEVELOPED_PENALTY;
      }
    else
      {
        if(!blackCastled(gs) && (index == 58 || index == 61))
          h += BISHOP_UNDEVELOPED_PENALTY;
      }

    return h;
  }

/* Bishop trapped by enemy pawns on A2/H2/A7/H7 or on A3/H3/A6/H6 */
float bishopTrapped(unsigned char index, Move* negPawnAttacks, unsigned int negPawnAttacksLen)
  {
    float h = 0.0;
    unsigned int i;

    if(index == 8  || index == 16 || index == 40 || index == 48 ||
       index == 15 || index == 23 || index == 47 || index == 55 )
      {
        i = 0;
        while(i < negPawnAttacksLen && negPawnAttacks[i].to != index)
          i++;
        if(i < negPawnAttacksLen)
          h += BISHOP_PAWN_TRAPPED_CORNER_PENALTY;
      }

    return h;
  }

/* Marginal bonus for a knight defended by a pawn--yes, as SCORED BY A BISHOP */
float bishopKnightDefense(unsigned char index,
                          unsigned char* posTeam, unsigned char posLen,
                          Move* posPawnCoverage, unsigned int posPawnCoverageLen,
                          GameState* gs)
  {
    float h = 0.0;
    unsigned char i;
    unsigned int j;

    for(i = 0; i < posLen; i++)
      {
        if(isKnight(posTeam[i], gs))
          {
            j = 0;
            while(j < posPawnCoverageLen && posPawnCoverage[j].to != posTeam[i])
              j++;

            if(j < posPawnCoverageLen)
              h += BISHOP_KNIGHT_DEFENSE_BONUS;
          }
      }

    return h;
  }

/* Penalty for an undefended minor piece */
float bishopUndefended(unsigned char index, Move* posCoverage, unsigned int posCoverageLen)
  {
    unsigned int i = 0;

    while(i < posCoverageLen && posCoverage[i].to != index)
      i++;

    if(i == posCoverageLen)
      return UNDEFENDED_MINOR_PIECE_PENALTY;

    return 0.0;
  }

/*  Rook Evaluations  *****************************************************************************/
/* Award points and issue penalties for individual rook attributes. */
float rookEval(unsigned char index,
               unsigned char* posTeam, unsigned char posLen,
               Move* posCoverage, unsigned int posCoverageLen,
               unsigned char* negTeam, unsigned char negLen,
               GameState* gs)
  {
    float h = 0.0;

    h += rookIncreasePawnVal(gs);
    h += rookOpenFile(index, gs);
    h += rookHighRank(index, gs);
    h += Tarrasch(index, posTeam, posLen, negTeam, negLen, gs);
    h += rookTrappedUncastled(index, gs);
    h += rookEnemyQueen(index, gs);
    h += rookMutualDefense(index, posTeam, posLen, posCoverage, posCoverageLen, gs);

    return h;
  }

/* Increasing value as pawns disappear */
float rookIncreasePawnVal(GameState* gs)
  {
    unsigned char wP = 0, bP = 0;
    unsigned char i;
    for(i = 0; i < _NONE; i++)
      {
        if(isPawn(i, gs))
          {
            if(isWhite(i, gs))
              wP++;
            else
              bP++;
          }
      }

    return (16.0 - (float)wP - (float)bP) * ROOK_PAWN_INCREASE;
  }

/* Rook on open file */
float rookOpenFile(unsigned char index, GameState* gs)
  {
    if(isOpenFile(index, gs))
      return ROOK_OPEN_FILE_BONUS;
    return 0.0;
  }

/* Rook on 7th (possibly also 8th) rank */
float rookHighRank(unsigned char index, GameState* gs)
  {
    float h = 0.0;

    if(isWhite(index, gs))
      {
        if(row(index) == 6)
          h += ROOK_7TH_RANK_BONUS;
        else if(row(index) == 7)
          h += ROOK_8TH_RANK_BONUS;
      }
    else
      {
        if(row(index) == 1)
          h += ROOK_7TH_RANK_BONUS;
        else if(row(index) == 0)
          h += ROOK_8TH_RANK_BONUS;
      }

    return h;
  }

/* Tarrasch Rule:
   The idea behind the guideline is that
   (1) if a player's rook is behind his passed pawn, the rook protects it as it advances, and
   (2) if it is behind an opponent's passed pawn, the pawn cannot advance unless it is protected along its way. */
float Tarrasch(unsigned char index,
               unsigned char* posTeam, unsigned char posLen,
               unsigned char* negTeam, unsigned char negLen,
               GameState* gs)
  {
    float h = 0.0;
    unsigned char mostAdvPawnRow;
    unsigned char column[8];
    unsigned char i = 0, j = 0;

    while( i < posLen && !(isPawn(posTeam[i], gs) && sameSide(index, posTeam[i], gs)) )
      i++;

    while( j < negLen && !(isPawn(negTeam[i], gs) && opposed(index, negTeam[j], gs)) )
      j++;

    if(isSemiOpenFile(index, gs) && i < posLen)                     //  Rook is on semi-open file and an ALLY pawn was found on the same file.
      {
        if(isWhite(index, gs))
          {
            getCol(index, column);
            mostAdvPawnRow = 0;
            for(i = 0; i < 8; i++)
              {
                if(isPawn(column[i], gs) && isWhite(column[i], gs) && row(column[i]) > mostAdvPawnRow)
                  mostAdvPawnRow = row(column[i]);
              }
                                                                    //  Ally Pawn is ahead of Rook and is a passed Pawn.
            if(mostAdvPawnRow > row(index) && isPassedPawn(col(index), mostAdvPawnRow, gs))
              h += ROOK_TARRASCH_BONUS;
          }
        else
          {
            getCol(index, column);
            mostAdvPawnRow = 7;
            for(i = 0; i < 8; i++)
              {
                if(isPawn(column[i], gs) && isBlack(column[i], gs) && row(column[i]) < mostAdvPawnRow)
                  mostAdvPawnRow = row(column[i]);
              }
                                                                    //  Ally Pawn is ahead of Rook and is a passed Pawn.
            if(mostAdvPawnRow < row(index) && isPassedPawn(col(index), mostAdvPawnRow, gs))
              h += ROOK_TARRASCH_BONUS;
          }
      }
    else if(isSemiOpenFile(index, gs) && j < negLen)                //  Rook is on semi-open file and an ENEMY pawn was found on the same file.
      {
        if(isWhite(index, gs))
          {
            getCol(index, column);
            mostAdvPawnRow = 7;
            for(i = 0; i < 8; i++)
              {
                if(isPawn(column[i], gs) && isBlack(column[i], gs) && row(column[i]) < mostAdvPawnRow)
                  mostAdvPawnRow = row(column[i]);
              }
                                                                    //  Ally Pawn is ahead of Rook and is a passed Pawn.
            if(mostAdvPawnRow < row(index) && isPassedPawn(col(index), mostAdvPawnRow, gs))
              h += ROOK_TARRASCH_BONUS;
          }
        else
          {
            getCol(index, column);
            mostAdvPawnRow = 0;
            for(i = 0; i < 8; i++)
              {
                if(isPawn(column[i], gs) && isWhite(column[i], gs) && row(column[i]) > mostAdvPawnRow)
                  mostAdvPawnRow = row(column[i]);
              }
                                                                    //  Ally Pawn is ahead of Rook and is a passed Pawn.
            if(mostAdvPawnRow > row(index) && isPassedPawn(col(index), mostAdvPawnRow, gs))
              h += ROOK_TARRASCH_BONUS;
          }
      }

    return h;
  }

/* Penalty for rook blocked by uncastled king */
float rookTrappedUncastled(unsigned char index, GameState* gs)
  {
    if(isWhite(index, gs) && !whiteCastled(gs) && (index == 0 || index == 7))
      return ROOK_BEHIND_UNCASTLED_KING_PENALTY;
    else if(isBlack(index, gs) && !blackCastled(gs) && (index == 56 || index == 63))
      return ROOK_BEHIND_UNCASTLED_KING_PENALTY;
    return 0.0;
  }

/* Small bonus for a rook with enemy queen on the same file (doesn't matter open or not) */
float rookEnemyQueen(unsigned char index, GameState* gs)
  {
    unsigned char i = 0;
    unsigned char column[8];

    getCol(index, column);
    if(isWhite(index, gs))
      {
        while(i < 8 && gs->board[column[i]] != 'q')
          i++;
      }
    else
      {
        while(i < 8 && gs->board[column[i]] != 'Q')
          i++;
      }

    if(i < 8)
      return ROOK_ENEMY_QUEEN_FILE_BONUS;
    return 0.0;
  }

/* Rooks defending each other */
float rookMutualDefense(unsigned char index,
                        unsigned char* posTeam, unsigned char posLen,
                        Move* posCoverage, unsigned int posCoverageLen,
                        GameState* gs)
  {
    float h = 0.0;
    unsigned char i = 0;
    unsigned char rCtr = 0;

    while(i < posLen)
      {
        if(isRook(posTeam[i], gs))
          rCtr++;
        i++;
      }

    if(rCtr > 1)
      {
        rCtr = 0;
        for(i = 0; i < posCoverageLen; i++)
          {
            if(isRook(posCoverage[i].from, gs) && isRook(posCoverage[i].to, gs))
              h += ROOK_MUTUAL_DEFENSE_BONUS;
          }
      }
    return h;
  }

/*  Queen Evaluations  ****************************************************************************/
/* Award points and issue penalties for queen attributes. */
float queenEval(unsigned char index,
                unsigned char* posTeam, unsigned char posLen,
                Move* posMoves, unsigned int posMovesLen,
                Move* posXRay, unsigned int posXRayLen,
                GameState* gs)
  {
    float h = 0.0;

    h += queenEarlyDevelopment(index,
                               posTeam, posLen,
                               posMoves, posMovesLen, gs);
    h += queenKingTropism(index,
                          posXRay, posXRayLen,
                          gs);
    return h;
  }

/* Penalty for early development:
   1. penalize queen if it is on file a, b, g or h during development (requires working def. of "development")
   2. penalize queen for distance from its minor pieces (bishops and knights)
   The idea is that the queen supported by a minor piece can be very powerful,
   but the queen alone makes only shallow threats.
   Why don't we instead reward the queen for attacking squares also attacked by minor pieces?
*/
float queenEarlyDevelopment(unsigned char index,
                            unsigned char* posTeam, unsigned char posLen,
                            Move* posMoves, unsigned int posMovesLen,
                            GameState* gs)
  {
    float h = 0.0;
    Move queenAttacks[64];                                          //  At most, a queen may have 27 moves, (but what if we have more than one queen?)
    unsigned char queenAttacksLen = 0;
    Move bishopAttacks[64];                                         //  At most, a bishop may have 13 moves, (but what if we have more than two bishops?)
    unsigned char bishopAttacksLen = 0;
    Move knightAttacks[64];                                         //  At most, a knight may have 8 moves, (but what if we have more than two knights?)
    unsigned char knightAttacksLen = 0;
    unsigned int i;
    unsigned char j, k;

    for(i = 0; i < posMovesLen; i++)                                //  Count.
      {
        if(isQueen(posMoves[i].from, gs))
          {
            queenAttacks[queenAttacksLen].from = posMoves[i].from;
            queenAttacks[queenAttacksLen].to = posMoves[i].to;
            queenAttacks[queenAttacksLen].promo = _NO_PROMO;
            queenAttacksLen++;
          }
        else if(isBishop(posMoves[i].from, gs))
          {
            bishopAttacks[bishopAttacksLen].from = posMoves[i].from;
            bishopAttacks[bishopAttacksLen].to = posMoves[i].to;
            bishopAttacks[bishopAttacksLen].promo = _NO_PROMO;
            bishopAttacksLen++;
          }
        else if(isKnight(posMoves[i].from, gs))
          {
            knightAttacks[knightAttacksLen].from = posMoves[i].from;
            knightAttacks[knightAttacksLen].to = posMoves[i].to;
            knightAttacks[knightAttacksLen].promo = _NO_PROMO;
            knightAttacksLen++;
          }
      }

    for(i = 0; i < queenAttacksLen; i++)
      {
        j = 0;
        while(j < bishopAttacksLen && bishopAttacks[j].to != queenAttacks[i].to)
          j++;

        k = 0;
        while(k < knightAttacksLen && knightAttacks[k].to != queenAttacks[i].to)
          k++;

        if(j < bishopAttacksLen || k < knightAttacksLen)
          h += QUEEN_OVERLAPPING_ATTACK_BONUS;
      }

    return h;
  }

float queenKingTropism(unsigned char index, Move* posXRay, unsigned int posXRayLen, GameState* gs)
  {
    float h = 0.0;
    unsigned char i = 0;
    unsigned char enemyKingMap[_NONE];

    if(isWhite(index, gs))
      {
        while(i < _NONE && gs->board[i] != 'k')
          i++;
      }
    else
      {
        while(i < _NONE && gs->board[i] != 'K')
          i++;
      }
    if(i == _NONE)                                                  //  This should never happen because Kings are always on board!
      return 0.0;

    bfs(enemyKingMap, i, 16, 4);

    for(i = 0; i < posXRayLen; i++)
      {
        if(posXRay[i].from == index)
          h += (float)enemyKingMap[ posXRay[i].to ] * QUEEN_KING_XRAY_BONUS;
      }

    return h;
  }

/* Maps an 8 x 8 point-board around the given 'start' index.
   The start index is set to 'val,' and each successive orbit is decremented by 'grain' until == 0
     1  1  1  1  1  1  0  0
     2  2  2  2  2  1  0  0
     2  3  3  3  2  1  0  0
     2  3 [4] 3  2  1  0  0
     2  3  3  3  2  1  0  0
     2  2  2  2  2  1  0  0
     1  1  1  1  1  1  0  0
     0  0  0  0  0  0  0  0  */
unsigned char bfs(unsigned char* map, unsigned char start, unsigned char val, unsigned char grain)
  {
    unsigned char queue[_NONE];                                     //  Allocate this all at once; we'll never need more than 64.
    unsigned char qLen = 0;
    bool visited[_NONE];                                            //  Allocate this all at once.

    unsigned char nSq, nVal, newVal;
    unsigned char i;
                                                                    //  _NONE is size of board.
    for(i = 0; i < _NONE; i++)                                      //  Initialize arrays.
      {
        queue[i] = _NONE;                                           //  Fill queue with NONEs.
        visited[i] = false;                                         //  Mark everything as unvisited.
        map[i] = 0;                                                 //  Fill map with zero(e)s.
      }

    queue[0] = start;                                               //  "Enqueue" the starting point.
    map[start] = val;                                               //  Start gets value.
    qLen++;                                                         //  Grow the queue by one.

    while(qLen > 0)
      {
        nSq = queue[0];                                             //  "Pop left".
        nVal = map[nSq];                                            //  Take the value of this index in the map.
        qLen--;                                                     //  Shrink queue.

        for(i = 0; i < _NONE - 1; i++)                              //  Shift everything down.
          queue[i] = queue[i + 1];
        queue[_NONE - 1] = _NONE;
                                                                    //  Have we visited this square already?
        if(!visited[nSq] && nVal > 0)                               //  Do we have any more mileage?
          {
            visited[nSq] = true;                                    //  Mark as visited.
            if(nVal > grain)                                        //  What is new value, given grain?
              newVal = nVal - grain;
            else
              newVal = 0;

            if(u(nSq) < _NONE && !visited[u(nSq)])                  //  "Enqueue" UP.
              {
                i = 0;
                while(i < qLen && queue[i] != u(nSq))               //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = u(nSq);
                    map[ u(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(ur(nSq) < _NONE && !visited[ur(nSq)])                //  "Enqueue" UP-RIGHT.
              {
                i = 0;
                while(i < qLen && queue[i] != ur(nSq))              //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = ur(nSq);
                    map[ ur(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(r(nSq) < _NONE && !visited[r(nSq)])                  //  "Enqueue" RIGHT.
              {
                i = 0;
                while(i < qLen && queue[i] != r(nSq))               //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = r(nSq);
                    map[ r(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(dr(nSq) < _NONE && !visited[dr(nSq)])                //  "Enqueue" DOWN-RIGHT.
              {
                i = 0;
                while(i < qLen && queue[i] != dr(nSq))              //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = dr(nSq);
                    map[ dr(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(d(nSq) < _NONE && !visited[d(nSq)])                  //  "Enqueue" DOWN.
              {
                i = 0;
                while(i < qLen && queue[i] != d(nSq))               //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = d(nSq);
                    map[ d(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(dl(nSq) < _NONE && !visited[dl(nSq)])                //  "Enqueue" DOWN-LEFT.
              {
                i = 0;
                while(i < qLen && queue[i] != dl(nSq))              //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = dl(nSq);
                    map[ dl(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(l(nSq) < _NONE && !visited[l(nSq)])                  //  "Enqueue" LEFT.
              {
                i = 0;
                while(i < qLen && queue[i] != l(nSq))               //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = l(nSq);
                    map[ l(nSq) ] = newVal;
                    qLen++;
                  }
              }
            if(ul(nSq) < _NONE && !visited[ul(nSq)])                //  "Enqueue" UP-LEFT.
              {
                i = 0;
                while(i < qLen && queue[i] != ul(nSq))              //  Do not enqueue duplicates.
                  i++;
                if(i == qLen)
                  {
                    queue[qLen] = ul(nSq);
                    map[ ul(nSq) ] = newVal;
                    qLen++;
                  }
              }
          }
      }

    return _NONE;
  }

/*  King Evaluations  ****************************************************************************/
/* Award points and issue penalties for king attributes. */
float kingEval(unsigned char index,
               unsigned char* negTeam, unsigned char negLen,
               Move* negMoves, unsigned int negMovesLen,
               GameState* gs)
  {
    float h = 0.0;
    unsigned char ph = phase(gs);
    unsigned char r;
    unsigned char c;

    h += kingCastlingRights(index, gs);                             //  Castling
    if(ph == MIDDLE_GAME)                                           //  Safety in middle-game
      {
        h += kingPawnShield(index, gs);
        h += kingPawnStorm(index, negTeam, negLen, gs);
        h += kingTropism(index, negTeam, negLen, gs);
        h += kingZoneAttacks(index, negMoves, negMovesLen, gs);
      }
    else if(ph == END_GAME)                                         //  Centrality in endgame
      {
        r = row(index);
        c = col(index);

        if(c > 1 && c < 6)
          h += KING_ENDGAME_CENTRALITY;
        if(r > 1 && r < 6)
          h += KING_ENDGAME_CENTRALITY;
      }

    return h;
  }

/* Castling rights */
float kingCastlingRights(unsigned char index, GameState* gs)
  {
    float h = 0.0;

    if(isWhite(index, gs))
      {
        if(whiteCastled(gs))
          h += KING_CASTLED_BONUS;
        else if( !(whiteKingsidePrivilege(gs) || whiteQueensidePrivilege(gs)) )
          h += KING_LOST_CASTLE_RIGHTS_PENALTY;
      }
    else
      {
        if(blackCastled(gs))
          h += KING_CASTLED_BONUS;
        else if( !(blackKingsidePrivilege(gs) || blackQueensidePrivilege(gs)) )
          h += KING_LOST_CASTLE_RIGHTS_PENALTY;
      }

    return h;
  }

/* Pawn Shield: defined as proximity of Pawns forward of King and the absence of an open file next
                to the King */
float kingPawnShield(unsigned char index, GameState* gs)
  {
    float h = 0.0;
    bool awayFromOpenFile = false;

    if(col(index) == 0)                                             //  King to extreme left: check right
      awayFromOpenFile = !isOpenFile(1, gs);
    else if(col(index) == 7)                                        //  King to extreme right: check left
      awayFromOpenFile = !isOpenFile(6, gs);
    else                                                            //  King elsewhere: check left and right
      awayFromOpenFile = !(isOpenFile(col(index) - 1, gs) || isOpenFile(col(index) + 1, gs));

    if(awayFromOpenFile)                                            //  No arrangements are rewarded unless King is away
      {                                                             //  from an open file
        if(isWhite(index, gs))
          {
            if(u(index) < _NONE)                                    //  Immediately adjacent to King
              {
                if(isWhite(u(index), gs) && isPawn(u(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }
            if(ul(index) < _NONE)
              {
                if(isWhite(ul(index), gs) && isPawn(ul(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }
            if(ur(index) < _NONE)
              {
                if(isWhite(ur(index), gs) && isPawn(ur(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }

            if(u(u(index)) < _NONE)                                 //  One square distant of King
              {
                if(isWhite(u(u(index)), gs) && isPawn(u(u(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
            if(u(ul(index)) < _NONE)
              {
                if(isWhite(u(ul(index)), gs) && isPawn(u(ul(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
            if(u(ur(index)) < _NONE)
              {
                if(isWhite(u(ur(index)), gs) && isPawn(u(ur(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
          }
        else
          {
            if(d(index) < _NONE)                                    //  Immediately adjacent to King
              {
                if(isBlack(d(index), gs) && isPawn(d(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }
            if(dl(index) < _NONE)
              {
                if(isBlack(dl(index), gs) && isPawn(dl(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }
            if(dr(index) < _NONE)
              {
                if(isBlack(dr(index), gs) && isPawn(dr(index), gs))
                  h += KING_PAWN_SHIELD_IMMEDIATE;
              }

            if(d(d(index)) < _NONE)                                 //  One square distant of King
              {
                if(isBlack(d(d(index)), gs) && isPawn(d(d(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
            if(d(dl(index)) < _NONE)
              {
                if(isBlack(d(dl(index)), gs) && isPawn(d(dl(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
            if(d(dr(index)) < _NONE)
              {
                if(isBlack(d(dr(index)), gs) && isPawn(d(dr(index)), gs))
                  h += KING_PAWN_SHIELD_ONE_DISTANT;
              }
          }
      }

    return h;
  }

/* Pawn Storm: If the enemy pawns are near to the king, there might be a threat of opening a file,
               even if the pawn shield is intact. Penalties for storming enemy pawns must be lower
               than penalties for (semi)open files, otherwise the pawn storm might backfire, resulting
               in a blockage. */
float kingPawnStorm(unsigned char index,
                    unsigned char* negTeam, unsigned char negLen,
                    GameState* gs)
  {
    float h = 0.0;
    unsigned char i;

    if(col(index) == 0)                                             //  King to extreme left: check right
      {
        for(i = 0; i < negLen; i++)
          {
            if(isPawn(negTeam[i], gs) &&
               (col(negTeam[i]) == 0 || col(negTeam[i]) == 1) &&
               abs(row(negTeam[i]) - row(index)) <= 2)
              h += KING_STORMING_PAWN_PENALTY;
          }
      }
    else if(col(index) == 7)                                        //  King to extreme right: check left
      {
        for(i = 0; i < negLen; i++)
          {
            if(isPawn(negTeam[i], gs) &&
               (col(negTeam[i]) == 6 || col(negTeam[i]) == 7) &&
               abs(row(negTeam[i]) - row(index)) <= 2)
              h += KING_STORMING_PAWN_PENALTY;
          }
      }
    else                                                            //  King elsewhere: check left and right
      {
        for(i = 0; i < negLen; i++)
          {
            if(isPawn(negTeam[i], gs) &&
               (col(negTeam[i]) == col(index) + 1 ||
                col(negTeam[i]) == col(index) - 1 ||
                col(negTeam[i]) == col(index)) &&
               abs(row(negTeam[i]) - row(index)) <= 2)
              h += KING_STORMING_PAWN_PENALTY;
          }
      }

    return h;
  }

/* King Tropism:  a simplified form of king safety evaluation. It takes into account the distance
                  between the King and the attacking pieces, possibly weighted against piece value.
                  For example, one may double the distance value for a queen, and halve it for bishops
                  and rooks. This kind of evaluation acts in a probabilistic way - it is by no means
                  certain that being close to the king helps in attacking it. Nevertheless, using this
                  kind of crude evaluation term increases a probability of building up an attack. */
float kingTropism(unsigned char index,
                  unsigned char* negTeam, unsigned char negLen,
                  GameState* gs)
  {
    float h = 0.0;
    unsigned char i = 0;
    unsigned char kingMap[_NONE];

    bfs(kingMap, index, 7, 1);

    for(i = 0; i < negLen; i++)
      {
        if(isPawn(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_PAWN_WEIGHT;
        else if(isKnight(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_KNIGHT_WEIGHT;
        else if(isBishop(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_BISHOP_WEIGHT;
        else if(isRook(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_ROOK_WEIGHT;
        else if(isQueen(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_QUEEN_WEIGHT;
        else if(isKing(negTeam[i], gs))
          h -= kingMap[ negTeam[i] ] * KING_TROPISM_KING_WEIGHT;
      }

    return h;
  }

/* King Attacks: The zone is defined as squares to which the King can move plus one additional
                 orbit. If a piece attacks the zone, we increase a penalty total according to the
                 type of piece doing the attack. */
float kingZoneAttacks(unsigned char index,
                      Move* negMoves, unsigned int negMovesLen,
                      GameState* gs)
  {
    float h = 0.0;
    unsigned char zone[_NONE];
    unsigned int i;

    bfs(zone, index, 3, 1);                                         //  0  0  0  0  0  0  0  0
                                                                    //  1  1  1  1  1  0  0  0
                                                                    //  1  2  2  2  1  0  0  0
                                                                    //  1  2 [3] 2  1  0  0  0
                                                                    //  1  2  2  2  1  0  0  0
                                                                    //  1  1  1  1  1  0  0  0
                                                                    //  0  0  0  0  0  0  0  0
                                                                    //  0  0  0  0  0  0  0  0
    for(i = 0; i < negMovesLen; i++)                                //  Check all enemy attacks.
      {
        if(isPawn(negMoves[i].from, gs))                            //  A Pawn is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_PAWN * (float)(zone[ negMoves[i].to ]);
        else if(isKnight(negMoves[i].from, gs))                     //  A Knight is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_KNIGHT * (float)(zone[ negMoves[i].to ]);
        else if(isBishop(negMoves[i].from, gs))                     //  A Bishop is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_BISHOP * (float)(zone[ negMoves[i].to ]);
        else if(isRook(negMoves[i].from, gs))                       //  A Rook is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_ROOK * (float)(zone[ negMoves[i].to ]);
        else if(isQueen(negMoves[i].from, gs))                      //  A Queen is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_QUEEN * (float)(zone[ negMoves[i].to ]);
        else                                                        //  A King is attacking the King Zone (or zero).
          h -= KING_ZONE_ATTACKED_BY_KING * (float)(zone[ negMoves[i].to ]);
      }

    return h;
  }

/**************************************************************************************************
 Center Control
 Scores moves and attacks according to the center control map of values. */
float centercontrol(bool white, Move* posMoves, unsigned int posMovesLen, Move* posPawnAttacks, unsigned int posPawnAttacksLen)
  {
    float h = 0.0;
    unsigned char centerControlMap[_NONE];
    unsigned char i;

    buildCenterMap(white, centerControlMap);                        //  Fill in map.

    for(i = 0; i < posMovesLen; i++)                                //  Sum up moves.
      h += (float)centerControlMap[ posMoves[i].to ];
    for(i = 0; i < posPawnAttacksLen; i++)                          //  Sum up pawn attacks (counts twice if among moves).
      h += (float)centerControlMap[ posPawnAttacks[i].to ];

    return h;
  }

/*  7 | 7 8 8 8 8 8 8 7
    6 | 7 8 9 9 9 9 8 7
    5 | 7 8 8 9 9 8 8 7
    4 | 6 7 8 9 9 8 7 6
    3 | 5 6 7 8 8 7 6 5
    2 | 4 5 6 7 7 6 5 4
    1 | 2 3 5 5 5 5 3 2
    0 | 1 2 3 4 4 3 2 1
      +----------------
        A B C D E F G H  */
void buildCenterMap(bool white, unsigned char* m)
  {
    if(white)
      {
        m[0]  = 1;  m[1]  = 2;  m[2]  = 3;  m[3]  = 4;  m[4]  = 4;  m[5]  = 3;  m[6]  = 2;  m[7]  = 1;
        m[8]  = 2;  m[9]  = 3;  m[10] = 5;  m[11] = 5;  m[12] = 5;  m[13] = 5;  m[14] = 3;  m[15] = 2;
        m[16] = 4;  m[17] = 5;  m[18] = 6;  m[19] = 7;  m[20] = 7;  m[21] = 6;  m[22] = 5;  m[23] = 4;
        m[24] = 5;  m[25] = 6;  m[26] = 7;  m[27] = 8;  m[28] = 8;  m[29] = 7;  m[30] = 6;  m[31] = 5;
        m[32] = 6;  m[33] = 7;  m[34] = 8;  m[35] = 9;  m[36] = 9;  m[37] = 8;  m[38] = 7;  m[39] = 6;
        m[40] = 7;  m[41] = 8;  m[42] = 8;  m[43] = 9;  m[44] = 9;  m[45] = 8;  m[46] = 8;  m[47] = 7;
        m[48] = 7;  m[49] = 8;  m[50] = 9;  m[51] = 9;  m[52] = 9;  m[53] = 9;  m[54] = 8;  m[55] = 7;
        m[56] = 7;  m[57] = 8;  m[58] = 8;  m[59] = 8;  m[60] = 8;  m[61] = 8;  m[62] = 8;  m[63] = 7;
      }
    else
      {
        m[63] = 1;  m[62] = 2;  m[61] = 3;  m[60] = 4;  m[59] = 4;  m[58] = 3;  m[57] = 2;  m[56] = 1;
        m[55] = 2;  m[54] = 3;  m[53] = 5;  m[52] = 5;  m[51] = 5;  m[50] = 5;  m[49] = 3;  m[48] = 2;
        m[47] = 4;  m[46] = 5;  m[45] = 6;  m[44] = 7;  m[43] = 7;  m[42] = 6;  m[41] = 5;  m[40] = 4;
        m[39] = 5;  m[38] = 6;  m[37] = 7;  m[36] = 8;  m[35] = 8;  m[34] = 7;  m[33] = 6;  m[32] = 5;
        m[31] = 6;  m[30] = 7;  m[29] = 8;  m[28] = 9;  m[27] = 9;  m[26] = 8;  m[25] = 7;  m[24] = 6;
        m[23] = 7;  m[22] = 8;  m[21] = 8;  m[20] = 9;  m[19] = 9;  m[18] = 8;  m[17] = 8;  m[16] = 7;
        m[15] = 7;  m[14] = 8;  m[13] = 9;  m[12] = 9;  m[11] = 9;  m[10] = 9;  m[9]  = 8;  m[8]  = 7;
        m[7]  = 7;  m[6]  = 8;  m[5]  = 8;  m[4]  = 8;  m[3]  = 8;  m[2]  = 8;  m[1]  = 8;  m[0]  = 7;
      }
    return;
  }

/**************************************************************************************************
 Vulnerability
 Pieces held en prise by a less valuable piece are effectively dead.
 WHOSE vulnerability? The OPPOSITE of the GIVEN team! */
float vulnerability(Move* posMoves, unsigned int posMovesLen, GameState* gs)
  {
    float h = 0.0;
    unsigned int i;
    float attackerVal;                                              //  Material value of attacker
    float targetedVal;                                              //  Material value of the target

    for(i = 0; i < posMovesLen; i++)                                //  Check every enemy attack
      {
        if(!isEmpty(posMoves[i].to, gs))                            //  If target is occupied
          {
            attackerVal = materialLookup(posMoves[i].from, gs);
            targetedVal = materialLookup(posMoves[i].to, gs);
            if(targetedVal >= attackerVal)                          //  If the en prise piece is AS or
              h += targetedVal * VULNERABLE_DISCOUNT;               //  MORE valuable, consider it (slightly) lost.
          }
      }

    return h;
  }

/**************************************************************************************************
 Trapped Pieces
   Define a trapped piece as any non-pawn, non-king for which some portion (TRAPPED_PIECE_RATIO)
   of its available moves are under attack.
   Example:   .  k {.} n  .  b {.} .
              .  p  .  . [n] p  p  B
              .  . {.} .  .  . {.} .
              .  .  Q {P} R {.} .  .
              P  .  P  .  .  .  .  .
              .  Q  N  P  .  .  P  b
              .  .  .  .  .  P  .  P
              R  .  B  .  .  .  K  .
            Piece at [52] (black knight in square brackets, E7 = 52) means moveC[52] = 6 and attackC[52] = 6.  */
float trapped(Move* posMoves, unsigned int posMovesLen,
              Move* negMoves, unsigned int negMovesLen,
              Move* negPawnAttacks, unsigned int negPawnAtacksLen,
              Move* negCoverage, unsigned int negCoverageLen, GameState* gs)
  {
    float h = 0.0;                                                  //  Trapped piece score to return.
    unsigned char moveC[_NONE];                                     //  Count moves for ally at [i].
    unsigned char attackC[_NONE];                                   //  Count squares under attack to which [i] could move.
    unsigned int i, j;

    for(i = 0; i < _NONE; i++)                                      //  Blank out counters
      {
        moveC[i] = 0;
        attackC[i] = 0;
      }

    for(i = 0; i < posMovesLen; i++)                                //  For all moves...
      {                                                             //  if it's neither a pawn nor the king...
        if(!isPawn(posMoves[i].from, gs) && !isKing(posMoves[i].from, gs))
          {
            moveC[ posMoves[i].from ]++;                            //  Add to piece at [i]'s number of moves.

            for(j = 0; j < negMovesLen; j++)                        //  For all enemy pieces' attacks that cover the current move...
              {
                                                                    //  (Pawns attack differently, handled below.)
                if( !isPawn(negMoves[j].from, gs) && negMoves[j].to == posMoves[i].to )
                  attackC[ posMoves[i].from ]++;
              }
            for(j = 0; j < negPawnAtacksLen; j++)                   //  For all enemy pawns' attacks that cover the current move...
              {
                if(negPawnAttacks[j].to == posMoves[i].to)
                  attackC[ posMoves[i].from ]++;
              }
            for(j = 0; j < negCoverageLen; j++)                     //  For all enemy coverage, for which gain < loss...
              {
                if( negCoverage[j].to == posMoves[i].to && materialLookup(posMoves[i].to, gs) < materialLookup(posMoves[i].from, gs) )
                  attackC[ posMoves[i].from ]++;
              }
          }
      }

    for(i = 0; i < _NONE; i++)
      {
        if(moveC[i] > 0 && (float)attackC[i] / (float)moveC[i] >= TRAPPED_PIECE_RATIO)
          h += TRAPPED_PIECE_PENALTY;
      }

    return h;
  }

/**************************************************************************************************
 Pins
   Define a pin as an X-ray attack by a sliding piece on any other piece which first meets another
   (typically less valuable) piece. Removing this less valuable piece, then, would expose the
   X-ray target. */
float pins(unsigned char* posTeam, unsigned char posTeamLen,
           Move* negMoves, unsigned int negMovesLen,
           Move* posCoverage, unsigned int posCoverageLen,
           Move* negCoverage, unsigned int negCoverageLen, GameState* gs)
  {
    float h = 0.0;                                                  //  Pins score to return.
    unsigned char set[8];                                           //  Set builder: on an 8 x 8 board, no set will be larger than 8.
    unsigned char setLen;                                           //  Set length.
    unsigned char i, j;                                             //  Count through team, count through sets.

    /* All pins must first meet these two conditions to factor into this evaluation:
       1. Squares along the sliding piece's line of attack must fit this pattern:
          * Zero or more empty squares
          * Enemy piece (the pinned piece)
          * Zero or more empty squares
          * Enemy piece (the shielded piece)
       2. The shielded piece must be the king or a piece of GREATER value than the attacking ally */

    unsigned char pinnedIndex;                                      //  If found, the index of the pinned piece.
    unsigned char shieldedIndex;                                    //  If found, the index of the shielded piece.

    for(i = 0; i < posTeamLen; i++)
      {
        if(isBishop(posTeam[i], gs))
          {
            //////////////////////////////////////////////////////////  Search along UL path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = ulSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along UR path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = urSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along DR path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = drSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along DL path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = dlSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);
          }
        else if(isRook(posTeam[i], gs))
          {
            //////////////////////////////////////////////////////////  Search along U path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = uSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along R path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = rSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along D path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = dSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);

            //////////////////////////////////////////////////////////  Search along L path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = lSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned).
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found.
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded).
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternBishopRook(posTeam[i], pinnedIndex, shieldedIndex,
                                             negMoves, negMovesLen,
                                             posCoverage, posCoverageLen,
                                             negCoverage, negCoverageLen,
                                             gs);
          }
        else if(isQueen(posTeam[i], gs))
          {
            //////////////////////////////////////////////////////////  Search along U path.
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = uSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along R path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = rSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along D path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = dSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along L path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = lSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along UL path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = ulSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along UR path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = urSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along DR path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = drSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);

            //////////////////////////////////////////////////////////  Search along DL path
            pinnedIndex = _NONE;
            shieldedIndex = _NONE;
            setLen = dlSet(posTeam[i], "wb", gs, set);
            j = 0;
            while(j < setLen && isEmpty(set[j], gs))                //  Find first enemy piece (pinned)
              j++;
            if(j < setLen && opposed(posTeam[i], set[j], gs))
              {
                pinnedIndex = set[j];                               //  Pinned piece found
                j++;                                                //  What's next?
                while(j < setLen && isEmpty(set[j], gs))            //  Find next enemy piece (shielded)
                  j++;
                if(j < setLen && opposed(posTeam[i], set[j], gs))   //  Found it: pattern holds!
                  shieldedIndex = set[j];
              }
            if(pinnedIndex != _NONE && shieldedIndex != _NONE)
              h += awardPinPatternQueen(posTeam[i], pinnedIndex, shieldedIndex,
                                        negMoves, negMovesLen,
                                        posCoverage, posCoverageLen,
                                        negCoverage, negCoverageLen,
                                        gs);
          }
      }

    return h;
  }

/* Simplify the pin routine above: assuming we've identified the pin pattern, we now test whether or not
   to award this pin pattern as advantageous. */
float awardPinPatternBishopRook(unsigned char attackerIndex, unsigned char pinnedIndex, unsigned char shieldedIndex,
                                Move* negMoves, unsigned int negMovesLen,
                                Move* posCoverage, unsigned int posCoverageLen,
                                Move* negCoverage, unsigned int negCoverageLen,
                                GameState* gs)
  {
    float h = 0.0;
    unsigned int j, k;

    if(isKing(shieldedIndex, gs))                                   //  King is shielded.
      {
        j = 0;                                                      //  Find out whether ally attacker is itself attacked.
        while(j < negMovesLen && negMoves[j].to != attackerIndex)
          j++;
        if(j == negMovesLen)                                        //  Attacker is not attacked: award absolute pin
          h += ABSOLUTE_PIN_BONUS;                                  //  Absolute Pin: shielded piece is king, and attacker cannot be attacked
                                                                    //  Attacker IS attacked--by a higher-valued pinned piece
        else if(materialLookup(pinnedIndex, gs) >= materialLookup(attackerIndex, gs))
          {
            j = 0;
            while(j < posCoverageLen && posCoverage[j].to != attackerIndex)
              j++;
            if(j < posCoverageLen)                                  //  Attacked by >= piece and covered: award partial pin bonus
              h += PARTIAL_PIN_BONUS;                               //  Partial Pin: shielded piece is king, and attacker CAN be attacked
          }                                                         //               (only reward if our attacker is covered)
      }
                                                                    //  Threaten EQUAL-OR-BETTER (not worth much to pin for a pawn)
    else if(materialLookup(shieldedIndex, gs) >= materialLookup(attackerIndex, gs))
      {
        k = 0;                                                      //  Find out whether the shielded piece is covered
        while(k < negCoverageLen && negCoverage[k].to != shieldedIndex)
          k++;
        if(k == negCoverageLen)                                     //  Shielded piece is NOT covered: proceed
          {
            j = 0;                                                  //  Find out whether ally attacker is itself attacked
            while(j < negMovesLen && negMoves[j].to != attackerIndex)
              j++;
            if(j == negMovesLen)                                    //  Not attacked: award relative pin
              h += RELATIVE_PIN_BONUS;                              //  Relative Pin: shielded piece is not king but is greater than attacker,
                                                                    //  and attacker cannot be attacked
            else if(materialLookup(pinnedIndex, gs) >= materialLookup(attackerIndex, gs))
              {
                j = 0;
                while(j < posCoverageLen && posCoverage[j].to != attackerIndex)
                  j++;
                                                                    //  Attacked by >= piece, covered, and shielded uncovered: award partial-relative pin bonus
                if(j < posCoverageLen)                              //  Relative-Partial Pin: shielded piece is not king but is greater than attacker,
                  h += PARTIAL_RELATIVE_PIN_BONUS;                  //    and attacker CAN be attacked (only reward if our attacker is covered)
              }
          }
      }

    return h;
  }

/* Simplify the pin routine above: assuming we've identified the pin pattern, we now test whether or not
   to award this pin pattern as advantageous. */
float awardPinPatternQueen(unsigned char attackerIndex, unsigned char pinnedIndex, unsigned char shieldedIndex,
                           Move* negMoves, unsigned int negMovesLen,
                           Move* posCoverage, unsigned int posCoverageLen,
                           Move* negCoverage, unsigned int negCoverageLen,
                           GameState* gs)
  {
    float h = 0.0;
    unsigned char j, k;

    if(isKing(shieldedIndex, gs))                                   //  King is shielded
      {
        j = 0;                                                      //  Find out whether ally attacker is itself attacked
        while(j < negMovesLen && negMoves[j].to != attackerIndex)
          j++;
        if(j == negMovesLen)                                        //  Attacker is not attacked: award absolute pin
          h += ABSOLUTE_PIN_BONUS;                                  //  Absolute Pin: shielded piece is king, and attacker cannot be attacked
                                                                    //  Attacker IS attacked--by a higher-valued pinned piece
        else if(materialLookup(pinnedIndex, gs) == materialLookup(attackerIndex, gs))
          {
            j = 0;
            while(j < posCoverageLen && posCoverage[j].to != attackerIndex)
              j++;
            if(j < posCoverageLen)                                  //  Attacked by >= piece and covered: award partial pin bonus
              h += PARTIAL_PIN_BONUS;                               //  Partial Pin: shielded piece is king, and attacker CAN be attacked
          }                                                         //               (only reward if our attacker is covered)
      }
                                                                    //  Threaten EQUAL-OR-BETTER (not worth much to pin for a pawn)
    else if(materialLookup(shieldedIndex, gs) == materialLookup(attackerIndex, gs))
      {
        k = 0;                                                      //  Find out whether the shielded piece is covered
        while(k < negCoverageLen && negCoverage[k].to != shieldedIndex)
          k++;
        if(k == negCoverageLen)                                     //  Shielded piece is NOT covered: proceed
          {
            j = 0;                                                  //  Find out whether ally attacker is itself attacked
            while(j < negMovesLen && negMoves[j].to != attackerIndex)
              j++;
            if(j == negMovesLen)                                    //  Not attacked: award relative pin
              h += RELATIVE_PIN_BONUS;                              //  Relative Pin: shielded piece is not king but is greater than attacker,
                                                                    //  and attacker cannot be attacked
            //  No sense rewarding partial-relative pin because what could justify the loss of a queen?
          }
      }

    return h;
  }

/**************************************************************************************************
 Expanded Game Logic   */

/* An open file is defined as a column with no pawns on it. */
bool isOpenFile(unsigned char index, GameState* gs)
  {
    unsigned char c = 0;
    unsigned char column[8];

    getCol(index, column);
    while(c < 8)
      {
        if(isPawn(column[c], gs))
          return false;
        c++;
      }

    return true;
  }

/* A semi-open file is defined as a column with only pawns of one team on it. */
bool isSemiOpenFile(unsigned char index, GameState* gs)
  {
    unsigned char c = 0;
    bool wPawnFound = false;
    bool bPawnFound = false;
    unsigned char column[8];

    getCol(index, column);
    while(c < 8)
      {
        if(isPawn(column[c], gs))
          {
            if(isWhite(column[c], gs))
              wPawnFound = true;
            else
              bPawnFound = true;
          }
        c++;
      }

    return ((wPawnFound && !bPawnFound) || (!wPawnFound && bPawnFound));
  }

/* Return number of pieces. Actual positions stored in given buffer. */
unsigned char getWhite(GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char i;

    for(i = 0; i < _NONE; i++)
      {
        if(isWhite(i, gs))
          {
            buffer[len] = i;
            len++;
          }
      }

    return len;
  }

/* Return number of pieces. Actual positions stored in given buffer. */
unsigned char getBlack(GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char i;

    for(i = 0; i < _NONE; i++)
      {
        if(isBlack(i, gs))
          {
            buffer[len] = i;
            len++;
          }
      }

    return len;
  }

/* Return number of pawns. Actual positions stored in given buffer. */
unsigned char getPawns(bool gatherWhite, GameState* gs, unsigned char* buffer)
  {
    unsigned char len = 0;
    unsigned char i;

    for(i = 0; i < _NONE; i++)
      {
        if(gatherWhite)
          {
            if(isWhite(i, gs) && isPawn(i, gs))
              {
                buffer[len] = i;
                len++;
              }
          }
        else
          {
            if(isBlack(i, gs) && isPawn(i, gs))
              {
                buffer[len] = i;
                len++;
              }
          }
      }

    return len;
  }

/* Write to given buffer all indices in the column of the given index. */
unsigned char getCol(unsigned char index, unsigned char* c)
  {
    unsigned char i, j, k = 0;

    if(index < _NONE)
      {
        i = col(index);
        j = i + 57;

        while(i < j)
          {
            c[k] = i;
            i += 8;
            k++;
          }
      }
    else
      {
        for(i = 0; i < 8; i++)
          c[i] = _NONE;
      }

    return 8;
  }

/* Write to given buffer all indices in the row of the given index. */
unsigned char getRow(unsigned char index, unsigned char* c)
  {
    unsigned char i, j, k = 0;

    if(index < _NONE)
      {
        i = row(index) * 8;
        j = i + 8;

        while(i < j)
          {
            c[k] = i;
            i++;
            k++;
          }
      }
    else
      {
        for(i = 0; i < 8; i++)
          c[i] = _NONE;
      }

    return 8;
  }

/**************************************************************************************************
 Game Phase
   Working definitions of...
     OPENING GAME: majority of pawns still on their original positions.
                   both queens on board.
                   all rooks on board.
                   prior to both sides castling, both sides giving up castling,
                   or one side castling and the other giving up the right to castle.
                   majority of minor pieces not yet captured
     MIDDLE GAME:  both queens on board.
                   all rooks on board.
     END GAME:     absence of all other conditions.  */
unsigned char phase(GameState* gs)
  {
    unsigned char i;
    unsigned char wQctr = 0;                                        //  Count queens
    unsigned char bQctr = 0;
    unsigned char minorCtr = 0;                                     //  Count minor pieces (Bishops and Knights)
    unsigned char majorCtr = 0;                                     //  Count major pieces (Rooks)
    unsigned char pPosCtr = 0;                                      //  Count all pawns on original positions
    unsigned char pCtr = 0;                                         //  Count all pawns
    bool wStillHasRights, bStillHasRights;

    for(i = 0; i < _NONE; i++)                                      //  Count up pieces
      {
        if(isQueen(i, gs))
          {
            if(isWhite(i, gs))
              wQctr++;
            else
              bQctr++;
          }
        else if(isKnight(i, gs) || isBishop(i, gs))
          minorCtr++;
        else if(isRook(i, gs))
          majorCtr++;
        else if(isPawn(i, gs))
          {
            pCtr++;
            if( (isWhite(i, gs) && row(i) == 1) || (isBlack(i, gs) && row(i) == 6) )
              pPosCtr++;
          }
      }

    wStillHasRights = (whiteKingsidePrivilege(gs) || whiteQueensidePrivilege(gs));
    bStillHasRights = (blackKingsidePrivilege(gs) || blackQueensidePrivilege(gs));

    if( minorCtr >= 4 && majorCtr == 4 && wQctr == 1 && bQctr == 1 && (float)pPosCtr / (float)pCtr >= 0.5 && wStillHasRights && bStillHasRights)
      return OPENING_GAME;
    if(wQctr == 1 && bQctr == 1 && majorCtr == 4)
      return MIDDLE_GAME;

    return END_GAME;
  }

/**************************************************************************************************
 Quiesscence Test
   Define "quiet" positions as ones where NEITHER side has an attack */
bool quiet(GameState* gs)
  {
    Move wmoves[_MAX_MOVES];
    Move bmoves[_MAX_MOVES];
    unsigned int wLen, wAttack;
    unsigned int bLen, bAttack;
    unsigned int i;

    wLen = getMovesForTeam(true, gs, wmoves);
    bLen = getMovesForTeam(false, gs, bmoves);

    for(i = 0; i < wLen; i++)
      {
        if(!isEmpty(wmoves[i].to, gs))
          wAttack++;
      }
    for(i = 0; i < bLen; i++)
      {
        if(!isEmpty(bmoves[i].to, gs))
          bAttack++;
      }

    return (wAttack == 0 && bAttack == 0);
  }

#endif