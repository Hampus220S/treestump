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

extern void attacks_init(void);

extern void masks_init(void);

extern void relevant_bits_init(void);


extern U64 attacks_bishop_get(Square square, Position position);

extern U64 attacks_rook_get  (Square square, Position position);

extern U64 attacks_queen_get (Square square, Position position);

extern U64 attacks_king_get  (Square square);

extern U64 attacks_knight_get(Square square);

extern U64 attacks_pawn_get  (Square square, Side side);

extern U64 attacks_get       (Square square, Position position);

#endif // PIECE_H
