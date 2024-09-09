#include "../treestump.h"

int board_bit_amount(U64 bitboard)
{
  int count = 0;

  while(bitboard)
  {
    count++;
    bitboard &= bitboard - 1;
  }

  return count;
}

int board_ls1b_index(U64 bitboard)
{
  if(!bitboard) return -1;

  return board_bit_amount((bitboard & -bitboard) - 1);
}

/*
 * Get the piece that is on the specified square
 */
Piece boards_square_piece(U64 boards[12], Square square)
{
  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    if(BOARD_SQUARE_GET(boards[piece], square)) return piece;
  }

  return PIECE_NONE;
}

