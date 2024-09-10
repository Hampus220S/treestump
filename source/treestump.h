/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef TREESTUMP_H
#define TREESTUMP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <argp.h>
#include <signal.h>
#include <string.h>

#include "debug.h"

typedef unsigned long long  U64;
typedef unsigned long       U32;

#define BOARD_SQUARE_SET(BOARD, SQUARE) ((BOARD) |  (1ULL << (SQUARE)))
#define BOARD_SQUARE_GET(BOARD, SQUARE) ((BOARD) &  (1ULL << (SQUARE)))
#define BOARD_SQUARE_POP(BOARD, SQUARE) ((BOARD) & ~(1ULL << (SQUARE)))

#define BOARD_FILES   8
#define BOARD_RANKS   8
#define BOARD_SQUARES 64

typedef enum
{
  A8, B8, C8, D8, E8, F8, G8, H8,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A1, B1, C1, D1, E1, F1, G1, H1,
  SQUARE_NONE
} Square;

typedef enum 
{
  SIDE_WHITE, 
  SIDE_BLACK, 
  SIDE_BOTH 
} Side;

typedef enum
{
  CASTLE_WHITE_KING  = 0b0001,
  CASTLE_WHITE_QUEEN = 0b0010,
  CASTLE_BLACK_KING  = 0b0100,
  CASTLE_BLACK_QUEEN = 0b1000,
  CASTLE_WHITE       = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN,
  CASTLE_BLACK       = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN
} Castle;

typedef struct
{
  U64     boards[12];
  U64     covers[3];
  Side    side;
  Square  passant;
  Castle  castle;
  int     clock;
  int     turns;
} Position;

#include "treestump/piece.h"
#include "treestump/position.h"
#include "treestump/move.h"
#include "treestump/uci.h"
#include "treestump/engine.h"

extern void random_keys_init(void);

#endif // TREESTUMP_H
