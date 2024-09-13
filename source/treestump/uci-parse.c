/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "uci-intern.h"

/*
 *
 */
static MoveArray move_strings_parse(Position position, const char moves_string[])
{
  MoveArray moveArray;

  memset(moveArray.moves, MOVE_NONE, sizeof(moveArray.moves));
  moveArray.amount = 0;

  while(*moves_string)
  {
    Move move = move_string_parse(position, moves_string);

    if(move != MOVE_NONE)
    {
      moveArray.moves[moveArray.amount++] = move;
    }

    while(*moves_string && *moves_string != ' ') moves_string++;

    moves_string++;
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

    if(args.debug) info_print("Start of perft");

    perft_test(position, depth);

    if(args.debug) info_print("End of perft");

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
    searchmoves = move_strings_parse(position, goString + 12);
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
  move_string_create(moveString, bestMove);

  printf("bestmove %s\n", moveString);
}

/*
 * RETURN (same as fen_parse, int status)
 * - 0 | Success
 * - 1 | Failed to parse fen
 */
static int uci_position_fen_parse(Position* position, const char* position_string)
{
  if(strncmp(position_string, "startpos", 8) == 0)
  {
    return fen_parse(position, FEN_START);
  }
  else if(strncmp(position_string, "fen", 3) == 0)
  {
    return fen_parse(position, position_string + 4);
  }
  else return 1;
}

/*
 * RETURN (int status)
 * - 0 | Success
 * - 1 | Failed to parse move
 */
static int uci_position_moves_parse(Position* position, char* moves_string)
{
  while(*moves_string)
  {
    Move move = move_string_parse(*position, moves_string);

    if(move == MOVE_NONE)
    {
      return 1;
    }

    move_make(position, move);

    while(*moves_string && *moves_string != ' ') moves_string++;

    moves_string++;
  }

  return 0;
}

/*
 * RETURN (int status)
 * - 0 | Success
 * - 1 | Failed to parse position fen
 * - 2 | Failed to parse position moves
 */
static int uci_position_parse(Position* position, const char* position_string)
{
  Position temp_position;

  if(uci_position_fen_parse(&temp_position, position_string) != 0)
  {
    if(args.debug) error_print("Failed to parse position fen");

    return 1;
  }

  char* moves_string = strstr(position_string, "moves");

  if(moves_string != NULL)
  {
    if(uci_position_moves_parse(&temp_position, moves_string + 6) != 0)
    {
      if(args.debug) error_print("Failed to parse position moves");

      return 2;
    }
  }

  *position = temp_position;

  return 0;
}

/*
 *
 */
static void uci_uci_handler(void)
{
  printf("id name TreeStump\n");
  printf("id author Hampus Fridholm\n");
  printf("uciok\n");
}

/*
 *
 */
static void uci_isready_handler(void)
{
  printf("readyok\n");
}

/*
 *
 */
static void uci_stop_handler(void)
{
  printf("bestmove a1a1\n");
}

/*
 *
 */
static void uci_ucinewgame_handler(void)
{

}

/*
 *
 */
static void uci_ponderhit_handler(void)
{

}

/*
 *
 */
static void uci_quit_handler(void)
{

}

/*
 *
 */
static void uci_help_handler(void)
{
  // Read this from file and print it out
  printf("\ntreestump is a powerful chess engine for playing and analyzing.\n\
It is released as free software licensed under the GNU GPLv3 License.\n\
Stockfish is normally used with a graphical user interface (GUI) and implements\n\
the Universal Chess Interface (UCI) protocol to communicate with a GUI, an API, etc.\n\
For any further information, visit https://github.com/official-stockfish/Stockfish#readme\n\
or read the corresponding README.md and Copying.txt files distributed along with this program.\n\n");
}

/*
 * Parse UCI command string and update position
 *
 * PARAMS
 * - Position*   position   | Current board position
 * - const char* uci_string | UCI command string
 *
 * RETURN (int status)
 * - 0 | Success
 * - 1 | Bad input
 * - 2 | Unknown command
 */
int uci_parse(Position* position, const char* uci_string)
{
  if(!position || !uci_string) return 1;

  if(strcmp(uci_string, "uci") == 0)
  {
    uci_uci_handler();
  }
  else if(strncmp(uci_string, "position", 8) == 0)
  {
    uci_position_parse(position, uci_string + 9);
  }
  else if(strncmp(uci_string, "go", 2) == 0)
  {
    uci_go_parse(*position, uci_string + 3);
  }
  else if(strcmp(uci_string, "d") == 0)
  {
    position_print(*position);

    /*
    for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_BLACK_KING; piece++)
    {
      printf("piece: %c\n", PIECE_SYMBOLS[piece]);
      bitboard_print(position->boards[piece]);
    }

    for(Side side = SIDE_WHITE; side <= SIDE_BOTH; side++)
    {
      printf("side: %c\n", SIDE_SYMBOLS[side]);
      bitboard_print(position->covers[side]);
    }
    */
  }
  else if(strncmp(uci_string, "isready", 7) == 0)
  {
    uci_isready_handler();
  }
  else if(strcmp(uci_string, "ucinewgame") == 0)
  {
    uci_ucinewgame_handler();
  }
  else if(strcmp(uci_string, "stop") == 0)
  {
    uci_stop_handler();
  }
  else if(strcmp(uci_string, "ponderhit") == 0)
  {
    uci_ponderhit_handler();
  }
  else if(strcmp(uci_string, "quit") == 0)
  {
    uci_quit_handler();
  }
  else if(strcmp(uci_string, "help") == 0)
  {
    uci_help_handler();
  }
  else
  {
    printf("Unknown command: '%s'. Type help for more information.\n", uci_string);

    return 2;
  }

  return 0;
}
