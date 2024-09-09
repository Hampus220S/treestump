/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

// Rename this
bool ident_capture_move(U64 boards[12], Square targetSquare)
{
  Piece targetPiece = boards_square_piece(boards, targetSquare);

  return (targetPiece != PIECE_NONE);
}

// Rename this, and rewrite to be as simple as possible
bool ident_passant_move(U64 boards[12], Piece sourcePiece, Square sourceSquare, Square targetSquare)
{
  // Change this function, boards_square_piece should not be used
  Piece targetPiece = boards_square_piece(boards, targetSquare);

  if(targetPiece != PIECE_NONE) return false;

  int squareDiff = (targetSquare - sourceSquare);

  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    if((squareDiff == -9) || (squareDiff == -7)) return true;
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    if((squareDiff == +9) || (squareDiff == +7)) return true;
  }
  return false;
}

Move create_double_move(Square sourceSquare, Square targetSquare, Piece piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);

  move |= MOVE_PIECE_SET(piece);

  move |= MOVE_MASK_DOUBLE;

  return move;
}

Move create_promote_move(U64 boards[12], Square sourceSquare, Square targetSquare, Piece piece, Piece promotePiece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);

  move |= MOVE_PIECE_SET(piece);

  move |= MOVE_PROMOTE_SET(promotePiece);

  if(ident_capture_move(boards, targetSquare)) move |= MOVE_MASK_CAPTURE;

  return move;
}

Move create_castle_move(Square sourceSquare, Square targetSquare, Piece piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);

  move |= MOVE_PIECE_SET(piece);

  move |= MOVE_MASK_CASTLE;

  return move;
}

Move create_normal_move(U64 boards[12], Square sourceSquare, Square targetSquare, Piece piece)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);

  move |= MOVE_PIECE_SET(piece);

  if(ident_capture_move(boards, targetSquare)) move |= MOVE_MASK_CAPTURE;

  return move;
}

/*
 * This function does not care about rules.
 * It tries to identify special moves, and give them flags
 *
 * MOVE_MASK_DOUBLE  | A pawn does a double move
 * MOVE_MASK_PASSANT | A pawn does enpassant
 * MOVE_MASK_CASTLE  | A king castles
 * MOVE_MASK_CAPTURE | A piece captures another piece
 */
static Move move_flag_create(U64 boards[12], Piece sourcePiece, Square sourceSquare, Square targetSquare)
{
  if(sourcePiece == PIECE_WHITE_PAWN || sourcePiece == PIECE_BLACK_PAWN)
  {
    // If the pawn moves 2 rows, it must be because it does a double move
    if(abs(sourceSquare - targetSquare) == (BOARD_FILES * 2))
    {
      return MOVE_MASK_DOUBLE;
    }

    if(ident_passant_move(boards, sourcePiece, sourceSquare, targetSquare))
    {
      return MOVE_MASK_PASSANT;
    }
  }
  else if(sourcePiece == PIECE_WHITE_KING || sourcePiece == PIECE_BLACK_KING)
  {
    // If the king moves 2 columns, it must be because it castles
    if(abs(targetSquare - sourceSquare) == 2)
    {
      return MOVE_MASK_CASTLE;
    }
  }

  if(ident_capture_move(boards, targetSquare))
  {
    return MOVE_MASK_CAPTURE;
  }

  return MOVE_NONE;
}

/*
 *
 */
static Move move_promote_create(Piece sourcePiece, Square targetSquare, Piece promotePiece)
{
  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    if(targetSquare >= A8 && targetSquare <= H8)
    {
      // Here, PIECE_WHITE_PAWN means no promote piece is specified
      if(promotePiece == PIECE_WHITE_PAWN)
      {
        promotePiece = PIECE_WHITE_QUEEN;
      }

      return MOVE_PROMOTE_SET(promotePiece);
    }
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    if(targetSquare >= A1 && targetSquare <= H1)
    {
      // Here, PIECE_WHITE_PAWN means no promote piece is specified
      if(promotePiece == PIECE_WHITE_PAWN)
      {
        promotePiece = PIECE_BLACK_QUEEN;
      }

      return MOVE_PROMOTE_SET(promotePiece);
    }
  }

  return MOVE_NONE;
}

/*
 * This function is not meant to create legal moves per say,
 * but to give the move the right information to possible be a legal move
 */
Move move_create(U64 boards[12], Square sourceSquare, Square targetSquare, Piece promotePiece)
{
  Move move = MOVE_NONE;

  Piece sourcePiece = boards_square_piece(boards, sourceSquare);

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);
  move |= MOVE_PIECE_SET(sourcePiece);

  move |= move_promote_create(sourcePiece, targetSquare, promotePiece);
  move |= move_flag_create(boards, sourcePiece, sourceSquare, targetSquare);

  return move;
}
