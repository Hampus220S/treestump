/*
 * Check if square is attacked
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

/*
 *
 */
static bool square_is_attacked_by_queen(Position position, Square square, Side side)
{
  U64 attacks = attacks_queen_get(square, position);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_QUEEN] :
              position.boards[PIECE_BLACK_QUEEN];

  return (attacks & board);
}

/*
 *
 */
static bool square_is_attacked_by_bishop(Position position, Square square, Side side)
{
  U64 attacks = attacks_bishop_get(square, position);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_BISHOP] :
              position.boards[PIECE_BLACK_BISHOP];

  return (attacks & board);
}

/*
 *
 */
static bool square_is_attacked_by_rook(Position position, Square square, Side side)
{
  U64 attacks = attacks_rook_get(square, position);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_ROOK] :
              position.boards[PIECE_BLACK_ROOK];

  return (attacks & board);
}

/*
 *
 */
static bool square_is_attacked_by_pawn(Position position, Square square, Side side)
{
  U64 attacks = (side == SIDE_WHITE) ?
                attacks_pawn_get(square, SIDE_BLACK) :
                attacks_pawn_get(square, SIDE_WHITE);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_PAWN] :
              position.boards[PIECE_BLACK_PAWN];

  return (attacks & board);
}

/*
 *
 */
static bool square_is_attacked_by_king(Position position, Square square, Side side)
{
  U64 attacks = attacks_king_get(square);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_KING] :
              position.boards[PIECE_BLACK_KING];

  return (attacks & board);
}

/*
 *
 */
static bool square_is_attacked_by_knight(Position position, Square square, Side side)
{
  U64 attacks = attacks_knight_get(square);

  U64 board = (side == SIDE_WHITE) ?
              position.boards[PIECE_WHITE_KNIGHT] :
              position.boards[PIECE_BLACK_KNIGHT];

  return (attacks & board);
}

/*
 * Check if supplied square is attacked by oponent side pieces
 *
 * Next: Add side back to functions
 */
bool square_is_attacked(Position position, Square square, Side side)
{
  return (square_is_attacked_by_queen( position, square, side) ||
          square_is_attacked_by_rook(  position, square, side) ||
          square_is_attacked_by_bishop(position, square, side) ||
          square_is_attacked_by_pawn(  position, square, side) ||
          square_is_attacked_by_king(  position, square, side) ||
          square_is_attacked_by_knight(position, square, side));
}
