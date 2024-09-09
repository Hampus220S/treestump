/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#ifndef PIECE_INTERN_H
#define PIECE_INTERN_H

extern U64 BISHOP_LOOKUP_MASKS [BOARD_SQUARES];
extern U64 ROOK_LOOKUP_MASKS   [BOARD_SQUARES];
extern U64 PAWN_LOOKUP_MASKS[2][BOARD_SQUARES];
extern U64 KNIGHT_LOOKUP_MASKS [BOARD_SQUARES];
extern U64 KING_LOOKUP_MASKS   [BOARD_SQUARES];

extern int BISHOP_RELEVANT_BITS[BOARD_SQUARES];
extern int ROOK_RELEVANT_BITS  [BOARD_SQUARES];

extern const U64 BISHOP_MAGIC_NUMBERS[BOARD_SQUARES];
extern const U64 ROOK_MAGIC_NUMBERS  [BOARD_SQUARES];

extern U64 create_index_cover(int index, U64 attackMask, int bitAmount);

extern U64 calculate_bishop_attacks(Square square, U64 block);

extern U64 calculate_rook_attacks(Square square, U64 block);

#endif // PIECE_INTERN_H
