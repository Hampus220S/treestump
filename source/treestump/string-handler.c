#include "../treestump.h"

extern const Piece SYMBOL_PIECES[];

extern const char* SQUARE_STRINGS[BOARD_SQUARES];

extern const char PIECE_SYMBOLS[12];


Square string_square(const char squareString[])
{
  int file = squareString[0] - 'a';
  int rank = BOARD_RANKS - (squareString[1] - '0');

  if(!(file >= 0 && file < BOARD_FILES) || !(rank >= 0 && rank < BOARD_RANKS)) return SQUARE_NONE;

  return (rank * BOARD_FILES) + file;
}

Move string_move(const char moveString[])
{
  Move parseMove = 0;

  Square sourceSquare = string_square(moveString += 0);
  if(sourceSquare == SQUARE_NONE) return 0;

  Square targetSquare = string_square(moveString += 2);
  if(targetSquare == SQUARE_NONE) return 0;

  parseMove |= MOVE_SOURCE_SET(sourceSquare);
  parseMove |= MOVE_TARGET_SET(targetSquare);

  Piece promotePiece = SYMBOL_PIECES[(unsigned char) *moveString];

  if(promotePiece != PIECE_WHITE_PAWN) parseMove |= MOVE_PROMOTE_SET(promotePiece);

  return parseMove;
}

char* move_string(char* moveString, Move move)
{
  const char* sourceString = SQUARE_STRINGS[MOVE_SOURCE_GET(move)];
  const char* targetString = SQUARE_STRINGS[MOVE_TARGET_GET(move)];

  Piece promotePiece = MOVE_PROMOTE_GET(move);

  if(promotePiece == PIECE_WHITE_PAWN) sprintf(moveString, "%s%s", sourceString, targetString);

  else sprintf(moveString, "%s%s%c", sourceString, targetString, PIECE_SYMBOLS[promotePiece]);

  return moveString;
}

bool split_string_delim(char (*stringArray)[128], const char string[], int length, const char delim[], int amount)
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
