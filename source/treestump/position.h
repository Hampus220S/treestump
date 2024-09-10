/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef POSITION_H
#define POSITION_H

#include "../treestump.h"

extern void board_lines_init(void);


extern int    board_bit_amount_get(U64 bitboard);

extern Square board_first_square_get(U64 bitboard);


extern void position_print(Position position);


extern Piece square_piece_get(U64 boards[12], Square square);


extern bool square_is_attacked(Position position, Square square, Side side);

#endif // POSITION_H
