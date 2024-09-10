/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "engine-intern.h"

/*
 *
 */
static void moves_white_pawn_promote_create(MoveArray* moveArray, Position position, Square sourceSquare)
{
  for(Square targetSquare = (sourceSquare - 9); targetSquare <= (sourceSquare - 7); targetSquare++)
  {
    Move move = move_promote_create(position.boards, sourceSquare, targetSquare, PIECE_WHITE_PAWN, PIECE_WHITE_QUEEN);

    if(!move_is_legal(position, move)) continue;

    for(Piece promotePiece = PIECE_WHITE_KNIGHT; promotePiece <= PIECE_WHITE_QUEEN; promotePiece++)
    {
      move = (move & ~MOVE_MASK_PROMOTE) | MOVE_PROMOTE_SET(promotePiece);

      moveArray->moves[moveArray->amount++] = move;
    }
  }
}

/*
 *
 */
static void moves_white_pawn_create(MoveArray* moveArray, Position position, Square sourceSquare)
{
  if(sourceSquare >= A7 && sourceSquare <= H7)
  {
    moves_white_pawn_promote_create(moveArray, position, sourceSquare);
  }
  else // Rewrite this code
  {
    for(Square targetSquare = (sourceSquare - 9); targetSquare <= (sourceSquare - 7); targetSquare++)
    {
      Move move = move_normal_create(position.boards, sourceSquare, targetSquare, PIECE_WHITE_PAWN);

      if(targetSquare == position.passant) move |= MOVE_MASK_PASSANT;

      if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
    }

    if(sourceSquare >= A2 && sourceSquare <= H2)
    {
      Move move = move_double_create(sourceSquare, sourceSquare - (BOARD_FILES * 2), PIECE_WHITE_PAWN);

      if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
    }
  }
}

/*
 *
 */
static void moves_black_pawn_promote_create(MoveArray* moveArray, Position position, Square sourceSquare)
{
  for(Square targetSquare = (sourceSquare + 7); targetSquare <= (sourceSquare + 9); targetSquare++)
  {
    Move move = move_promote_create(position.boards, sourceSquare, targetSquare, PIECE_BLACK_PAWN, PIECE_BLACK_QUEEN);

    if(!move_is_legal(position, move)) continue;

    for(Piece promotePiece = PIECE_BLACK_KNIGHT; promotePiece <= PIECE_BLACK_QUEEN; promotePiece++)
    {
      move = (move & ~MOVE_MASK_PROMOTE) | MOVE_PROMOTE_SET(promotePiece);

      moveArray->moves[moveArray->amount++] = move;
    }
  }
}

/*
 *
 */
static void moves_black_pawn_create(MoveArray* moveArray, Position position, Square sourceSquare)
{
  if(sourceSquare >= A2 && sourceSquare <= H2)
  {
    moves_black_pawn_promote_create(moveArray, position, sourceSquare);
  }
  else // Rewrite this code
  {
    for(Square targetSquare = (sourceSquare + 7); targetSquare <= (sourceSquare + 9); targetSquare++)
    {
      Move move = move_normal_create(position.boards, sourceSquare, targetSquare, PIECE_BLACK_PAWN);

      if(targetSquare == position.passant) move |= MOVE_MASK_PASSANT;

      if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
    }

    if(sourceSquare >= A7 && sourceSquare <= H7)
    {
      Move move = move_double_create(sourceSquare, sourceSquare + (BOARD_FILES * 2), PIECE_BLACK_PAWN);

      if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
    }
  }
}

/*
 *
 */
static void moves_white_castle_create(MoveArray* moveArray, Position position)
{
  Move move = move_castle_create(E1, C1, PIECE_WHITE_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

  move = move_castle_create(E1, G1, PIECE_WHITE_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
}

/*
 *
 */
static void moves_black_castle_create(MoveArray* moveArray, Position position)
{
  Move move = move_castle_create(E8, C8, PIECE_BLACK_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

  move = move_castle_create(E8, G8, PIECE_BLACK_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
}

/*
 *
 */
static void moves_white_normal_create(MoveArray* moveArray, Position position, Square sourceSquare, Piece piece)
{
  if(piece == PIECE_WHITE_KING && sourceSquare == E1)
  {
    moves_white_castle_create(moveArray, position);
  }
  
  U64 attackBoard = attacks_get(sourceSquare, position);

  while(attackBoard)
  {
    Square targetSquare = board_first_square_get(attackBoard);

    Move move = move_normal_create(position.boards, sourceSquare, targetSquare, piece);
    
    if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

    attackBoard = BOARD_SQUARE_POP(attackBoard, targetSquare);
  }
}

/*
 *
 */
static void moves_white_create(MoveArray* moveArray, Position position)
{
  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_WHITE_KING; piece++)
  {
    U64 pieceBoard = position.boards[piece];

    while(pieceBoard)
    {
      Square sourceSquare = board_first_square_get(pieceBoard);

      if(piece == PIECE_WHITE_PAWN)
      {
        moves_white_pawn_create(moveArray, position, sourceSquare);
      }
      else
      {
        moves_white_normal_create(moveArray, position, sourceSquare, piece);
      }

      pieceBoard = BOARD_SQUARE_POP(pieceBoard, sourceSquare);
    }
  }
}

/*
 *
 */
static void moves_black_normal_create(MoveArray* moveArray, Position position, Square sourceSquare, Piece piece)
{
  if(piece == PIECE_BLACK_KING && sourceSquare == E8)
  {
    moves_black_castle_create(moveArray, position);
  }

  U64 attackBoard = attacks_get(sourceSquare, position);

  while(attackBoard)
  {
    Square targetSquare = board_first_square_get(attackBoard);

    Move move = move_normal_create(position.boards, sourceSquare, targetSquare, piece);
    
    if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

    attackBoard = BOARD_SQUARE_POP(attackBoard, targetSquare);
  }
}

/*
 *
 */
static void moves_black_create(MoveArray* moveArray, Position position)
{
  for(Piece piece = PIECE_BLACK_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    U64 pieceBoard = position.boards[piece];

    while(pieceBoard)
    {
      Square sourceSquare = board_first_square_get(pieceBoard);

      if(piece == PIECE_BLACK_PAWN)
      {
        moves_black_pawn_create(moveArray, position, sourceSquare);
      }
      else
      {
        moves_black_normal_create(moveArray, position, sourceSquare, piece);
      }

      pieceBoard = BOARD_SQUARE_POP(pieceBoard, sourceSquare);
    }
  }
}

/*
 * Create legal moves for the specified position
 */
void moves_create(MoveArray* moveArray, Position position)
{
  if(position.side == SIDE_WHITE)
  {
    moves_white_create(moveArray, position);
  }
  else
  {
    moves_black_create(moveArray, position);
  }
}
