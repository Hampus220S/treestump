/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef PIECE_H
#define PIECE_H

#include "../treestump.h"

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

// Remove extern from this, and create getter like for attacks
extern U64 BOARD_LOOKUP_LINES[BOARD_SQUARES][BOARD_SQUARES];

extern void init_board_lookup_lines(void);

extern void init_piece_lookup_attacks(void);

extern void init_piece_lookup_masks(void);

extern void init_bishop_rook_relevant_bits(void);


extern U64 bishop_lookup_attacks(Square square, Position position);

extern U64 rook_lookup_attacks  (Square square, Position position);

extern U64 queen_lookup_attacks (Square square, Position position);

extern U64 king_lookup_attacks  (Square square);

extern U64 knight_lookup_attacks(Square square);

extern U64 pawn_lookup_attacks  (Square square, Side side);

extern U64 piece_lookup_attacks (Square square, Position position);

#endif // PIECE_H
