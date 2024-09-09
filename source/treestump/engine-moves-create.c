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
void create_white_pawn_promote_moves(MoveArray* moveArray, Position position, Square sourceSquare)
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
void create_white_pawn_moves(MoveArray* moveArray, Position position, Square sourceSquare)
{
  if(sourceSquare >= A7 && sourceSquare <= H7)
  {
    create_white_pawn_promote_moves(moveArray, position, sourceSquare);
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
void create_black_pawn_promote_moves(MoveArray* moveArray, Position position, Square sourceSquare)
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
void create_black_pawn_moves(MoveArray* moveArray, Position position, Square sourceSquare)
{
  if(sourceSquare >= A2 && sourceSquare <= H2)
  {
    create_black_pawn_promote_moves(moveArray, position, sourceSquare);
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
void create_white_castle_moves(MoveArray* moveArray, Position position)
{
  Move move = move_castle_create(E1, C1, PIECE_WHITE_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

  move = move_castle_create(E1, G1, PIECE_WHITE_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
}

/*
 *
 */
void create_black_castle_moves(MoveArray* moveArray, Position position)
{
  Move move = move_castle_create(E8, C8, PIECE_BLACK_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

  move = move_castle_create(E8, G8, PIECE_BLACK_KING);

  if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;
}

/*
 *
 */
void create_white_normal_moves(MoveArray* moveArray, Position position, Square sourceSquare, Piece piece)
{
  if(piece == PIECE_WHITE_KING && sourceSquare == E1) create_white_castle_moves(moveArray, position);

  
  U64 attackBoard = attacks_get(sourceSquare, position);

  while(attackBoard)
  {
    Square targetSquare = board_ls1b_index(attackBoard);

    Move move = move_normal_create(position.boards, sourceSquare, targetSquare, piece);
    
    if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

    attackBoard = BOARD_SQUARE_POP(attackBoard, targetSquare);
  }
}

/*
 *
 */
void create_white_moves(MoveArray* moveArray, Position position)
{
  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_WHITE_KING; piece++)
  {
    U64 pieceBoard = position.boards[piece];

    while(pieceBoard)
    {
      Square sourceSquare = board_ls1b_index(pieceBoard);

      if(piece == PIECE_WHITE_PAWN) create_white_pawn_moves(moveArray, position, sourceSquare);

      else create_white_normal_moves(moveArray, position, sourceSquare, piece);

      pieceBoard = BOARD_SQUARE_POP(pieceBoard, sourceSquare);
    }
  }
}

/*
 *
 */
void create_black_normal_moves(MoveArray* moveArray, Position position, Square sourceSquare, Piece piece)
{
  if(piece == PIECE_BLACK_KING && sourceSquare == E8) create_black_castle_moves(moveArray, position);
  

  U64 attackBoard = attacks_get(sourceSquare, position);

  while(attackBoard)
  {
    Square targetSquare = board_ls1b_index(attackBoard);

    Move move = move_normal_create(position.boards, sourceSquare, targetSquare, piece);
    
    if(move_is_legal(position, move)) moveArray->moves[moveArray->amount++] = move;

    attackBoard = BOARD_SQUARE_POP(attackBoard, targetSquare);
  }
}

/*
 *
 */
void create_black_moves(MoveArray* moveArray, Position position)
{
  for(Piece piece = PIECE_BLACK_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    U64 pieceBoard = position.boards[piece];

    while(pieceBoard)
    {
      Square sourceSquare = board_ls1b_index(pieceBoard);

      if(piece == PIECE_BLACK_PAWN) create_black_pawn_moves(moveArray, position, sourceSquare);
      
      else create_black_normal_moves(moveArray, position, sourceSquare, piece);

      pieceBoard = BOARD_SQUARE_POP(pieceBoard, sourceSquare);
    }
  }
}

/*
 *
 */
void create_moves(MoveArray* moveArray, Position position)
{
  if(position.side == SIDE_WHITE) create_white_moves(moveArray, position);
  
  else if(position.side == SIDE_BLACK) create_black_moves(moveArray, position);
}
