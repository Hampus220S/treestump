/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-11
 */

#include "../treestump.h"

/*
 * Check if an enemy piece is standing on the target square
 */
static bool enemy_is_on_target_square(Position position, Piece piece, Square target_square)
{
  Side side = PIECE_SIDE_GET(piece);

  return (position.covers[!side] & (1ULL << target_square));
}

/*
 * Create a double jump pawn move, with the right flags
 */
Move move_double_create(Square source_square, Square target_square, Piece pawn_piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(source_square);
  move |= MOVE_TARGET_SET(target_square);

  move |= MOVE_PIECE_SET(pawn_piece);

  move |= MOVE_MASK_DOUBLE;

  return move;
}

/*
 * Create a promote move, with the right flags
 */
Move move_promote_create(Position position, Square pawn_square, Square promote_square, Piece pawn_piece, Piece promote_piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(pawn_square);
  move |= MOVE_TARGET_SET(promote_square);

  move |= MOVE_PIECE_SET  (pawn_piece);
  move |= MOVE_PROMOTE_SET(promote_piece);

  if(enemy_is_on_target_square(position, pawn_piece, promote_square))
  {
    move |= MOVE_MASK_CAPTURE;
  }

  return move;
}

/*
 * Create a castling move, with the right flags
 */
Move move_castle_create(Square source_square, Square target_square, Piece king_piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(source_square);
  move |= MOVE_TARGET_SET(target_square);

  move |= MOVE_PIECE_SET(king_piece);

  move |= MOVE_MASK_CASTLE;

  return move;
}

/*
 * Create a just normal move, either a capture or quiet move
 */
Move move_normal_create(Position position, Square source_square, Square target_square, Piece piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(source_square);
  move |= MOVE_TARGET_SET(target_square);

  move |= MOVE_PIECE_SET(piece);

  if(enemy_is_on_target_square(position, piece, target_square))
  {
    move |= MOVE_MASK_CAPTURE;
  }

  return move;
}

/*
 * This function does not care about rules
 * It tries to identify special moves, and give them flags
 *
 * MOVE_MASK_DOUBLE  | A pawn does a double jump move
 * MOVE_MASK_PASSANT | A pawn does enpassant
 * MOVE_MASK_CASTLE  | A king castles
 * MOVE_MASK_CAPTURE | A piece captures another piece
 */
static Move move_flag_create(Position position, Square source_square, Square target_square, Piece piece)
{
  if(piece == PIECE_WHITE_PAWN || piece == PIECE_BLACK_PAWN)
  {
    // If the pawn moves 2 rows, it must be because it does a double move
    if(abs(source_square - target_square) == (BOARD_FILES * 2))
    {
      return MOVE_MASK_DOUBLE;
    }

    if(target_square == position.passant)
    {
      return MOVE_MASK_PASSANT;
    }
  }
  else if(piece == PIECE_WHITE_KING || piece == PIECE_BLACK_KING)
  {
    // If the king moves 2 columns, it must be because it castles
    if(abs(target_square - source_square) == 2)
    {
      return MOVE_MASK_CASTLE;
    }
  }

  if(enemy_is_on_target_square(position, piece, target_square))
  {
    return MOVE_MASK_CAPTURE;
  }

  return MOVE_NONE;
}

/*
 * Create a move with just a promote piece
 *
 * If no promote piece is supplied and pawn will promote,
 * a queen will be sat as the promote piece
 *
 * RETURN (Move move)
 *
 * Fix: Rename or refactor this function
 */
static Move move_promote_piece_create(Piece piece, Square target_square, Piece promote_piece)
{
  if(piece == PIECE_WHITE_PAWN)
  {
    if(target_square >= A8 && target_square <= H8)
    {
      // Here, PIECE_WHITE_PAWN means no promote piece is specified
      if(promote_piece == PIECE_WHITE_PAWN)
      {
        promote_piece = PIECE_WHITE_QUEEN;
      }

      return MOVE_PROMOTE_SET(promote_piece);
    }
  }
  else if(piece == PIECE_BLACK_PAWN)
  {
    if(target_square >= A1 && target_square <= H1)
    {
      // Here, PIECE_WHITE_PAWN means no promote piece is specified
      if(promote_piece == PIECE_WHITE_PAWN)
      {
        promote_piece = PIECE_BLACK_QUEEN;
      }

      return MOVE_PROMOTE_SET(promote_piece);
    }
  }

  return MOVE_NONE;
}

/*
 * This function is not meant to create legal moves per say,
 * but to give the move the right information to possible be a legal move
 *
 * PARAMS
 * - Piece promote_piece | Potential promote piece (can be PIECE_NONE)
 */
Move move_create(Position position, Square source_square, Square target_square, Piece promote_piece)
{
  Move move = MOVE_NONE;

  Piece piece = square_piece_get(position.boards, source_square);

  move |= MOVE_SOURCE_SET(source_square);
  move |= MOVE_TARGET_SET(target_square);
  move |= MOVE_PIECE_SET(piece);

  move |= move_promote_piece_create(piece, target_square, promote_piece);
  move |= move_flag_create(position, source_square, target_square, piece);

  return move;
}
