#include "../treestump.h"

extern Piece boards_square_piece(U64 boards[12], Square square);

bool ident_capture_move(U64 boards[12], Square targetSquare)
{
  Piece targetPiece = boards_square_piece(boards, targetSquare);

  return (targetPiece != PIECE_NONE);
}

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

// Divide this function into smaller functions
Move create_move(U64 boards[12], Square sourceSquare, Square targetSquare)
{
  Move move = MOVE_NONE;

  move |= MOVE_SOURCE_SET(sourceSquare);
  move |= MOVE_TARGET_SET(targetSquare);


  Piece sourcePiece = boards_square_piece(boards, sourceSquare);

  move |= MOVE_PIECE_SET(sourcePiece);


  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    if(targetSquare >= A8 && targetSquare <= H8) move |= MOVE_PROMOTE_SET(PIECE_WHITE_QUEEN);

    if((sourceSquare - targetSquare) == (BOARD_FILES * 2)) move |= MOVE_MASK_DOUBLE;

    if(ident_passant_move(boards, sourcePiece, sourceSquare, targetSquare)) move |= MOVE_MASK_PASSANT;
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    if(targetSquare >= A1 && targetSquare <= H1) move |= MOVE_PROMOTE_SET(PIECE_WHITE_QUEEN);

    if((targetSquare - sourceSquare) == (BOARD_FILES * 2)) move |= MOVE_MASK_DOUBLE;

    if(ident_passant_move(boards, sourcePiece, sourceSquare, targetSquare)) move |= MOVE_MASK_PASSANT;
  }
  else if(sourcePiece == PIECE_WHITE_KING || sourcePiece == PIECE_BLACK_KING)
  {
    int squareDiff = (targetSquare - sourceSquare);

    if((squareDiff == +2) || (squareDiff == -2)) move |= MOVE_MASK_CASTLE;
  }

  if(ident_capture_move(boards, targetSquare)) move |= MOVE_MASK_CAPTURE;

  return move;
}

Move complete_move(U64 boards[12], Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  return create_move(boards, sourceSquare, targetSquare);
}
