/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

extern U64 searchedNodes;

/*
 *
 */
static MoveArray string_moves(U64 boards[12], const char movesString[])
{
  MoveArray moveArray;

  memset(moveArray.moves, MOVE_NONE, sizeof(moveArray.moves));
  moveArray.amount = 0;

  while(*movesString)
  {
    Move parsedMove = string_move(movesString);

    // if(parsedMove == 0) break;

    if(parsedMove != MOVE_NONE)
    {
      Move move = complete_move(boards, parsedMove);

      moveArray.moves[moveArray.amount++] = move;
    }

    while(*movesString && *movesString != ' ') movesString++;

    movesString++;
  }
  return moveArray;
}

/*
 *
 */
static void uci_go_parse(Position position, const char goString[])
{
  if(!strncmp(goString, "perft", 5))
  {
    int depth = atoi(goString + 6);

    perft_test(position, depth);

    return;
  }

  int depth = 4;
  int nodes = -1;
  int movetime = -1;

  MoveArray searchmoves;

  memset(searchmoves.moves, MOVE_NONE, sizeof(searchmoves.moves));
  searchmoves.amount = 0;


  char* string;
  
  if((string = strstr(goString, "searchmoves")))
  {
    // Search only on these moves
    searchmoves = string_moves(position.boards, goString + 12);
  }
  if(!strncmp(goString, "ponder", 5))
  {
    // This is not implemented
  }
  if((string = strstr(goString, "depth")))
  {
    // Search x plies only
    depth = atoi(string + 6);
  } 
  if((string = strstr(goString, "nodes")))
  {
    // Search x nodes only
    nodes = atoi(string + 6);
  }
  if((string = strstr(goString, "mate")))
  {
    // Search for a mate in x moves
  }
  if((string = strstr(goString, "movetime")))
  {
    // Search exactly x milliseconds
    movetime = atoi(string + 9);
  }
  if((string = strstr(goString, "infinite")))
  {
    // This is not implemented
  }

  // int time = -1;
  // int inc = -1;
  
  if((string = strstr(goString, "wtime")) && position.side == SIDE_WHITE)
  {
    // White has x milliseconds left on the clock
    // time = atoi(string + 6);
  }
  else if((string = strstr(goString, "btime")) && position.side == SIDE_BLACK)
  {
    // Black has x milliseconds left on the clock
    // time = atoi(string + 6);
  }

  if((string = strstr(goString, "winc")) && position.side == SIDE_WHITE)
  {
    // White's increment per move in milliseconds
    // inc = atoi(string + 5);
  }
  else if((string = strstr(goString, "binc")) && position.side == SIDE_BLACK)
  {
    // Blacks's increment per move in milliseconds
    // inc = atoi(string + 5);
  }

  // printf("best_move(%d, %d, %d)\n", depth, nodes, movetime);

  Move bestMove = best_move(position, depth, nodes, movetime, searchmoves);

  char moveString[8];
  move_string(moveString, bestMove);

  printf("bestmove %s\n", moveString);
}

/*
 *
 */
static Position uci_position_parse(const char positionString[])
{
  Position position;

  if(!strncmp(positionString, "startpos", 8))
  {
    fen_parse(&position, FEN_START);
  }
  else if(!strncmp(positionString, "fen", 3))
  {
    fen_parse(&position, positionString + 4);
  }
  else fen_parse(&position, FEN_START);


  char* movesString = strstr(positionString, "moves");

  if(movesString != NULL)
  {
    movesString += 6;

    while(*movesString)
    {
      Move parsedMove = string_move(movesString);

      if(parsedMove == MOVE_NONE) break;

      Move move = complete_move(position.boards, parsedMove);

      make_move(&position, move);

      while(*movesString && *movesString != ' ') movesString++;

      movesString++;
    }
  }
  return position;
}

/*
 *
 */
void uci_parse(Position* position, const char uciString[])
{
  if(!strcmp(uciString, "uci"))
  {
    printf("id name TreeStump\n");
    printf("id author Hampus Fridholm\n");
    printf("uciok\n");
  }
  else if(!strncmp(uciString, "position", 8))
  {
    *position = uci_position_parse(uciString + 9);
  }
  else if(!strncmp(uciString, "go", 2))
  {
    uci_go_parse(*position, uciString + 3);
  }
  else if(!strcmp(uciString, "d"))
  {
    position_print(*position);
  }
  else if(!strncmp(uciString, "isready", 7))
  {
    printf("readyok\n");
  }
  else if(!strncmp(uciString, "debug", 5))
  {

  }
  else if(!strcmp(uciString, "ucinewgame"))
  {

  }
  else if(!strcmp(uciString, "stop"))
  {
    printf("bestmove a1a1\n");
  }
  else if(!strcmp(uciString, "ponderhit"))
  {

  }
}
