/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

#include "uci-intern.h"

/*
 * Split a string at deliminator into multiple smaller string parts
 *
 * PARAMS
 * - char** string_array | array to store splitted strings to
 *   - the array should be allocated with size >= split_amount
 *
 * RETURN (size_t count)
 * - number of strings successfully splitted
 */
static size_t string_split_at_delim(char** string_array, const char* string, const char* delim, size_t split_amount)
{
  if(!string_array || !string)
  {
    if(args.debug) error_print("String array or string not allocated");

    return 0;
  }

  if(split_amount <= 0) return 0;

  char string_copy[strlen(string) + 1]; 
  strcpy(string_copy, string);

  char* string_token = NULL;
  
  string_token = strtok(string_copy, delim);

  size_t index;

  for(index = 0; (index < split_amount) && string_token; index++)
  {
    string_array[index] = strdup(string_token);

    string_token = strtok(NULL, delim);
  }

  return index;
}

/*
 *
 */
static int fen_side_parse(Side* side, const char* string)
{
  if(strlen(string) != 1) return 1;

  switch(*string)
  {
    case 'w':
      *side = SIDE_WHITE;
      return 0;

    case 'b':
      *side = SIDE_BLACK;
      return 0;

    default:
      return 2;
  }
}

/*
 *
 */
static int fen_clock_parse(int* clock, const char* string)
{
  int number = atoi(string);

  if((number == 0) && (*string != '0')) return 1;

  *clock = number;

  return 0;
}

/*
 *
 */
static int fen_turns_parse(int* turns, const char* string)
{
  int number = atoi(string);

  if((number == 0) && (*string != '0')) return 1;

  *turns = number;

  return 0;
}

/*
 *
 */
static int fen_passant_parse(Square* passant, const char* string)
{
  if(strcmp(string, "-") == 0)
  {
    *passant = SQUARE_NONE;
    
    return 0;
  }

  if(strlen(string) != 2) return 1;


  int file = string[0] - 'a';
  int rank = BOARD_RANKS - (string[1] - '0');

  if(!(file >= 0 && file < BOARD_FILES) || !(rank >= 0 && rank < BOARD_RANKS))
  {
    return 2;
  }

  *passant = (rank * BOARD_FILES) + file;

  return 0;
}

/*
 *
 */
static int fen_castle_parse(Castle* castle, const char* string)
{
  if(strcmp(string, "-") == 0) return 0;

  size_t string_len = strlen(string);

  for(size_t index = 0; index < string_len; index++)
  {
    switch(string[index])
    {
      case 'K':
        *castle |= CASTLE_WHITE_KING;
        break;

      case 'Q':
        *castle |= CASTLE_WHITE_QUEEN;
        break;

      case 'k':
        *castle |= CASTLE_BLACK_KING;
        break;

      case 'q':
        *castle |= CASTLE_BLACK_QUEEN;
        break;

      default:
        return 1;
    }
  }

  return 0;
}

/*
 *
 */
static void string_array_free(char** string_array, size_t string_count)
{
  if(!string_array) return;

  for(size_t index = 0; index < string_count; index++)
  {
    if(string_array[index]) free(string_array[index]);
  }
}

/*
 * RETURN (int status)
 * - 0 | Success
 * - 1 | Failed to split fen board
 */
static int fen_boards_parse(U64 boards[12], const char string[])
{
  memset(boards, 0ULL, sizeof(U64) * 12);

  char* string_array[8];

  size_t split_count = 0;

  if((split_count = string_split_at_delim(string_array, string, "/", BOARD_RANKS)) < BOARD_RANKS)
  {
    if(args.debug) error_print("Failed to split fen board string");

    string_array_free(string_array, split_count);

    return 1;
  }

  for(int rank = 0; rank < BOARD_RANKS; rank++)
  {
    char*  rank_string        = string_array[rank];
    size_t rank_string_length = strlen(rank_string);

    // Future Fix: check if rank length is less or equal to 8
    // and if there is any symbols that should not be in fen string
    int file = 0;

    for(int index = 0; index < rank_string_length; index++)
    {
      char symbol = rank_string[index];


      Square square = (rank * BOARD_FILES) + file;

      if((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z'))
      {
        Piece piece = SYMBOL_PIECES[symbol];

        boards[piece] = BOARD_SQUARE_SET(boards[piece], square);

        file++;
      }
      else if(symbol >= '0' && symbol <= '9')
      {
        file += (symbol - '0');
      }
      else
      {
        if(args.debug) error_print("Unknown fen symbol: (%c)", symbol);
      }
    }
  }

  string_array_free(string_array, split_count);

  return 0;
}

/*
 *
 */
static bool fen_index_part_parse(Position* position, int index, const char* string)
{
  switch(index)
  {
    case 0:
      if(fen_boards_parse(position->boards, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen board");

        return false;
      }
      return true;

    case 1:
      if(fen_side_parse(&position->side, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen side");

        return false;
      }
      return true;

    case 2:
      if(fen_castle_parse(&position->castle, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen castle");

        return false;
      }
      return true;

    case 3:
      if(fen_passant_parse(&position->passant, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen passant");

        return false;
      }
      return true;

    case 4:
      if(fen_clock_parse(&position->clock, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen clock");

        return false;
      }
      return true;

    case 5:
      if(fen_turns_parse(&position->turns, string) != 0)
      {
        if(args.debug) error_print("Failed to parse fen turns");

        return false;
      }
      return true;

    default:
      return false;
  }
}

/*
 * Parse fen string
 *
 * RETURN (int status)
 * - 0 | Success
 * - 1 | Failed to split fen string
 */
int fen_parse(Position* position, const char* fen_string)
{
  if(args.debug) info_print("Parsing fen (%s)", fen_string);

  char* string_array[6];

  size_t split_count = 0;

  if((split_count = string_split_at_delim(string_array, fen_string, " ", 6)) < 4)
  {
    if(args.debug) error_print("Failed to split fen string");

    string_array_free(string_array, split_count);

    return 1;
  }

  Position temp_position;

  for(int index = 0; index < split_count; index++)
  {
    if(!fen_index_part_parse(&temp_position, index, string_array[index]))
    {
      string_array_free(string_array, split_count);

      return 2;
    }
  }

  *position = temp_position;

  string_array_free(string_array, split_count);


  memset(position->covers, 0ULL, sizeof(U64) * 3);


  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_WHITE_KING; piece++)
  {
    position->covers[SIDE_WHITE] |= position->boards[piece];
  }

  for(Piece piece = PIECE_BLACK_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    position->covers[SIDE_BLACK] |= position->boards[piece];
  }

  position->covers[SIDE_BOTH] = position->covers[SIDE_WHITE] | position->covers[SIDE_BLACK];


  if(args.debug) info_print("Parsed fen");

  return 0;
}
