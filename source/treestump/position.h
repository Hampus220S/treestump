/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef POSITION_H
#define POSITION_H

#include "../treestump.h"

typedef enum 
{
  SIDE_WHITE, 
  SIDE_BLACK, 
  SIDE_BOTH 
} Side;

typedef unsigned char Castle;

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

extern const Castle CASTLE_BLACK_QUEEN;
extern const Castle CASTLE_BLACK_KING;
extern const Castle CASTLE_WHITE_QUEEN;
extern const Castle CASTLE_WHITE_KING;

extern const Castle CASTLE_WHITE;
extern const Castle CASTLE_BLACK;

extern void position_print(Position position);

extern Piece boards_square_piece(U64 boards[12], Square square);

#endif // POSITION_H
