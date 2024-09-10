/*
 * Make move in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-10
 */

#include "../treestump.h"

extern const Castle SQUARE_CASTLES[];

/*
 * Pick up the specified piece from the source square,
 * place down the piece to the target square,
 * reflect the change in the cover boards
 *
 * PARAMS
 * - Position* position      | Position to make the move on
 * - Piece     piece         | Which piece to move
 * - Square    source_square | Square to pick up piece from
 * - Square    target_square | Square to place down piece to
 */
static void position_piece_move(Position* position, Piece piece, Square source, Square target)
{
  U64  move_board = (1ULL << source) ^ (1ULL << target);
  Side side = PIECE_SIDE_GET(piece);

  position->boards[piece]     ^= move_board;
  position->covers[side]      ^= move_board;
  position->covers[SIDE_BOTH] ^= move_board;
}

/*
 * Pick up a piece from the specified square
 * reflect the change in the cover boards
 *
 * PARAMS
 * - Position* position | Position to pick up piece on
 * - Square    square   | Square to pick up piece from
 *
 * If there is no piece at specified square, do nothing
 */
static void position_square_piece_pick_up(Position* position, Square square)
{
  Piece piece = square_piece_get(position->boards, square);
  
  if(piece == PIECE_NONE) return;

  Side side = PIECE_SIDE_GET(piece);

  position->boards[piece]     = BOARD_SQUARE_POP(position->boards[piece],     square);
  position->covers[side]      = BOARD_SQUARE_POP(position->covers[side],      square);
  position->covers[SIDE_BOTH] = BOARD_SQUARE_POP(position->covers[SIDE_BOTH], square);
}

/*
 * Pick up the specified pawn from the pawn square,
 * place down the specified promote piece to the promote square,
 * reflect the change in the cover boards
 *
 * PARAMS
 * - Position* position       | Position to promote pawn on
 * - Piece     pawn_piece     | Which pawn to promote
 * - Square    pawn_square    | Square to pick up pawn from
 * - Piece     promote_piece  | Which piece to promote to
 * - Square    promote_square | Square to place down promote piece to
 */
static void position_pawn_promote(Position* position, Piece pawn_piece, Square pawn_square, Piece promote_piece, Square promote_square)
{
  U64  move_board = (1ULL << pawn_square) ^ (1ULL << promote_square);
  Side side = PIECE_SIDE_GET(pawn_piece);

  position->boards[promote_piece] = BOARD_SQUARE_SET(position->boards[promote_piece], promote_square);
  position->boards[pawn_piece]    = BOARD_SQUARE_POP(position->boards[pawn_piece],    pawn_square);
  position->covers[side]      ^= move_board;
  position->covers[SIDE_BOTH] ^= move_board;
}

#define CASTLE_ROOK_SOURCE_GET(SOURCE, TARGET) ((TARGET) > (SOURCE)) ? ((SOURCE) + 3) : ((SOURCE) - 4)
#define CASTLE_ROOK_TARGET_GET(SOURCE, TARGET) ((TARGET) > (SOURCE)) ? ((TARGET) - 1) : ((TARGET) + 1)

/*
 * Castle the king and his rook
 */
static void move_castle_make(Position* position, Move move)
{
  Square king_source = MOVE_SOURCE_GET(move);
  Square king_target = MOVE_TARGET_GET(move);

  Piece king_piece = MOVE_PIECE_GET(move);
  Piece rook_piece = (king_piece == PIECE_WHITE_KING) ? PIECE_WHITE_ROOK : PIECE_BLACK_ROOK;

  Square rook_source = CASTLE_ROOK_SOURCE_GET(king_source, king_target);
  Square rook_target = CASTLE_ROOK_TARGET_GET(king_source, king_target);

  // 1. Move the king to its castling square
  position_piece_move(position, king_piece, king_source, king_target);

  // 2. Move the rook to its castling square
  position_piece_move(position, rook_piece, rook_source, rook_target);

  // 3. Clear the castling rights for the side that castled
  position->castle &= (king_piece == PIECE_WHITE_KING) ? ~CASTLE_WHITE : ~CASTLE_BLACK;
}

/*
 * Make a normal move, not capturing anything
 */
static void move_normal_quiet_make(Position* position, Move move)
{
  Square source = MOVE_SOURCE_GET(move);
  Square target = MOVE_TARGET_GET(move);

  Piece piece = MOVE_PIECE_GET(move);

  position_piece_move(position, piece, source, target);

  // Check what this is doing here...
  position->clock++;
}

/*
 * Capture one of the opponents pieces
 */
static void move_normal_capture_make(Position* position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  Piece piece = MOVE_PIECE_GET(move);

  // 1. Pick up the piece that is beging captured
  position_square_piece_pick_up(position, target_square);

  // 2. Move the attacking piece to its square
  position_piece_move(position, piece, source_square, target_square);

  // 3. Reset the clock, because a capture was made
  position->clock = 0;
}

/*
 * Clear castling rights for moving piece
 */
static void castle_clear_for_moving_piece(Position* position, Move move)
{
  switch(MOVE_PIECE_GET(move))
  {
    case PIECE_WHITE_ROOK: case PIECE_BLACK_ROOK:
      position->castle &= ~SQUARE_CASTLES[MOVE_SOURCE_GET(move)];
      break;

    case PIECE_WHITE_KING:
      position->castle &= ~CASTLE_WHITE;
      break;

    case PIECE_BLACK_KING:
      position->castle &= ~CASTLE_BLACK;
      break;

    default:
      break;
  }
}

/*
 * Clear castling rights for the piece being captured
 */
static void castle_clear_for_capture_piece(Position* position, Move move)
{
  position->castle &= ~SQUARE_CASTLES[MOVE_TARGET_GET(move)];
}

/*
 * Make a normal move, either a capture or a quiet move
 */
static void move_normal_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_normal_capture_make(position, move);
  }
  else
  {
    move_normal_quiet_make(position, move);
  }

  castle_clear_for_moving_piece(position, move);
}

/*
 * Make a pawn move - capturing the oppontens enpassant pawn
 */
static void move_pawn_passant_make(Position* position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  Piece piece = MOVE_PIECE_GET(move);

  position_square_piece_pick_up(position, position->passant);

  position_piece_move(position, piece, source_square, target_square);
}

#define PASSANT_SQUARE_GET(PIECE, SQUARE) (((PIECE) == PIECE_WHITE_PAWN) ? ((SQUARE) - BOARD_FILES) : ((SQUARE) + BOARD_FILES))

/*
 * Make a pawn move - double jumping in the start
 *
 * Set the enpassant square to where the pawn jumped over
 */
static void move_pawn_double_make(Position* position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  Piece pawn_piece = MOVE_PIECE_GET(move);

  position_piece_move(position, pawn_piece, source_square, target_square);

  position->passant = PASSANT_SQUARE_GET(pawn_piece, source_square);
}

/*
 * Make a pawn move - promote without capturing any piece
 */
static void move_pawn_promote_quiet_make(Position* position, Move move)
{
  Piece  pawn_piece     = MOVE_PIECE_GET(move);
  Square pawn_square    = MOVE_SOURCE_GET(move);

  Piece  promote_piece  = MOVE_PROMOTE_GET(move);
  Square promote_square = MOVE_TARGET_GET(move);

  position_pawn_promote(position, pawn_piece, pawn_square, promote_piece, promote_square);
}

/*
 * Make a pawn move - promote and capture opponents piece
 */
static void move_pawn_promote_capture_make(Position* position, Move move)
{
  Piece  pawn_piece     = MOVE_PIECE_GET(move);
  Square pawn_square    = MOVE_SOURCE_GET(move);

  Piece  promote_piece  = MOVE_PROMOTE_GET(move);
  Square promote_square = MOVE_TARGET_GET(move);

  position_square_piece_pick_up(position, promote_square);

  position_pawn_promote(position, pawn_piece, pawn_square, promote_piece, promote_square);
} 

/*
 * Make a pawn move - promote with either capture or quiet move
 */
static void move_pawn_promote_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_pawn_promote_capture_make(position, move);
  }
  else
  {
    move_pawn_promote_quiet_make(position, move);
  }

  // This is made for both cases of promote moves
  // position->passant = PIECE_NONE;
}

/*
 *
 */
static void move_pawn_normal_quiet_make(Position* position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  Piece piece = MOVE_PIECE_GET(move);

  position_piece_move(position, piece, source_square, target_square);
}

/*
 *
 */
static void move_pawn_normal_capture_make(Position* position, Move move)
{
  Square source_square = MOVE_SOURCE_GET(move);
  Square target_square = MOVE_TARGET_GET(move);

  Piece piece = MOVE_PIECE_GET(move);

  position_square_piece_pick_up(position, target_square);

  position_piece_move(position, piece, source_square, target_square);
}

/*
 *
 */
static void move_pawn_normal_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_pawn_normal_capture_make(position, move);
  }
  else
  {
    move_pawn_normal_quiet_make(position, move);
  }

  // This is made for both pawn normal moves
  // position->passant = PIECE_NONE;
}

/*
 *
 */
static void move_pawn_make(Position* position, Move move)
{
  if(move & MOVE_MASK_DOUBLE)
  {
    move_pawn_double_make(position, move);
  }
  else if(move & MOVE_MASK_PROMOTE)
  {
    move_pawn_promote_make(position, move);
  }
  else if(move & MOVE_MASK_PASSANT)
  {
    move_pawn_passant_make(position, move);
  }
  else
  {
    move_pawn_normal_make(position, move);
  }

  // This is made for every pawn move
  position->clock = 0;
}

/*
 * Make move in position
 * excpected that the move is legal and valid
 *
 * This function just executes, and does not validate
 */
void move_make(Position* position, Move move)
{
  position->clock++;

  position->passant = SQUARE_NONE;

  Piece sourcePiece = MOVE_PIECE_GET(move);

  // 2. Make more specific things based on type of move
  if(sourcePiece == PIECE_WHITE_PAWN || sourcePiece == PIECE_BLACK_PAWN)
  {
    move_pawn_make(position, move);
  }
  else if(move & MOVE_MASK_CASTLE)
  {
    move_castle_make(position, move);
  }
  else
  {
    move_normal_make(position, move);
  }

  // If a rook is being captured, remove its castling rights
  castle_clear_for_capture_piece(position, move);

  // If black made the move, a new turn has been made
  if(position->side == SIDE_BLACK) position->turns++;

  // Switch sides
  position->side = !position->side;
}
