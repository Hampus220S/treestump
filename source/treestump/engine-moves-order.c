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
static void moves_swap(Move* moves, int index1, int index2)
{
  Move temp_move = moves[index1];
  moves[index1]  = moves[index2];
  moves[index2]  = temp_move;
}

/*
 *
 */
static void scores_swap(int* scores, int index1, int index2)
{
  int temp_score = scores[index1];
  scores[index1] = scores[index2];
  scores[index2] = temp_score;
}

/*
 *
 */
static void moves_and_scores_swap(Move* moves, int* scores, int index1, int index2)
{
  moves_swap (moves,  index1, index2);

  scores_swap(scores, index1, index2);
}

/*
 *
 */
static int moves_and_scores_partly_sort(Move* moves, int* scores, int index1, int index2)
{
  int pivot_score = scores[index2];

  int i_index = (index1 - 1);

  for(int j_index = index1; j_index <= (index2 - 1); j_index += 1)
  {
    if(scores[j_index] < pivot_score) continue;

    moves_and_scores_swap(moves, scores, (++i_index), j_index);
  }

  moves_and_scores_swap(moves, scores, (i_index + 1), index2);

  return (i_index + 1);
}

/*
 *
 */
static void moves_and_scores_betwen_indexes_sort(Move* moves, int* scores, int index1, int index2)
{
  if(index1 >= index2) return;

  int partIndex = moves_and_scores_partly_sort(moves, scores, index1, index2);

  moves_and_scores_betwen_indexes_sort(moves, scores, index1, (partIndex - 1));

  moves_and_scores_betwen_indexes_sort(moves, scores, (partIndex + 1), index2);
}

/*
 *
 */
static void moves_and_scores_sort(MoveArray* moveArray, int* scores)
{
  moves_and_scores_betwen_indexes_sort(moveArray->moves, scores, 0, (moveArray->amount - 1));
}

/*
 *
 */
static int move_score_guess(Position position, Move move)
{
  int score = 0;

  Square targetSquare = MOVE_TARGET_GET(move);
  Piece targetPiece = square_piece_get(position.boards, targetSquare);

  if(targetPiece != PIECE_NONE)
  {
    Piece sourcePiece = MOVE_PIECE_GET(move);

    int sourcePieceScore = PIECE_SCORES[sourcePiece];
    int targetPieceScore = PIECE_SCORES[targetPiece];

    score += ((10 * sourcePieceScore) - targetPieceScore);
  }

  Piece promotePiece = MOVE_PROMOTE_GET(move);

  if(promotePiece != PIECE_WHITE_PAWN)
  {
    score += PIECE_SCORES[promotePiece];
  }

  return (position.side == SIDE_WHITE) ? score : -score;
}

/*
 *
 */
static void move_scores_guess(int* scores, Position position, MoveArray moveArray)
{
  for(int index = 0; index < moveArray.amount; index++)
  {
    scores[index] = move_score_guess(position, moveArray.moves[index]);
  }
}

/*
 * Order a list of moves based on calculated guesses about the moves
 */
void moves_guess_order(MoveArray* moveArray, Position position)
{
  int scores[moveArray->amount];

  move_scores_guess(scores, position, *moveArray);

  moves_and_scores_sort(moveArray, scores);
}
