/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "../treestump.h"

typedef struct
{
  Move moves[256];
  int amount;
} MoveArray;

extern void perft_test(Position position, int depth);

extern Move best_move(Position position, int depth, int nodes, int movetime, MoveArray searchmoves);

#endif // ENGINE_H
