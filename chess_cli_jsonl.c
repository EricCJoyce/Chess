#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gamestate.h"
#include "philadelphia.h"
#include "jsmn.h"

static int tok_eq(const char*, const jsmntok_t*, const char*);
static int tok_copy_string(const char*, const jsmntok_t*, char*, size_t);
static int json_find_top_value(const char*, const jsmntok_t*, int, const char*);
static int hex_nibble(char);
static int hex_decode_67(const char*, uint8_t[67]);
static void hex_encode(const uint8_t*, size_t, char*);
static int move_hex_decode_3(const char*, Move*);
static void move_hex_encode_3(Move*, char[7]);
void serialize(GameState*, unsigned char*);
void deserialize(const uint8_t*, GameState*);

//  echo '{"cmd":"startpos"}' | ./chess_cli
//  echo '{"cmd":"draw","state_hex":"ec0004020305060302040101010101010101000000000000000000000000000000000000000000000000000000000000000007070707070707070a08090b0c09080a00"}' | ./chess_cli
//  echo '{"cmd":"features","state_hex":"ec0004020305060302040101010101010101000000000000000000000000000000000000000000000000000000000000000007070707070707070a08090b0c09080a00"}' | ./chess_cli

//  echo '{"cmd":"legal_moves","state_hex":"ec0004020305060302040101010101010101000000000000000000000000000000000000000000000000000000000000000007070707070707070a08090b0c09080a00"}' | ./chess_cli
//  echo '{"cmd":"apply_move","state_hex":"ec0004020305060302040101010101010101000000000000000000000000000000000000000000000000000000000000000007070707070707070a08090b0c09080a00","move_hex":"0c1c00"}' | ./chess_cli

//  echo '{"cmd":"draw","state_hex":"6c0804020305060302040101010100010101000000000000000000000000010000000000000000000000000000000000000007070707070707070a08090b0c09080a00"}' | ./chess_cli
//  echo '{"cmd":"features","state_hex":"6c0804020305060302040101010100010101000000000000000000000000010000000000000000000000000000000000000007070707070707070a08090b0c09080a00"}' | ./chess_cli

static int handle_request(const char* line);

int main(void)
  {
    char line[4096];

    while(fgets(line, sizeof(line), stdin))
      {
        int rc = handle_request(line);
        fflush(stdout);
        fflush(stderr);
        if(rc != 0)
          return rc;
      }

    return 0;
  }

static int handle_request(const char* line)
  {
    unsigned int i;
    int ntok;
    jsmn_parser p;                                                  //  Parse JSON.
    jsmntok_t toks[256];
    int cmd_i;
    char cmd[64];
    uint8_t state[67];
    uint8_t next_state[67];
    char hex[2 * 67 + 1];
    char state_hex[2 * 67 + 1];
    int st_i;
    GameState gs;

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

    float f[11];

    int is_term;
    int res;

    Move moves[_MAX_MOVES];
    unsigned int movesLen = 0;
    char mh[7];
    int mv_i;

    char move_hex[7];
    Move mv;
    float piece_total;
    float total;

    //////////////////////////////////////////////////////////////////
    jsmn_init(&p);
    ntok = jsmn_parse(&p, line, (int)strlen(line), toks, (int)(sizeof(toks)/sizeof(toks[0])));
    if(ntok < 0)
      {
        fprintf(stderr, "JSON parse error: %d\n", ntok);
        return 3;
      }

    cmd_i = json_find_top_value(line, toks, ntok, "cmd");           //  cmd
    if(cmd_i < 0 || toks[cmd_i].type != JSMN_STRING)
      {
        fprintf(stderr, "Missing/invalid 'cmd'.\n");
        return 4;
      }

    if(!tok_copy_string(line, &toks[cmd_i], cmd, sizeof(cmd)))
      {
        fprintf(stderr, "'cmd' too long.\n");
        return 4;
      }

    if(strcmp(cmd, "startpos") == 0)
      {
                                                                    //  Write the start set.
        state[0] = 236; state[1] = 0; state[2] = 4; state[3] = 2; state[4] = 3; state[5] = 5; state[6] = 6; state[7] = 3; state[8] = 2; state[9] = 4;
        state[10] = 1; state[11] = 1; state[12] = 1; state[13] = 1; state[14] = 1; state[15] = 1; state[16] = 1; state[17] = 1; state[18] = 0; state[19] = 0;
        state[20] = 0; state[21] = 0; state[22] = 0; state[23] = 0; state[24] = 0; state[25] = 0; state[26] = 0; state[27] = 0; state[28] = 0; state[29] = 0;
        state[30] = 0; state[31] = 0; state[32] = 0; state[33] = 0; state[34] = 0; state[35] = 0; state[36] = 0; state[37] = 0; state[38] = 0; state[39] = 0;
        state[40] = 0; state[41] = 0; state[42] = 0; state[43] = 0; state[44] = 0; state[45] = 0; state[46] = 0; state[47] = 0; state[48] = 0; state[49] = 0;
        state[50] = 7; state[51] = 7; state[52] = 7; state[53] = 7; state[54] = 7; state[55] = 7; state[56] = 7; state[57] = 7; state[58] = 10; state[59] = 8;
        state[60] = 9; state[61] = 11; state[62] = 12; state[63] = 9; state[64] = 8; state[65] = 10; state[66] = 0;
        hex_encode(state, 67, hex);
        printf("{\"state_hex\":\"%s\"}\n", hex);
        return 0;
      }

    if(strcmp(cmd, "print_move") == 0)
      {
        mv_i = json_find_top_value(line, toks, ntok, "move_hex");
        if(mv_i < 0 || toks[mv_i].type != JSMN_STRING)
          {
            fprintf(stderr, "Missing/invalid 'move_hex'.\n");
            return 6;
          }

        if(!tok_copy_string(line, &toks[mv_i], move_hex, sizeof(move_hex)))
          {
            fprintf(stderr, "'move_hex' wrong length.\n");
            return 6;
          }

        if(!move_hex_decode_3(move_hex, &mv))
          {
            fprintf(stderr, "Bad hex in 'move_hex'.\n");
            return 6;
          }

        if(mv.promo == _NO_PROMO)
          printf("{\"move_from\":%d, \"move_to\":%d}\n", mv.from, mv.to);
        else
          {
            switch(mv.promo)
              {
                case _PROMO_KNIGHT: printf("{\"move_from\":%d, \"move_to\":%d, \"promo\":\"N\"}\n", mv.from, mv.to);  break;
                case _PROMO_BISHOP: printf("{\"move_from\":%d, \"move_to\":%d, \"promo\":\"B\"}\n", mv.from, mv.to);  break;
                case _PROMO_ROOK:   printf("{\"move_from\":%d, \"move_to\":%d, \"promo\":\"R\"}\n", mv.from, mv.to);  break;
                case _PROMO_QUEEN:  printf("{\"move_from\":%d, \"move_to\":%d, \"promo\":\"Q\"}\n", mv.from, mv.to);  break;
              }
          }

        return 0;
      }

    st_i = json_find_top_value(line, toks, ntok, "state_hex");      //  All other commands require state_hex.
    if(st_i < 0 || toks[st_i].type != JSMN_STRING)
      {
        fprintf(stderr, "Missing/invalid 'state_hex'.\n");
        return 5;
      }

    if(!tok_copy_string(line, &toks[st_i], state_hex, sizeof(state_hex)))
      {
        fprintf(stderr, "'state_hex' wrong length.\n");
        return 5;
      }

    if(!hex_decode_67(state_hex, state))
      {
        fprintf(stderr, "Bad hex in 'state_hex'.\n");
        return 5;
      }

    memset(&gs, 0, sizeof(gs));
    deserialize(state, &gs);

    if(strcmp(cmd, "draw") == 0)
      {
        printf("{");
        printf("\"row_8\":\"");
        for(i = 56; i < _NONE; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_7\":\"");
        for(i = 48; i < 56; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_6\":\"");
        for(i = 40; i < 48; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_5\":\"");
        for(i = 32; i < 40; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_4\":\"");
        for(i = 24; i < 32; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_3\":\"");
        for(i = 16; i < 24; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_2\":\"");
        for(i = 8; i < 16; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        printf("\"row_1\":\"");
        for(i = 0; i < 8; i++)
          {
            switch(gs.board[i])
              {
                case _EMPTY:         printf(".");  break;
                case _WHITE_PAWN:    printf("P");  break;
                case _WHITE_KNIGHT:  printf("N");  break;
                case _WHITE_BISHOP:  printf("B");  break;
                case _WHITE_ROOK:    printf("R");  break;
                case _WHITE_QUEEN:   printf("Q");  break;
                case _WHITE_KING:    printf("K");  break;
                case _BLACK_PAWN:    printf("p");  break;
                case _BLACK_KNIGHT:  printf("n");  break;
                case _BLACK_BISHOP:  printf("b");  break;
                case _BLACK_ROOK:    printf("r");  break;
                case _BLACK_QUEEN:   printf("q");  break;
                case _BLACK_KING:    printf("k");  break;
              }
          }
        printf("\",");

        if(gs.whiteToMove)
          printf("\"white_to_move\":true}\n");
        else
          printf("\"white_to_move\":false}\n");

        return 0;
      }

    if(strcmp(cmd, "features") == 0)
      {
        //////////////////////////////////////////////////////////////  Compute the following only ONCE
        whiteMaterialLength = getWhite(&gs, whiteMaterial);         //  unsigned chars
        blackMaterialLength = getBlack(&gs, blackMaterial);         //  unsigned chars

        whiteMovesLength = getMovesForTeam(true, &gs, whiteMoves);  //  Moves
        blackMovesLength = getMovesForTeam(false, &gs, blackMoves); //  Moves
                                                                    //  Pawn Attack Moves
        whitePawnAttacksLength = getPawnAttacksTeam(true, &gs, whitePawnAttacks);
                                                                    //  Pawn Attack Moves
        blackPawnAttacksLength = getPawnAttacksTeam(false, &gs, blackPawnAttacks);
                                                                    //  Pawn Target Moves
        whitePawnTargetsLength = getPawnTargetsTeam(true, &gs, whitePawnTargets);
                                                                    //  Pawn Target Moves
        blackPawnTargetsLength = getPawnTargetsTeam(false, &gs, blackPawnTargets);

        whiteCoverageLength = getCoverage(true, &gs, whiteCoverage);//  Coverage Moves
        blackCoverageLength = getCoverage(false, &gs, blackCoverage);

        whitePawnCoverageLength = 0;                                //  Coverage only by pawns
        for(i = 0; i < whiteCoverageLength; i++)                    //  Count up pawn coverage.
          {
            if(isPawn(whiteCoverage[i].from, &gs))
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
            if(isPawn(blackCoverage[i].from, &gs))
              {
                blackPawnCoverage[blackPawnCoverageLength].from = blackCoverage[i].from;
                blackPawnCoverage[blackPawnCoverageLength].to = blackCoverage[i].to;
                blackPawnCoverage[blackPawnCoverageLength].promo = blackCoverage[i].promo;
                blackPawnCoverageLength++;
              }
          }

        whiteScopeLength = getScope(true, &gs, whiteScope);         //  Scope
        blackScopeLength = getScope(false, &gs, blackScope);

        whiteXRayLength = getXRay(true, &gs, whiteXRay);            //  X-Ray attacks
        blackXRayLength = getXRay(false, &gs, blackXRay);

        if(gs.whiteToMove)
          {
            f[0] = material(whiteMaterial, whiteMaterialLength, &gs) - material(blackMaterial, blackMaterialLength, &gs);
            f[1] = mobility(whiteMoves, whiteMovesLength, &gs) - mobility(blackMoves, blackMovesLength, &gs);
            f[2] = attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, &gs) - attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, &gs);
            f[3] = coverage(whiteCoverage, whiteCoverageLength, &gs) - coverage(blackCoverage, blackCoverageLength, &gs);
            f[4] = pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, &gs) - pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, &gs);
            f[5] = development(true, &gs) - development(false, &gs);
            f[6] = pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength, blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, &gs) - pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength, whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, &gs);
            f[7] = centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength) - centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength);
            f[8] = vulnerability(whiteMoves, whiteMovesLength, &gs) - vulnerability(blackMoves, blackMovesLength, &gs);
            f[9] = trapped(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, &gs) - trapped(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, &gs);
            f[10] = pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength, whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, &gs) - pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength, blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, &gs);
          }
        else
          {
            f[0] = material(blackMaterial, blackMaterialLength, &gs) - material(whiteMaterial, whiteMaterialLength, &gs);
            f[1] = mobility(blackMoves, blackMovesLength, &gs) - mobility(whiteMoves, whiteMovesLength, &gs);
            f[2] = attacks(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, &gs) - attacks(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, &gs);
            f[3] = coverage(blackCoverage, blackCoverageLength, &gs) - coverage(whiteCoverage, whiteCoverageLength, &gs);
            f[4] = pawnstructure(blackMaterial, blackMaterialLength, blackPawnCoverage, blackPawnCoverageLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, &gs) - pawnstructure(whiteMaterial, whiteMaterialLength, whitePawnCoverage, whitePawnCoverageLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, &gs);
            f[5] = development(false, &gs) - development(true, &gs);
            f[6] = pieceeval(blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackCoverage, blackCoverageLength, blackPawnCoverage, blackPawnCoverageLength, blackPawnTargets, blackPawnTargetsLength, blackScope, blackScopeLength, blackXRay, blackXRayLength, whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, &gs) - pieceeval(whiteMaterial, whiteMaterialLength, whiteMoves, whiteMovesLength, whiteCoverage, whiteCoverageLength, whitePawnCoverage, whitePawnCoverageLength, whitePawnTargets, whitePawnTargetsLength, whiteScope, whiteScopeLength, whiteXRay, whiteXRayLength, blackMaterial, blackMaterialLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, &gs);
            f[7] = centercontrol(false, blackMoves, blackMovesLength, blackPawnAttacks, blackPawnAttacksLength) - centercontrol(true, whiteMoves, whiteMovesLength, whitePawnAttacks, whitePawnAttacksLength);
            f[8] = vulnerability(blackMoves, blackMovesLength, &gs) - vulnerability(whiteMoves, whiteMovesLength, &gs);
            f[9] = trapped(blackMoves, blackMovesLength, whiteMoves, whiteMovesLength, whitePawnTargets, whitePawnTargetsLength, whiteCoverage, whiteCoverageLength, &gs) - trapped(whiteMoves, whiteMovesLength, blackMoves, blackMovesLength, blackPawnTargets, blackPawnTargetsLength, blackCoverage, blackCoverageLength, &gs);
            f[10] = pins(blackMaterial, blackMaterialLength, whiteMoves, whiteMovesLength, blackCoverage, blackCoverageLength, whiteCoverage, whiteCoverageLength, &gs) - pins(whiteMaterial, whiteMaterialLength, blackMoves, blackMovesLength, whiteCoverage, whiteCoverageLength, blackCoverage, blackCoverageLength, &gs);
          }

        printf("{\"features\":[%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g]}\n",
               f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7],f[8],f[9],f[10]);
        return 0;
      }

    if(strcmp(cmd, "terminal") == 0)
      {
        is_term = terminal(&gs) ? 1 : 0;

        if(!is_term)
          {
            printf("{\"terminal\":false}\n");
          }
        else
          {
            res = isWin(&gs);
            if(res == GAME_OVER_STALEMATE)
              printf("{\"terminal\":true,\"result\":\"draw\"}\n");
            else
              printf("{\"terminal\":true,\"result\":\"loss\"}\n");
          }
        return 0;
      }

    if(strcmp(cmd, "legal_moves") == 0)
      {
        movesLen = getMoves(&gs, moves);

        printf("{\"moves_hex\":[");                                 //  Emit JSON.
        for(i = 0; i < movesLen; i++)
          {
            move_hex_encode_3(moves + i, mh);
            printf("\"%s\"%s", mh, (i + 1 < movesLen) ? "," : "");
          }
        printf("]}\n");
        return 0;
      }

    if(strcmp(cmd, "apply_move") == 0)
      {
        mv_i = json_find_top_value(line, toks, ntok, "move_hex");
        if(mv_i < 0 || toks[mv_i].type != JSMN_STRING)
          {
            fprintf(stderr, "Missing/invalid 'move_hex'.\n");
            return 6;
          }

        if(!tok_copy_string(line, &toks[mv_i], move_hex, sizeof(move_hex)))
          {
            fprintf(stderr, "'move_hex' wrong length.\n");
            return 6;
          }

        if(!move_hex_decode_3(move_hex, &mv))
          {
            fprintf(stderr, "Bad hex in 'move_hex'.\n");
            return 6;
          }

        makeMove(&mv, &gs);
        serialize(&gs, next_state);

        hex_encode(next_state, 67, hex);
        printf("{\"state_hex\":\"%s\"}\n", hex);
        return 0;
      }

    if(strcmp(cmd, "phase") == 0)
      {
        piece_total = 0.0;
        for(i = 0; i < _NONE; i++)
          {
            if(isQueen(i, &gs))
              piece_total += 4.0;
            else if(isRook(i, &gs))
              piece_total += 2.0;
            else if(isBishop(i, &gs))
              piece_total += 1.0;
            else if(isKnight(i, &gs))
              piece_total += 1.0;
            else if(isPawn(i, &gs))
              piece_total += 0.25;
          }
        total = piece_total / 28.0;
        total = (total > 1.0) ? 1.0 : (total < 0.0) ? 0.0 : total;

        printf("{\"phase\":%.9g}\n", total);                        //  Emit JSON.
        return 0;
      }

    fprintf(stderr, "Unknown cmd: %s\n", cmd);
    return 7;
  }

//  Compare a token to a literal string (token is not null-terminated).
static int tok_eq(const char* json, const jsmntok_t* tok, const char* s)
  {
    int len = (int)strlen(s);
    int tlen = tok->end - tok->start;
    return (tok->type == JSMN_STRING && tlen == len && strncmp(json + tok->start, s, (size_t)len) == 0);
  }

//  Copy token string into out (null-terminated). Returns 1 on success.
static int tok_copy_string(const char* json, const jsmntok_t* tok, char* out, size_t out_cap)
  {
    int tlen = tok->end - tok->start;
    if((size_t)tlen + 1 > out_cap)
      return 0;
    memcpy(out, json + tok->start, (size_t)tlen);
    out[tlen] = '\0';
    return 1;
  }

//  Find the token index of value for a given key in the top-level object.
//  Returns value token index, or -1 if not found.
static int json_find_top_value(const char* json, const jsmntok_t* toks, int ntok, const char* key)
  {
    if(ntok < 1 || toks[0].type != JSMN_OBJECT)
      return -1;
                                                                    //  jsmn stores object as: { key, value, key, value, ... } in tokens after toks[0]
    int i = 1;
    int pairs = toks[0].size;
    for(int p = 0; p < pairs; p++)
      {
        const jsmntok_t *k = &toks[i];
        const jsmntok_t *v = &toks[i + 1];
        if(tok_eq(json, k, key))
          {
            return i + 1;
          }
                                                                    //  Advance i to next key. BUT: value can be an object/array with nested tokens.
                                                                    //  We need to skip over the entire value subtree.
        i += 2;
                                                                    //  If v is a primitive/string, skipping is already done.
                                                                    //  If v is object/array, skip its nested tokens:
        if(v->type == JSMN_OBJECT || v->type == JSMN_ARRAY)
          {
                                                                    //  Skip over all descendant tokens (simple walker).
            int to_skip = 1;
            while(to_skip > 0 && i < ntok)
              {
                if(toks[i].type == JSMN_OBJECT || toks[i].type == JSMN_ARRAY)
                  {
                    to_skip += toks[i].size;
                  }
                to_skip--;
                i++;
              }
          }
      }
    return -1;
  }

static int hex_nibble(char c)
  {
    if('0' <= c && c <= '9')
      return c - '0';
    c = (char)tolower((unsigned char)c);
    if('a' <= c && c <= 'f')
      return 10 + (c - 'a');
    return -1;
  }

static int hex_decode_67(const char* hex, uint8_t out[67])
  {
    if(!hex)
      return 0;
    if(strlen(hex) != 134)                                          //  Must be exactly 134 hex chars. 134=2*_GAMESTATE_BYTE_SIZE
      return 0;
    for(int i = 0; i < 67; i++)
      {
        int hi = hex_nibble(hex[2 * i]);
        int lo = hex_nibble(hex[2 * i + 1]);
        if(hi < 0 || lo < 0)
          return 0;
        out[i] = (uint8_t)((hi << 4) | lo);
      }
    return 1;
  }

static void hex_encode(const uint8_t* in, size_t n, char* out_hex)
  {
    static const char *digits = "0123456789abcdef";
    for(size_t i = 0; i < n; i++)
      {
        out_hex[2 * i]     = digits[(in[i] >> 4) & 0xF];
        out_hex[2 * i + 1] = digits[in[i] & 0xF];
      }
    out_hex[2 * n] = '\0';
  }

static int move_hex_decode_3(const char *hex, Move* mv)
  {
    uint8_t out_move[3];
    if(!hex)
      return 0;
    if(strlen(hex) != 6)
      return 0;
    for(int i = 0; i < 3; i++)
      {
        int hi = hex_nibble(hex[2*i]);
        int lo = hex_nibble(hex[2*i + 1]);
        if (hi < 0 || lo < 0) return 0;
        out_move[i] = (uint8_t)((hi << 4) | lo);
      }
    mv->from = out_move[0];
    mv->to = out_move[1];
    mv->promo = out_move[2];
    return 1;
  }

static void move_hex_encode_3(Move* mv, char out_hex[7])
  {
    static const char *d = "0123456789abcdef";

    out_hex[0] = d[(mv->from >> 4) & 0xF];
    out_hex[1] = d[mv->from & 0xF];

    out_hex[2] = d[(mv->to >> 4) & 0xF];
    out_hex[3] = d[mv->to & 0xF];

    out_hex[4] = d[(mv->promo >> 4) & 0xF];
    out_hex[5] = d[mv->promo & 0xF];

    out_hex[6] = '\0';
  }

/* Pack a GameState into the unsigned-char buffer. */
void serialize(GameState* gs, unsigned char* buffer)
  {
    unsigned char i = 0, j;
    unsigned char ch;

    //////////////////////////////////////////////////////////////////  (1 byte) Encode side to move and castling data.
    ch = 0;
    if(gs->whiteToMove)                                             //  Set bit: white to move.
      ch |= 128;
    if(gs->whiteKingsidePrivilege)                                  //  Set bit: white's kingside castling privilege.
      ch |= 64;
    if(gs->whiteQueensidePrivilege)                                 //  Set bit: white's queenside castling privilege.
      ch |= 32;
    if(gs->whiteCastled)                                            //  Set bit: white has castled.
      ch |= 16;
    if(gs->blackKingsidePrivilege)                                  //  Set bit: black's kingside castling privilege.
      ch |= 8;
    if(gs->blackQueensidePrivilege)                                 //  Set bit: black's queenside castling privilege.
      ch |= 4;
    if(gs->blackCastled)                                            //  Set bit: black has castled.
      ch |= 2;

    buffer[i++] = ch;                                               //  Write byte.

    //////////////////////////////////////////////////////////////////  (1 byte) Encode previous pawn double move data.
    ch = 0;
    ch |= (gs->previousDoublePawnMove & 128);                       //  Set bit: previous pawn double-move occurred on column A.
    ch |= (gs->previousDoublePawnMove & 64);                        //  Set bit: previous pawn double-move occurred on column B.
    ch |= (gs->previousDoublePawnMove & 32);                        //  Set bit: previous pawn double-move occurred on column C.
    ch |= (gs->previousDoublePawnMove & 16);                        //  Set bit: previous pawn double-move occurred on column D.
    ch |= (gs->previousDoublePawnMove & 8);                         //  Set bit: previous pawn double-move occurred on column E.
    ch |= (gs->previousDoublePawnMove & 4);                         //  Set bit: previous pawn double-move occurred on column F.
    ch |= (gs->previousDoublePawnMove & 2);                         //  Set bit: previous pawn double-move occurred on column G.
    ch |= (gs->previousDoublePawnMove & 1);                         //  Set bit: previous pawn double-move occurred on column H.
    buffer[i++] = ch;                                               //  Write byte.

    //////////////////////////////////////////////////////////////////  (64 bytes) Encode the board.
    for(j = 0; j < _NONE; j++)
      {
        ch = 0;
        if(isWhite(j, gs))
          {
            if(isPawn(j, gs))
              ch = _WHITE_PAWN;
            else if(isKnight(j, gs))
              ch = _WHITE_KNIGHT;
            else if(isBishop(j, gs))
              ch = _WHITE_BISHOP;
            else if(isRook(j, gs))
              ch = _WHITE_ROOK;
            else if(isQueen(j, gs))
              ch = _WHITE_QUEEN;
            else
              ch = _WHITE_KING;
          }
        else if(isBlack(j, gs))
          {
            if(isPawn(j, gs))
              ch = _BLACK_PAWN;
            else if(isKnight(j, gs))
              ch = _BLACK_KNIGHT;
            else if(isBishop(j, gs))
              ch = _BLACK_BISHOP;
            else if(isRook(j, gs))
              ch = _BLACK_ROOK;
            else if(isQueen(j, gs))
              ch = _BLACK_QUEEN;
            else
              ch = _BLACK_KING;
          }

        buffer[i++] = ch;                                           //  Write byte.
      }

    //////////////////////////////////////////////////////////////////  (1 byte) Encode the move counter.
    buffer[i++] = gs->moveCtr;

    return;                                                         //  TOTAL: 67 bytes.
  }

/* Recover a GameState from the unsigned-char buffer". */
void deserialize(const uint8_t* buffer, GameState* gs)
  {
    unsigned char i, j;

    for(i = 0; i < _NONE; i++)                                      //  Fill-in/blank-out.
      gs->board[i] = _EMPTY;
    gs->previousDoublePawnMove = 0;

    //////////////////////////////////////////////////////////////////  (1 byte) Decode side to move and castling data.
    gs->whiteToMove = ((buffer[0] & 128) == 128);                   //  Recover side to move.

    gs->whiteKingsidePrivilege = ((buffer[0] & 64) == 64);          //  Recover white's castling data.
    gs->whiteQueensidePrivilege = ((buffer[0] & 32) == 32);
    gs->whiteCastled = ((buffer[0] & 16) == 16);

    gs->blackKingsidePrivilege = ((buffer[0] & 8) == 8);            //  Recover black's castling data.
    gs->blackQueensidePrivilege = ((buffer[0] & 4) == 4);
    gs->blackCastled = ((buffer[0] & 2) == 2);

    //////////////////////////////////////////////////////////////////  (1 byte) Decode en-passant data.
    gs->previousDoublePawnMove |= (buffer[1] & 128);
    gs->previousDoublePawnMove |= (buffer[1] & 64);
    gs->previousDoublePawnMove |= (buffer[1] & 32);
    gs->previousDoublePawnMove |= (buffer[1] & 16);
    gs->previousDoublePawnMove |= (buffer[1] & 8);
    gs->previousDoublePawnMove |= (buffer[1] & 4);
    gs->previousDoublePawnMove |= (buffer[1] & 2);
    gs->previousDoublePawnMove |= (buffer[1] & 1);

    if(!(gs->previousDoublePawnMove == 128 ||
         gs->previousDoublePawnMove == 64  ||
         gs->previousDoublePawnMove == 32  ||
         gs->previousDoublePawnMove == 16  ||
         gs->previousDoublePawnMove == 8   ||
         gs->previousDoublePawnMove == 4   ||
         gs->previousDoublePawnMove == 2   ||
         gs->previousDoublePawnMove == 1   ||
         gs->previousDoublePawnMove == 0   ))
      gs->previousDoublePawnMove = 0;                               //  "There can be only one!"

    //////////////////////////////////////////////////////////////////  (64 bytes) Decode the board.
    i = 2;
    for(j = 0; j < _NONE; j++)
      {
        if(buffer[i] == _WHITE_PAWN)
          gs->board[j] = _WHITE_PAWN;
        else if(buffer[i] == _WHITE_KNIGHT)
          gs->board[j] = _WHITE_KNIGHT;
        else if(buffer[i] == _WHITE_BISHOP)
          gs->board[j] = _WHITE_BISHOP;
        else if(buffer[i] == _WHITE_ROOK)
          gs->board[j] = _WHITE_ROOK;
        else if(buffer[i] == _WHITE_QUEEN)
          gs->board[j] = _WHITE_QUEEN;
        else if(buffer[i] == _WHITE_KING)
          gs->board[j] = _WHITE_KING;
        else if(buffer[i] == _BLACK_PAWN)
          gs->board[j] = _BLACK_PAWN;
        else if(buffer[i] == _BLACK_KNIGHT)
          gs->board[j] = _BLACK_KNIGHT;
        else if(buffer[i] == _BLACK_BISHOP)
          gs->board[j] = _BLACK_BISHOP;
        else if(buffer[i] == _BLACK_ROOK)
          gs->board[j] = _BLACK_ROOK;
        else if(buffer[i] == _BLACK_QUEEN)
          gs->board[j] = _BLACK_QUEEN;
        else if(buffer[i] == _BLACK_KING)
          gs->board[j] = _BLACK_KING;

        i++;
      }

    //////////////////////////////////////////////////////////////////  (1 byte) Decode the move counter.
    gs->moveCtr = buffer[i++];

    return;                                                         //  TOTAL: 67 bytes.
  }
