#include "../engine.h"

#include "piece-lookup-attacks.h"

bool pawn_white_double_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  if(!(sourceSquare >= A2 && sourceSquare <= H2)) return false;

  if((sourceSquare - targetSquare) != (BOARD_FILES * 2)) return false;

  U64 moveCovers = (1ULL << targetSquare) | (1ULL << (targetSquare + BOARD_FILES));

  if(moveCovers & position.covers[SIDE_BOTH]) return false;

  return true;
}

bool pawn_black_double_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  if(!(sourceSquare >= A7 && sourceSquare <= H7)) return false;

  if((targetSquare - sourceSquare) != (BOARD_FILES * 2)) return false;

  U64 moveCovers = (1ULL << targetSquare) | (1ULL << (targetSquare - BOARD_FILES));

  if(moveCovers & position.covers[SIDE_BOTH]) return false;

  return true;
}

bool pawn_double_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_GET_PIECE(move);

  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    return pawn_white_double_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    return pawn_black_double_pseudo_legal(position, move);
  }
  else return false;
}

bool pawn_white_passant_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  if(!((sourceSquare >= A5) && (sourceSquare <= H5))) return false;

  if(!((targetSquare >= A6) && (targetSquare <= H6))) return false;


  int squareDiff = (targetSquare - sourceSquare);

  if((squareDiff != -9) && (squareDiff != -7)) return false;

  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_PAWN], (targetSquare + BOARD_FILES))) return false;

  if(position.covers[SIDE_BOTH] & (1ULL << targetSquare)) return false;

  return true;
}

bool pawn_black_passant_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  int squareDiff = (targetSquare - sourceSquare);

  if(!((sourceSquare >= A4) && (sourceSquare <= H4))) return false;

  if(!((targetSquare >= A3) && (targetSquare <= H3))) return false;

  if((squareDiff != +9) && (squareDiff != +7)) return false;

  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_PAWN], (targetSquare - BOARD_FILES))) return false;

  if(position.covers[SIDE_BOTH] & (1ULL << targetSquare)) return false;

  return true;
}

bool pawn_passant_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_GET_PIECE(move);

  if(sourcePiece == PIECE_WHITE_PAWN)
  {
    return pawn_white_passant_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_PAWN)
  {
    return pawn_black_passant_pseudo_legal(position, move);
  }
  else return false;
}

bool pawn_capture_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);


  if(!((1ULL << targetSquare) & position.covers[SIDE_BOTH])) return false;

  bool targetWhite = ((1ULL << targetSquare) & position.covers[SIDE_WHITE]);
  bool sourceWhite = ((1ULL << sourceSquare) & position.covers[SIDE_WHITE]);

  if(!(sourceWhite ^ targetWhite)) return false;


  Side side = (sourceWhite ? SIDE_WHITE : SIDE_BLACK);

  return (pawn_lookup_attacks(side, sourceSquare) & (1ULL << targetSquare));
}

bool pawn_normal_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  Piece sourcePiece = MOVE_GET_PIECE(move);

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

bool pawn_move_pseudo_legal(Position position, Move move)
{
  if(move & MOVE_MASK_DOUBLE)
  {
    return pawn_double_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_PASSANT)
  {
    return pawn_passant_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_CAPTURE)
  {
    return pawn_capture_pseudo_legal(position, move);
  }
  else return pawn_normal_pseudo_legal(position, move);
}

bool castle_white_king_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], H1)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << G1) | (1ULL << F1))) return false;

  if(!(position.castle & CASTLE_WHITE_KING)) return false;

  return true;
}

bool castle_white_queen_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], A1)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << C1) | (1ULL << D1))) return false;

  if(!(position.castle & CASTLE_WHITE_QUEEN)) return false;

  return true;
}

bool castle_white_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  if(sourceSquare != E1) return false;

  if(targetSquare == G1) return castle_white_king_pseudo_legal(position);
  
  else if(targetSquare == C1) return castle_white_queen_pseudo_legal(position);
  
  else return false;
}

bool castle_black_king_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], H1)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << G8) | (1ULL << F8))) return false;

  if(!(position.castle & CASTLE_BLACK_KING)) return false;

  return true;
}

bool castle_black_queen_pseudo_legal(Position position)
{
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], A8)) return false;

  if(position.covers[SIDE_BOTH] & ((1ULL << C8) | (1ULL << D8))) return false;

  if(!(position.castle & CASTLE_BLACK_QUEEN)) return false;

  return true;
}

bool castle_black_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  if(sourceSquare != E8) return false;

  if(targetSquare == G8) return castle_black_king_pseudo_legal(position);
  
  else if(targetSquare == C8) return castle_black_queen_pseudo_legal(position);
  
  else return false;
}

bool castle_move_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_GET_PIECE(move);

  if(sourcePiece == PIECE_WHITE_KING)
  {
    return castle_white_pseudo_legal(position, move);
  }
  else if(sourcePiece == PIECE_BLACK_KING)
  {
    return castle_black_pseudo_legal(position, move);
  }
  else return false;
}

bool normal_move_pseudo_legal(Position position, Move move)
{
  Square sourceSquare = MOVE_GET_SOURCE(move);
  Square targetSquare = MOVE_GET_TARGET(move);

  Piece sourcePiece = MOVE_GET_PIECE(move);

  // If the moving piece does not exists on the source square
  if(!BOARD_SQUARE_GET(position.boards[sourcePiece], sourceSquare)) return false;

  Piece targetPiece = boards_square_piece(position.boards, targetSquare);

  // If the capture flag and the target piece doesn't match each other
  if(((move & MOVE_MASK_CAPTURE) ? 1 : 0) ^ (targetPiece != PIECE_NONE)) return false;

  bool sourceWhite = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING);
  bool targetWhite = (targetPiece >= PIECE_WHITE_PAWN && targetPiece <= PIECE_WHITE_KING);

  // If source and target are not of different sides (same sides)
  if((move & MOVE_MASK_CAPTURE) && !(sourceWhite ^ targetWhite)) return false;

  // If there are any pieces between source and target square
  if(BOARD_LOOKUP_LINES[sourceSquare][targetSquare] & position.covers[SIDE_BOTH]) return false;

  // The piece is able to move from sourceSquare to targetSquare
  return (piece_lookup_attacks(position, sourceSquare) & (1ULL << targetSquare));
}

bool move_pseudo_legal(Position position, Move move)
{
  Piece sourcePiece = MOVE_GET_PIECE(move);

  if((sourcePiece == PIECE_WHITE_PAWN) || (sourcePiece == PIECE_BLACK_PAWN))
  {
    return pawn_move_pseudo_legal(position, move);
  }
  else if(move & MOVE_MASK_CASTLE)
  {
    return castle_move_pseudo_legal(position, move);
  }
  else return normal_move_pseudo_legal(position, move);
}

bool move_fully_legal(Position position, Move move)
{
  if(!move_pseudo_legal(position, move)) return false;

  return true;
}