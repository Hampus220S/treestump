/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "engine-intern.h"

U64 searchedNodes = 0;

/*
 *
 */
static int negamax(Position position, int depth, int nodes, int alpha, int beta)
{
  /*
  if(nodes > 0 && searchedNodes >= nodes)
  {
    int score = position_score_get(position);

    return (position.side == SIDE_WHITE) ? score : -score;
  }
  */

  if(depth <= 0)
  {
    searchedNodes++;

    int score = position_score_get(position);

    return (position.side == SIDE_WHITE) ? score : -score;
  }

  int bestScore = -50000;

  MoveArray moveArray;

  memset(moveArray.moves, 0, sizeof(moveArray.moves));
  moveArray.amount = 0;

  moves_create(&moveArray, position);


  if(moveArray.amount <= 0)
  {
    // Put this in a new function
    U64 kingBoard = (position.side == SIDE_WHITE) ? position.boards[PIECE_WHITE_KING] : position.boards[PIECE_BLACK_KING];

    Square kingSquare = board_first_square_get(kingBoard);

    if(kingSquare == SQUARE_NONE || square_is_attacked(position, kingSquare, !position.side))
    {
      return -49000 + depth;
    }
    else return 0; // Draw;
  }


  moves_guess_order(&moveArray, position);


  for(int index = 0; index < moveArray.amount; index++)
  {
    Position positionCopy = position;

    move_make(&positionCopy, moveArray.moves[index]);

    int currentScore = -negamax(positionCopy, (depth - 1), nodes, -beta, -alpha);

    if(currentScore > bestScore) bestScore = currentScore;

    if(bestScore > alpha) alpha = bestScore;

    if(alpha >= beta) break;
  }
  return bestScore;
}

/*
 *
 */
Move best_move(Position position, int depth, int nodes, int movetime, MoveArray searchmoves)
{
  searchedNodes = 0;

  MoveArray moveArray;

  memset(moveArray.moves, 0, sizeof(moveArray.moves));
  moveArray.amount = 0;

  if(searchmoves.amount > 0)
  {
    moveArray = searchmoves;
  }
  else
  {
    moves_create(&moveArray, position);

    if(moveArray.amount <= 0) return MOVE_NONE;
  }


  int bestScore = -50000;
  Move bestMove = moveArray.moves[0];

  for(int index = 0; index < moveArray.amount; index++)
  {
    Position positionCopy = position;

    Move currentMove = moveArray.moves[index];

    move_make(&positionCopy, currentMove);

    int currentScore = -negamax(positionCopy, (depth - 1), nodes, -50000, +50000);

    if(currentScore > bestScore) 
    {
      bestScore = currentScore;
      bestMove = currentMove;
    }
  }

  if(args.debug) info_print("Searched nodes: %d", searchedNodes);

  return bestMove;
}
