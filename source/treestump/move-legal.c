/*
 * Check if move is legal in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

/*
 *
 */
static bool move_pawn_double_white_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  if(!(sourceSquare >= A2 && sourceSquare <= H2)) return false;

  if((sourceSquare - targetSquare) != (BOARD_FILES * 2)) return false;

  U64 moveCovers = (1ULL << targetSquare) | (1ULL << (targetSquare + BOARD_FILES));

  if(moveCovers & position.covers[SIDE_BOTH]) return false;

  return true;
}

/*
 *
 */
static bool move_pawn_double_black_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  if(!(sourceSquare >= A7 && sourceSquare <= H7)) return false;

  if((targetSquare - sourceSquare) != (BOARD_FILES * 2)) return false;

  U64 moveCovers = (1ULL << targetSquare) | (1ULL << (targetSquare - BOARD_FILES));

  if(moveCovers & position.covers[SIDE_BOTH]) return false;

  return true;
}

/*
 *
 */
static bool move_pawn_double_is_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_PIECE_GET(move);

  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    return move_pawn_double_white_is_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    return move_pawn_double_black_is_pseudo_legal(position, move);
  }
  else return false;
}

/*
 *
 */
static bool move_pawn_passant_white_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  if(!((sourceSquare >= A5) && (sourceSquare <= H5))) return false;

  if(!((targetSquare >= A6) && (targetSquare <= H6))) return false;


  int squareDiff = (targetSquare - sourceSquare);

  if((squareDiff != -9) && (squareDiff != -7)) return false;

  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_PAWN], (targetSquare + BOARD_FILES))) return false;

  if(position.covers[SIDE_BOTH] & (1ULL << targetSquare)) return false;

  return true;
}

/*
 *
 */
static bool move_pawn_passant_black_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  int squareDiff = (targetSquare - sourceSquare);

  if(!((sourceSquare >= A4) && (sourceSquare <= H4))) return false;

  if(!((targetSquare >= A3) && (targetSquare <= H3))) return false;

  if((squareDiff != +9) && (squareDiff != +7)) return false;

  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_PAWN], (targetSquare - BOARD_FILES))) return false;

  if(position.covers[SIDE_BOTH] & (1ULL << targetSquare)) return false;

  return true;
}

/*
 *
 */
static bool move_pawn_passant_is_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_PIECE_GET(move);

  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    return move_pawn_passant_white_is_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    return move_pawn_passant_black_is_pseudo_legal(position, move);
  }
  else return false;
}

/*
 *
 */
static bool move_pawn_capture_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);


  if(!((1ULL << targetSquare) & position.covers[SIDE_BOTH])) return false;

  bool targetWhite = ((1ULL << targetSquare) & position.covers[SIDE_WHITE]);
  bool sourceWhite = ((1ULL << sourceSquare) & position.covers[SIDE_WHITE]);

  if(!(sourceWhite ^ targetWhite)) return false;


  Side side = (sourceWhite ? SIDE_WHITE : SIDE_BLACK);

  return (attacks_pawn_get(sourceSquare, side) & (1ULL << targetSquare));
}

/*
 *
 */
static bool move_pawn_normal_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  if((1ULL << targetSquare) & position.covers[SIDE_BOTH]) return false;


  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    if((sourceSquare - targetSquare) != BOARD_FILES) return false;
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    if((targetSquare - sourceSquare) != BOARD_FILES) return false;
  }
  else return false;

  return true;
}

/*
 *
 */
static bool move_pawn_is_pseudo_legal(Position position, Move move)
{
  if(move & MOVE_MASK_DOUBLE)
  {
    return move_pawn_double_is_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_PASSANT)
  {
    return move_pawn_passant_is_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_CAPTURE)
  {
    return move_pawn_capture_is_pseudo_legal(position, move);
  }
  else
  {
    return move_pawn_normal_is_pseudo_legal(position, move);
  }
}

/*
 *
 */
static bool move_castle_white_king_is_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], H1)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << G1) | (1ULL << F1))) return false;

  if(!(position.castle & CASTLE_WHITE_KING)) return false;

  if(square_is_attacked(position, F1, SIDE_BLACK)) return false;

  if(square_is_attacked(position, G1, SIDE_BLACK)) return false;

  if(square_is_attacked(position, E1, SIDE_BLACK)) return false;

  return true;
}

/*
 *
 */
static bool move_castle_white_queen_is_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], A1)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) return false;

  if(!(position.castle & CASTLE_WHITE_QUEEN)) return false;

  if(square_is_attacked(position, C1, SIDE_BLACK)) return false;

  if(square_is_attacked(position, D1, SIDE_BLACK)) return false;

  if(square_is_attacked(position, E1, SIDE_BLACK)) return false;

  return true;
}

/*
 *
 */
static bool move_castle_white_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  if(sourceSquare != E1) return false;

  if(targetSquare == G1)
  {
    return move_castle_white_king_is_pseudo_legal(position);
  }
  else if(targetSquare == C1)
  {
    return move_castle_white_queen_is_pseudo_legal(position);
  }
  else return false;
}

/*
 *
 */
static bool move_castle_black_king_is_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], H8)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << G8) | (1ULL << F8))) return false;

  if(!(position.castle & CASTLE_BLACK_KING)) return false;

  if(square_is_attacked(position, G8, SIDE_WHITE)) return false;

  if(square_is_attacked(position, F8, SIDE_WHITE)) return false;

  if(square_is_attacked(position, E8, SIDE_WHITE)) return false;

  return true;
}

/*
 *
 */
static bool move_castle_black_queen_is_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], A8)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) return false;

  if(!(position.castle & CASTLE_BLACK_QUEEN)) return false;

  if(square_is_attacked(position, C8, SIDE_WHITE)) return false;

  if(square_is_attacked(position, D8, SIDE_WHITE)) return false;

  if(square_is_attacked(position, E8, SIDE_WHITE)) return false;

  return true;
}

/*
 *
 */
static bool move_castle_black_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  if(sourceSquare != E8) return false;

  if(targetSquare == G8)
  {
    return move_castle_black_king_is_pseudo_legal(position);
  }
  else if(targetSquare == C8)
  {
    return move_castle_black_queen_is_pseudo_legal(position);
  }
  else return false;
}

/*
 *
 */
static bool move_castle_is_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_PIECE_GET(move);

  if(sourcePiece == PIECE_WHITE_KING)
  {
    return move_castle_white_is_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_KING)
  {
    return move_castle_black_is_pseudo_legal(position, move);
  }
  else return false;
}

/*
 *
 */
static bool move_normal_is_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  // If the moving piece does not exists on the source square
  if(!BOARD_SQUARE_GET(position.boards[sourcePiece], sourceSquare)) return false;

  Piece targetPiece = square_piece_get(position.boards, targetSquare);

  // If the capture flag and the target piece doesn't match each other
  if(((move & MOVE_MASK_CAPTURE) ? 1 : 0) ^ (targetPiece != PIECE_NONE)) return false;

  bool sourceWhite = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING);
  bool targetWhite = (targetPiece >= PIECE_WHITE_PAWN && targetPiece <= PIECE_WHITE_KING);

  // If source and target are not of different sides (same sides)
  if((move & MOVE_MASK_CAPTURE) && !(sourceWhite ^ targetWhite)) return false;

  // If there are any pieces between source and target square
  if(BOARD_LINES[sourceSquare][targetSquare] & position.covers[SIDE_BOTH]) return false;

  // The piece is able to move from sourceSquare to targetSquare
  return (attacks_get(sourceSquare, position) & (1ULL << targetSquare));
}

/*
 *
 */
static bool move_is_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_PIECE_GET(move);

  if((sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) && position.side != SIDE_WHITE) return false;
  if((sourcePiece >= PIECE_BLACK_PAWN && sourcePiece <= PIECE_BLACK_KING) && position.side != SIDE_BLACK) return false;

  if((sourcePiece == PIECE_WHITE_PAWN) || (sourcePiece == PIECE_BLACK_PAWN))
  {
    return move_pawn_is_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_CASTLE)
  {
    return move_castle_is_pseudo_legal(position, move);
  }
  else
  {
    return move_normal_is_pseudo_legal(position, move);
  }
}

/*
 *
 */
bool move_is_legal(Position position, Move move)
{
  if(!move_is_pseudo_legal(position, move)) return false;

  Position movedPosition = position;

  move_make(&movedPosition, move);


  Piece kingPiece = (movedPosition.side == SIDE_WHITE) ? PIECE_BLACK_KING : PIECE_WHITE_KING;

  Square kingSquare = board_ls1b_index(movedPosition.boards[kingPiece]);

  // The king should always exists, but if it don't return false
  if(kingSquare == -1) return false;

  return !square_is_attacked(movedPosition, kingSquare, movedPosition.side);
}
