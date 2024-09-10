/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "uci-intern.h"

/*
 * Parse a square string, ex e4, to a square object
 */
static Square square_string_parse(const char* string)
{
  int file = string[0] - 'a';
  int rank = BOARD_RANKS - (string[1] - '0');

  if(!(file >= 0 && file < BOARD_FILES) || !(rank >= 0 && rank < BOARD_RANKS)) return SQUARE_NONE;

  return (rank * BOARD_FILES) + file;
}

/*
 * Parse a move string, ex e2e4, to a move object
 */
Move move_string_parse(U64 boards[12], const char* string)
{
  Square sourceSquare = square_string_parse(string += 0);

  if(sourceSquare == SQUARE_NONE) return MOVE_NONE;

  Square targetSquare = square_string_parse(string += 2);

  if(targetSquare == SQUARE_NONE) return MOVE_NONE;


  Piece promotePiece = SYMBOL_PIECES[(unsigned char) *string];


  return move_create(boards, sourceSquare, targetSquare, promotePiece);
}

/*
 * Create a move string, ex e2e4, from a move object
 */
char* move_string_create(char* string, Move move)
{
  const char* sourceString = SQUARE_STRINGS[MOVE_SOURCE_GET(move)];
  const char* targetString = SQUARE_STRINGS[MOVE_TARGET_GET(move)];

  Piece promotePiece = MOVE_PROMOTE_GET(move);

  if(promotePiece == PIECE_WHITE_PAWN)
  {
    sprintf(string, "%s%s", sourceString, targetString);
  }
  else
  {
    sprintf(string, "%s%s%c", sourceString, targetString, PIECE_SYMBOLS[promotePiece]);
  }

  return string;
}

/*
 * Split a string at deliminator into multiple smaller string parts
 */
bool string_split_at_delim(char (*stringArray)[128], const char string[], int length, const char delim[], int amount)
{
  if(amount < 1) return false;

  char stringCopy[length + 1]; 
  strcpy(stringCopy, string);

  char* stringToken = NULL;
  
  if((stringToken = strtok(stringCopy, delim)) == NULL) return false;

  for(int index = 0; index < amount; index += 1)
  {
    strcpy(stringArray[index], stringToken);

    if(index == (amount - 1)) stringToken = strtok(NULL, delim);

    else if((stringToken = strtok(NULL, delim)) == NULL) return false;
  }
  return true;
}
