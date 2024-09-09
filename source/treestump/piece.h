/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef PIECE_H
#define PIECE_H

#include "../treestump.h"

// Remove extern from this, and create getter like for attacks
extern U64 BOARD_LOOKUP_LINES[BOARD_SQUARES][BOARD_SQUARES];

extern void init_board_lookup_lines(void);

extern void init_piece_lookup_attacks(void);

extern void init_piece_lookup_masks(void);

extern void init_bishop_rook_relevant_bits(void);

#endif // PIECE_H
