/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef TREESTUMP_H
#define TREESTUMP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

typedef unsigned char Castle;

typedef int Move;

typedef enum
{
  PIECE_WHITE_PAWN, 
  PIECE_WHITE_KNIGHT, 
  PIECE_WHITE_BISHOP, 
  PIECE_WHITE_ROOK, 
  PIECE_WHITE_QUEEN, 
  PIECE_WHITE_KING,
  PIECE_BLACK_PAWN, 
  PIECE_BLACK_KNIGHT, 
  PIECE_BLACK_BISHOP, 
  PIECE_BLACK_ROOK, 
  PIECE_BLACK_QUEEN, 
  PIECE_BLACK_KING,
  PIECE_NONE
} Piece;

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

typedef struct
{
  Move moves[256];
  int amount;
} MoveArray;

extern const Move MOVE_MASK_SOURCE;
extern const Move MOVE_MASK_TARGET;
extern const Move MOVE_MASK_PIECE;
extern const Move MOVE_MASK_PROMOTE;
extern const Move MOVE_MASK_CAPTURE;
extern const Move MOVE_MASK_DOUBLE;
extern const Move MOVE_MASK_PASSANT;
extern const Move MOVE_MASK_CASTLE;

extern const Move MOVE_SHIFT_SOURCE;
extern const Move MOVE_SHIFT_TARGET;
extern const Move MOVE_SHIFT_PIECE;
extern const Move MOVE_SHIFT_PROMOTE;

extern const Move MOVE_NONE;

// Rename to ex. MOVE_SOURCE_GET / MOVE_SOURCE_SET
#define MOVE_GET_SOURCE(MOVE)   ((MOVE & MOVE_MASK_SOURCE)  >> MOVE_SHIFT_SOURCE)
#define MOVE_GET_TARGET(MOVE)   ((MOVE & MOVE_MASK_TARGET)  >> MOVE_SHIFT_TARGET)
#define MOVE_GET_PIECE(MOVE)    ((MOVE & MOVE_MASK_PIECE)   >> MOVE_SHIFT_PIECE)
#define MOVE_GET_PROMOTE(MOVE)  ((MOVE & MOVE_MASK_PROMOTE) >> MOVE_SHIFT_PROMOTE)

#define MOVE_SET_SOURCE(SOURCE)   ((SOURCE << MOVE_SHIFT_SOURCE)  & MOVE_MASK_SOURCE)
#define MOVE_SET_TARGET(TARGET)   ((TARGET << MOVE_SHIFT_TARGET)  & MOVE_MASK_TARGET)
#define MOVE_SET_PIECE(PIECE)     ((PIECE << MOVE_SHIFT_PIECE)   & MOVE_MASK_PIECE)
#define MOVE_SET_PROMOTE(PROMOTE) ((PROMOTE << MOVE_SHIFT_PROMOTE) & MOVE_MASK_PROMOTE)


#include "treestump/fen-parse.h"
#include "treestump/string-handler.h"
#include "treestump/engine-values.h"


extern void init_board_lookup_lines(void);

extern void init_random_keys(void);

extern void init_piece_lookup_attacks(void);

extern void init_piece_lookup_masks(void);

extern void init_bishop_rook_relevant_bits(void);


extern void parse_uci(Position* position, const char uciString[]);

#endif // TREESTUMP_H
