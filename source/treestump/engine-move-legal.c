/*
 * Check if move is legal in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

extern bool move_castle_is_pseudo_legal(Position position, Move move);

extern bool move_pawn_is_pseudo_legal(Position position, Move move);

/*
 * Check if move is pseudo legal in position
 */
static bool engine_move_is_pseudo_legal(Position position, Move move)
{
  Piece piece = MOVE_PIECE_GET(move);

  if((piece == PIECE_WHITE_PAWN) || (piece == PIECE_BLACK_PAWN))
  {
    return move_pawn_is_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_CASTLE)
  {
    return move_castle_is_pseudo_legal(position, move);
  }
  else return true;
}

/*
 *
 */
bool engine_move_is_legal(Position position, Move move)
{
  if(!engine_move_is_pseudo_legal(position, move)) return false;

  Position moved_position = position;
  move_make(&moved_position, move);

  Square king_square = king_square_get(moved_position, position.side);

  // If the king does not exist, the move can no way be legal
  if(king_square == SQUARE_NONE) return false;

  return !square_is_attacked(moved_position, king_square, moved_position.side);
}
