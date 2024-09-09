/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#ifndef PIECE_INTERN_H
#define PIECE_INTERN_H

extern U64 MASKS_BISHOP [BOARD_SQUARES];
extern U64 MASKS_ROOK   [BOARD_SQUARES];
extern U64 MASKS_PAWN[2][BOARD_SQUARES];
extern U64 MASKS_KNIGHT [BOARD_SQUARES];
extern U64 MASKS_KING   [BOARD_SQUARES];

extern int RELEVANT_BITS_BISHOP[BOARD_SQUARES];
extern int RELEVANT_BITS_ROOK  [BOARD_SQUARES];

extern U64 MAGIC_NUMBERS_BISHOP[BOARD_SQUARES];
extern U64 MAGIC_NUMBERS_ROOK  [BOARD_SQUARES];

extern U64 index_cover_create(int index, U64 attackMask, int bitAmount);

extern U64 attacks_bishop_create(Square square, U64 block);

extern U64 attacks_rook_create(Square square, U64 block);

#endif // PIECE_INTERN_H
