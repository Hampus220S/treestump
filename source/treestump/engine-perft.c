/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-13
 */

#include "../treestump.h"

#include "engine-intern.h"

/*
 *
 */
static U64 perft_driver(Position position, int depth)
{
  if(depth <= 0) return 1;

  MoveArray moveArray;

  memset(moveArray.moves, 0, sizeof(moveArray.moves));
  moveArray.amount = 0;

  moves_create(&moveArray, position);

  U64 nodes = 0;

  for(int index = 0; index < moveArray.amount; index++)
  {
    Position positionCopy = position;

    move_make(&positionCopy, moveArray.moves[index]);

    nodes += perft_driver(positionCopy, depth - 1);
  }

  return nodes;
}

/*
 *
 */
void perft_test(Position position, int depth)
{
  MoveArray moveArray;

  memset(moveArray.moves, 0, sizeof(moveArray.moves));
  moveArray.amount = 0;

  moves_create(&moveArray, position);

  char moveString[8];

  U64 totalNodes = 0;

  for(int index = 0; index < moveArray.amount; index++)
  {
    Position positionCopy = position;

    move_make(&positionCopy, moveArray.moves[index]);

    U64 moveNodes = perft_driver(positionCopy, depth - 1);

    totalNodes += moveNodes;

    memset(moveString, 0, sizeof(moveString));

    move_string_create(moveString, moveArray.moves[index]);

    printf("%s: %llu\n", moveString, moveNodes);
  }

  printf("\nNodes searched: %llu\n", totalNodes);
}
