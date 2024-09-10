/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-10
 */

#ifndef UCI_INTERN_H
#define UCI_INTERN_H

extern void bitboard_print(U64 bitboard);

extern const char SIDE_SYMBOLS[2];

extern const Piece SYMBOL_PIECES[];

extern const char* SQUARE_STRINGS[BOARD_SQUARES];

extern const char PIECE_SYMBOLS[12];

#endif // UCI_INTERN_H
