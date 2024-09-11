/*
 * Check if move is legal in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

/*
 * Check if white pawn double jump move is pseudo legal
 */
static bool move_pawn_double_white_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so pawn is standing on 2nd rank
  if(!(source_square >= A2 && source_square <= H2)) return false;

  // Check so pawn is moving 2 ranks forward
  if((source_square - target_square) != (BOARD_FILES * 2)) return false;

  // Check so no piece is in the way
  U64 move_cover = (1ULL << target_square) | (1ULL << (target_square + BOARD_FILES));

  if(move_cover & position.covers[SIDE_BOTH]) return false;

  return true;
}

/*
 * Check if black pawn double jump move is pseudo legal
 */
static bool move_pawn_double_black_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so pawn is standing on 7th rank
  if(!(source_square >= A7 && source_square <= H7)) return false;

  // Check so pawn is moving 2 ranks forward
  if((target_square - source_square) != (BOARD_FILES * 2)) return false;

  // Check so no piece is in the way
  U64 move_cover = (1ULL << target_square) | (1ULL << (target_square - BOARD_FILES));

  if(move_cover & position.covers[SIDE_BOTH]) return false;

  return true;
}

/*
 * Check if pawn double jump move is pseudo legal
 */
static bool move_pawn_double_is_pseudo_legal(Position position, Move move)
{
  switch(MOVE_PIECE_GET(move))
  {
    case PIECE_WHITE_PAWN:
      return move_pawn_double_white_is_pseudo_legal(position, move);

    case PIECE_BLACK_PAWN:
      return move_pawn_double_black_is_pseudo_legal(position, move);

    default:
      return false;
  }
}

/*
 * Check if white pawn enpassant move is pseudo legal
 */
static bool move_pawn_passant_white_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so pawn is standing on 5th rank
  if(!((source_square >= A5) && (source_square <= H5))) return false;

  // Check so passant square is on 6th rank
  if(!((target_square >= A6) && (target_square <= H6))) return false;

  // Check so pawn is move one step forward to the side
  int moved_squares = (target_square - source_square);

  if((moved_squares != -9) && (moved_squares != -7)) return false;

  // Check so black pawn is standing next to passant square
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_PAWN], (target_square + BOARD_FILES))) return false;

  // Check so target square is empty
  if(position.covers[SIDE_BOTH] & (1ULL << target_square)) return false;

  return true;
}

/*
 * Check if black pawn enpassant move is pseudo legal
 */
static bool move_pawn_passant_black_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so pawn is standing on 4th rank
  if(!((source_square >= A4) && (source_square <= H4))) return false;

  // Check so passant square is on 3rd rank
  if(!((target_square >= A3) && (target_square <= H3))) return false;

  // Check so pawn is move one step forward to the side
  int moved_squares = (target_square - source_square);

  if((moved_squares != +9) && (moved_squares != +7)) return false;

  // Check so white pawn is standing next to passant square
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_PAWN], (target_square - BOARD_FILES))) return false;

  // Check so target square is empty
  if(position.covers[SIDE_BOTH] & (1ULL << target_square)) return false;

  return true;
}

/*
 * Check if pawn enpassant move is pseudo legal
 */
static bool move_pawn_passant_is_pseudo_legal(Position position, Move move)
{
  switch(MOVE_PIECE_GET(move))
  {
    case PIECE_WHITE_PAWN:
      return move_pawn_passant_white_is_pseudo_legal(position, move);

    case PIECE_BLACK_PAWN:
      return move_pawn_passant_black_is_pseudo_legal(position, move);

    default:
      return false;
  }
}

/*
 * Check if pawn capture move is pseudo legal
 *
 * Fix: Rewrite this function
 */
static bool move_pawn_capture_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so a piece is standing on target square
  if(!((1ULL << target_square) & position.covers[SIDE_BOTH])) return false;

  // Check so source piece and target piece are not on same side
  bool targetWhite = ((1ULL << target_square) & position.covers[SIDE_WHITE]);
  bool sourceWhite = ((1ULL << source_square) & position.covers[SIDE_WHITE]);

  if(!(sourceWhite ^ targetWhite)) return false;

  // Check so pawn can attack at target square
  Side side = (sourceWhite ? SIDE_WHITE : SIDE_BLACK);

  if(!(attacks_pawn_get(source_square, side) & (1ULL << target_square))) return false;

  return true;
}

/*
 * Check if a normal pawn move is pseudo legal
 */
static bool move_pawn_normal_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Check so no piece is standing on target square
  if((1ULL << target_square) & position.covers[SIDE_BOTH]) return false;

  // Check so pawn is moving only 1 rank forward
  switch(MOVE_PIECE_GET(move))
  {
    case PIECE_WHITE_PAWN:
      if((source_square - target_square) != BOARD_FILES) return false;
      break;

    case PIECE_BLACK_PAWN:
      if((target_square - source_square) != BOARD_FILES) return false;
      break;

    default:
      return false;
  }

  return true;
}

/*
 * Check if pawn move is pseudo legal
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
 * Check if white king-side castling move is pseudo legal
 */
static bool move_castle_white_king_is_pseudo_legal(Position position)
{
  // Check so a white rook is standing on H1
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], H1)) return false;

  // Check so no piece is in the way
  if(position.covers[SIDE_BOTH] & ((1ULL << G1) | (1ULL << F1))) return false;

  // Check so the king has right to castle king-side
  if(!(position.castle & CASTLE_WHITE_KING)) return false;

  // Check so no square where king moves is attacked by black
  if(square_is_attacked(position, F1, SIDE_BLACK) ||
     square_is_attacked(position, G1, SIDE_BLACK) ||
     square_is_attacked(position, E1, SIDE_BLACK)) return false;

  return true;
}

/*
 * Check if white queen-side castling move is pseudo legal
 */
static bool move_castle_white_queen_is_pseudo_legal(Position position)
{
  // Check so a white rook is standing on A1
  if(!BOARD_SQUARE_GET(position.boards[PIECE_WHITE_ROOK], A1)) return false;

  // Check so no piece is in the way
  if(position.covers[SIDE_BOTH] & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) return false;

  // Check so the king has right to castle queen-side
  if(!(position.castle & CASTLE_WHITE_QUEEN)) return false;

  // Check so no square where king moves is attacked by black
  if(square_is_attacked(position, C1, SIDE_BLACK) ||
     square_is_attacked(position, D1, SIDE_BLACK) ||
     square_is_attacked(position, E1, SIDE_BLACK)) return false;

  return true;
}

/*
 * Check if white castling move is pseudo legal
 */
static bool move_castle_white_is_pseudo_legal(Position position, Move move)
{
  if(MOVE_SOURCE_GET(move) != E1) return false;

  switch(MOVE_TARGET_GET(move))
  {
    case G1:
      return move_castle_white_king_is_pseudo_legal(position);

    case C1:
      return move_castle_white_queen_is_pseudo_legal(position);

    default:
      return false;
  }
}

/*
 * Check if black king-side castling move is pseudo legal
 */
static bool move_castle_black_king_is_pseudo_legal(Position position)
{
  // Check so a white rook is standing on H8
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], H8)) return false;

  // Check so no piece is in the way
  if(position.covers[SIDE_BOTH] & ((1ULL << G8) | (1ULL << F8))) return false;

  // Check so the king has right to castle king-side
  if(!(position.castle & CASTLE_BLACK_KING)) return false;

  // Check so no square where king moves is attacked by white
  if(square_is_attacked(position, G8, SIDE_WHITE) ||
     square_is_attacked(position, F8, SIDE_WHITE) ||
     square_is_attacked(position, E8, SIDE_WHITE)) return false;

  return true;
}

/*
 * Check if black queen-side castling move is pseudo legal
 */
static bool move_castle_black_queen_is_pseudo_legal(Position position)
{
  // Check so a white rook is standing on A8
  if(!BOARD_SQUARE_GET(position.boards[PIECE_BLACK_ROOK], A8)) return false;

  // Check so no piece is in the way
  if(position.covers[SIDE_BOTH] & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) return false;

  // Check so the king has right to castle queen-side
  if(!(position.castle & CASTLE_BLACK_QUEEN)) return false;

  // Check so no square where king moves is attacked by white
  if(square_is_attacked(position, C8, SIDE_WHITE) ||
     square_is_attacked(position, D8, SIDE_WHITE) ||
     square_is_attacked(position, E8, SIDE_WHITE)) return false;

  return true;
}

/*
 * Check if black castling move is pseudo legal
 */
static bool move_castle_black_is_pseudo_legal(Position position, Move move)
{
  if(MOVE_SOURCE_GET(move) != E8) return false;

  switch(MOVE_TARGET_GET(move))
  {
    case G8:
      return move_castle_black_king_is_pseudo_legal(position);

    case C8:
      return move_castle_black_queen_is_pseudo_legal(position);

    default:
      return false;
  }
}

/*
 * Check if castling move is pseudo legal
 */
static bool move_castle_is_pseudo_legal(Position position, Move move)
{
  switch(MOVE_PIECE_GET(move))
  {
    case PIECE_WHITE_KING:
      return move_castle_white_is_pseudo_legal(position, move);

    case PIECE_BLACK_KING:
      return move_castle_black_is_pseudo_legal(position, move);

    default:
      return false;
  }
}

/*
 * Check if a normal move is pseudo legal
 */
static bool move_normal_is_pseudo_legal(Position position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  // Prio 1. If there are any pieces between source and target square
  if(BOARD_LINES[source_square][target_square] & position.covers[SIDE_BOTH])
  {
    return false;
  }

  Piece source_piece = MOVE_PIECE_GET(move);

  // If the moving piece does not exists on the source square
  if(!BOARD_SQUARE_GET(position.boards[source_piece], source_square))
  {
    return false;
  }

  Piece target_piece = square_piece_get(position.boards, target_square);

  // If the capture flag and the target piece doesn't match each other
  if(((move & MOVE_MASK_CAPTURE) ? 1 : 0) ^ (target_piece != PIECE_NONE))
  {
    return false;
  }

  bool sourceWhite = (source_piece >= PIECE_WHITE_PAWN && source_piece <= PIECE_WHITE_KING);
  bool targetWhite = (target_piece >= PIECE_WHITE_PAWN && target_piece <= PIECE_WHITE_KING);

  // If a target piece exists (capture) and 
  // source and target are not of different sides (same sides)
  if((move & MOVE_MASK_CAPTURE) && !(sourceWhite ^ targetWhite))
  {
    return false;
  }

  // The piece is able to move from source_square to target_square
  return (attacks_get(source_square, position) & (1ULL << target_square));
}

/*
 * Check if move is pseudo legal in position
 */
static bool move_is_pseudo_legal(Position position, Move move)
{
  Piece piece = MOVE_PIECE_GET(move);

  if(PIECE_SIDE_GET(piece) != position.side) return false;


  if((piece == PIECE_WHITE_PAWN) || (piece == PIECE_BLACK_PAWN))
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
 * Get the square of the king of the supplied side
 */
static Square king_square_get(Position position, Side side)
{
  Piece king_piece = (side == SIDE_WHITE) ? PIECE_WHITE_KING : PIECE_BLACK_KING;

  return board_first_square_get(position.boards[king_piece]);
}

/*
 * Check if a move is legal in position
 *
 * If the move is pseudo legal,
 * try making the move and see if the king is attacked
 * - if not, the move is legal
 *
 * RETURN (bool result)
 * - true  | Move is legal
 * - false | Move is illegal
 */
bool move_is_legal(Position position, Move move)
{
  if(!move_is_pseudo_legal(position, move)) return false;

  Position moved_position = position;
  move_make(&moved_position, move);

  Square king_square = king_square_get(moved_position, position.side);

  // If the king does not exist, the move can no way be legal
  if(king_square == SQUARE_NONE) return false;

  return !square_is_attacked(moved_position, king_square, moved_position.side);
}
