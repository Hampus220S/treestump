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
void bitboard_print(U64 bitboard)
{
  for(int rank = 0; rank < BOARD_RANKS; rank++)
  {
    for(int file = 0; file < BOARD_FILES; file++)
    {
      Square square = (rank * BOARD_FILES) + file;

      if(!file) printf("%d ", BOARD_RANKS - rank);

      printf("%c ", BOARD_SQUARE_GET(bitboard, square) ? 'X' : '.');
    }

    printf("\n");
  }

  printf("  A B C D E F G H\n");

  printf("\nBitboard: %llu\n\n", bitboard);
}

/*
 *
 */
void position_print(Position position)
{
  printf("\n");

  for(int rank = 0; rank < BOARD_RANKS; rank++)
  {
    for(int file = 0; file < BOARD_FILES; file++)
    {
      Square square = (rank * BOARD_FILES) + file;

      if(!file) printf("%d ", BOARD_RANKS - rank);

      int piece = square_piece_get(position.boards, square);

      printf("%c ", (piece != PIECE_NONE) ? PIECE_SYMBOLS[piece] : '.');
    }

    printf("\n");
  }

  printf("  A B C D E F G H\n\n");

  printf("Side:      %s\n", (position.side == SIDE_WHITE) ? "white" : "black");

  printf("Enpassant: %s\n", (position.passant != SQUARE_NONE) ? SQUARE_STRINGS[position.passant] : "no");

  printf("Castling:  %c%c%c%c\n",
    (position.castle & CASTLE_WHITE_KING)  ? 'K' : '-',
    (position.castle & CASTLE_WHITE_QUEEN) ? 'Q' : '-',
    (position.castle & CASTLE_BLACK_KING)  ? 'k' : '-',
    (position.castle & CASTLE_BLACK_QUEEN) ? 'q' : '-'
  );

  printf("\n");
}
