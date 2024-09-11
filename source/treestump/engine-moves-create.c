/*
 * Create an array of legal moves in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-11
 */

#include "../treestump.h"

#include "engine-intern.h"

/*
 * Add move to move array, but only if it is legal
 *
 * RETURN (bool result)
 * - true  | Move was added
 * - false | Move was not legal, and not added
 */
static bool move_add_if_legal(MoveArray* move_array, Position position, Move move)
{
  if(!move_is_legal(position, move)) return false;

  move_array->moves[move_array->amount++] = move;

  return true;
}

/*
 * Create the different promote moves for a white pawn
 */
static void moves_white_pawn_promote_pieces_create(MoveArray* move_array, Move move)
{
  for(Piece piece = PIECE_WHITE_KNIGHT; piece <= PIECE_WHITE_QUEEN; piece++)
  {
    move = (move & ~MOVE_MASK_PROMOTE) | MOVE_PROMOTE_SET(piece);

    move_array->moves[move_array->amount++] = move;
  }
}

/*
 * Create legal moves for a white pawn, which promotes
 *
 * If a pawn is standing on A7, pawn_square - 9 will result in unsigned underflow
 */
static void moves_white_pawn_promote_create(MoveArray* move_array, Position position, Square pawn_square)
{
  Square left_square  = ((int) pawn_square - 9) > 0 ? (pawn_square - 9) : 0;
  Square right_square = pawn_square - 7;

  for(Square promote_square = left_square; promote_square <= right_square; promote_square++)
  {
    Move move = move_promote_create(position, pawn_square, promote_square, PIECE_WHITE_PAWN, PIECE_WHITE_QUEEN);

    if(move_is_legal(position, move))
    {
      moves_white_pawn_promote_pieces_create(move_array, move);
    }
  }
}

/*
 * Create one of two capture moves for a white pawn
 *
 * If no attacks exist, no capture move is created
 */
static void move_white_pawn_capture_create(MoveArray* move_array, Position position, Square pawn_square, U64* attacks)
{
  if(!(*attacks)) return;

  Square target_square = board_first_square_get(*attacks);


  Move move = move_normal_create(position, pawn_square, target_square, PIECE_WHITE_PAWN);

  if(target_square == position.passant) move |= MOVE_MASK_PASSANT;

  move_add_if_legal(move_array, position, move);


  *attacks = BOARD_SQUARE_POP(*attacks, target_square);
}

/*
 * Create legal moves for a white pawn, that do not promote
 */
static void moves_white_pawn_other_create(MoveArray* move_array, Position position, Square pawn_square)
{
  Move move = move_normal_create(position, pawn_square, pawn_square - BOARD_FILES, PIECE_WHITE_PAWN);

  move_add_if_legal(move_array, position, move);

  // Get attack squares by looking up pawn attacks and
  // checking if a black piece is there
  U64 attacks = attacks_pawn_get(pawn_square, SIDE_WHITE);

  // If the pawn is not on 5th rank,
  // exclude all squares where there are no black pieces
  if(!(pawn_square >= A5 && pawn_square <= H5))
  {
    attacks &= position.covers[SIDE_BLACK];
  }

  move_white_pawn_capture_create(move_array, position, pawn_square, &attacks);

  move_white_pawn_capture_create(move_array, position, pawn_square, &attacks);


  // If pawn is standing on 2nd rank, create double jump move
  if(pawn_square >= A2 && pawn_square <= H2)
  {
    Move move = move_double_create(pawn_square, pawn_square - (BOARD_FILES * 2), PIECE_WHITE_PAWN);

    move_add_if_legal(move_array, position, move);
  }
}

/*
 * Create legal moves for a white pawn
 */
static void moves_white_pawn_create(MoveArray* move_array, Position position, Square pawn_square)
{
  if(pawn_square >= A7 && pawn_square <= H7)
  {
    moves_white_pawn_promote_create(move_array, position, pawn_square);
  }
  else
  {
    moves_white_pawn_other_create(move_array, position, pawn_square);
  }
}

/*
 * Create the different promote moves for a black pawn
 */
static void moves_black_pawn_promote_pieces_create(MoveArray* move_array, Move move)
{
  for(Piece piece = PIECE_BLACK_KNIGHT; piece <= PIECE_BLACK_QUEEN; piece++)
  {
    // Make this into a macro, or edit MOVE_PROMOTE_SET to implement this
    move = (move & ~MOVE_MASK_PROMOTE) | MOVE_PROMOTE_SET(piece);

    move_array->moves[move_array->amount++] = move;
  }
}

/*
 * Create legal moves for a black pawn, which promotes
 *
 * If a pawn is standing on H2, pawn_square + 9 will result in unsigned underflow
 */
static void moves_black_pawn_promote_create(MoveArray* move_array, Position position, Square pawn_square)
{
  Square right_square = ((int) pawn_square + 9) > 0 ? (pawn_square + 9) : 0;
  Square left_square  = pawn_square + 7;

  for(Square promote_square = left_square; promote_square <= right_square; promote_square++)
  {
    Move move = move_promote_create(position, pawn_square, promote_square, PIECE_BLACK_PAWN, PIECE_BLACK_QUEEN);

    if(move_is_legal(position, move))
    {
      moves_black_pawn_promote_pieces_create(move_array, move);
    }
  }
}

/*
 * Create one of two capture moves for a black pawn
 *
 * If no attacks exist, no capture move is created
 */
static void move_black_pawn_capture_create(MoveArray* move_array, Position position, Square pawn_square, U64* attacks)
{
  if(!(*attacks)) return;

  Square target_square = board_first_square_get(*attacks);


  Move move = move_normal_create(position, pawn_square, target_square, PIECE_BLACK_PAWN);

  if(target_square == position.passant) move |= MOVE_MASK_PASSANT;

  move_add_if_legal(move_array, position, move);


  *attacks = BOARD_SQUARE_POP(*attacks, target_square);
}

/*
 * Create legal moves for a black pawn, that do not promote
 */
static void moves_black_pawn_other_create(MoveArray* move_array, Position position, Square pawn_square)
{
  Move move = move_normal_create(position, pawn_square, pawn_square + BOARD_FILES, PIECE_BLACK_PAWN);

  move_add_if_legal(move_array, position, move);

  // Get attack squares by looking up pawn attacks and
  // checking if a black piece is there
  U64 attacks = attacks_pawn_get(pawn_square, SIDE_BLACK);

  // If the pawn is not on 4th rank,
  // exclude all squares where there are no white pieces
  if(!(pawn_square >= A4 && pawn_square <= H4))
  {
    attacks &= position.covers[SIDE_WHITE];
  }

  move_black_pawn_capture_create(move_array, position, pawn_square, &attacks);

  move_black_pawn_capture_create(move_array, position, pawn_square, &attacks);


  // If pawn is standing on 7th rank, create double jump move
  if(pawn_square >= A7 && pawn_square <= H7)
  {
    Move move = move_double_create(pawn_square, pawn_square + (BOARD_FILES * 2), PIECE_BLACK_PAWN);

    move_add_if_legal(move_array, position, move);
  }
}

/*
 * Create legal moves for a black pawn
 */
static void moves_black_pawn_create(MoveArray* move_array, Position position, Square pawn_square)
{
  if(pawn_square >= A2 && pawn_square <= H2)
  {
    moves_black_pawn_promote_create(move_array, position, pawn_square);
  }
  else
  {
    moves_black_pawn_other_create(move_array, position, pawn_square);
  }
}

/*
 * Create legal castling move for the white king
 */
static void moves_white_castle_create(MoveArray* move_array, Position position)
{
  Move move;

  if(position.castle & CASTLE_WHITE_QUEEN)
  {
    move = move_castle_create(E1, C1, PIECE_WHITE_KING);

    move_add_if_legal(move_array, position, move);
  }

  if(position.castle & CASTLE_WHITE_KING)
  {
    move = move_castle_create(E1, G1, PIECE_WHITE_KING);

    move_add_if_legal(move_array, position, move);
  }
}

/*
 * Create legal castling move for the black king
 */
static void moves_black_castle_create(MoveArray* move_array, Position position)
{
  Move move;

  if(position.castle & CASTLE_BLACK_QUEEN)
  {
    move = move_castle_create(E8, C8, PIECE_BLACK_KING);

    move_add_if_legal(move_array, position, move);
  }
  
  if(position.castle & CASTLE_BLACK_KING)
  {
    move = move_castle_create(E8, G8, PIECE_BLACK_KING);

    move_add_if_legal(move_array, position, move);
  }
}

/*
 * Create legal moves for pieces, except pawns
 */
static void moves_normal_create(MoveArray* move_array, Position position, Square source_square, Piece piece)
{
  U64 target_board = attacks_get(source_square, position);

  while(target_board)
  {
    Square target_square = board_first_square_get(target_board);

    Move move = move_normal_create(position, source_square, target_square, piece);
    
    move_add_if_legal(move_array, position, move);

    target_board = BOARD_SQUARE_POP(target_board, target_square);
  }
}

/*
 * Create legal moves for white pieces, except pawns
 */
static void moves_white_normal_create(MoveArray* move_array, Position position, Square source_square, Piece piece)
{
  moves_normal_create(move_array, position, source_square, piece);

  if(piece == PIECE_WHITE_KING && source_square == E1)
  {
    moves_white_castle_create(move_array, position);
  }
}

/*
 * Create legal moves for white
 */
static void moves_white_create(MoveArray* move_array, Position position)
{
  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_WHITE_KING; piece++)
  {
    U64 piece_board = position.boards[piece];

    while(piece_board)
    {
      Square source_square = board_first_square_get(piece_board);

      if(piece == PIECE_WHITE_PAWN)
      {
        moves_white_pawn_create(move_array, position, source_square);
      }
      else
      {
        moves_white_normal_create(move_array, position, source_square, piece);
      }

      piece_board = BOARD_SQUARE_POP(piece_board, source_square);
    }
  }
}

/*
 * Create legal moves for black pieces, except pawns
 */
static void moves_black_normal_create(MoveArray* move_array, Position position, Square source_square, Piece piece)
{
  moves_normal_create(move_array, position, source_square, piece);

  if(piece == PIECE_BLACK_KING && source_square == E8)
  {
    moves_black_castle_create(move_array, position);
  }
}

/*
 * Create legal moves for black
 */
static void moves_black_create(MoveArray* move_array, Position position)
{
  for(Piece piece = PIECE_BLACK_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    U64 piece_board = position.boards[piece];

    while(piece_board)
    {
      Square source_square = board_first_square_get(piece_board);

      if(piece == PIECE_BLACK_PAWN)
      {
        moves_black_pawn_create(move_array, position, source_square);
      }
      else
      {
        moves_black_normal_create(move_array, position, source_square, piece);
      }

      piece_board = BOARD_SQUARE_POP(piece_board, source_square);
    }
  }
}

/*
 * Create legal moves for the specified position
 */
void moves_create(MoveArray* move_array, Position position)
{
  if(position.side == SIDE_WHITE)
  {
    moves_white_create(move_array, position);
  }
  else
  {
    moves_black_create(move_array, position);
  }
}
