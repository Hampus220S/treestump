/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

U64 BOARD_LINES[BOARD_SQUARES][BOARD_SQUARES];

/*
 * This function determines if the line is either diagonal or straight,
 * based on rank and file offset
 *
 * RETURN (bool result)
 * - true  | The line is either  diagonal or  straight
 * - false | The line is neither diagonal nor straight
 */
static bool line_is_diagonal_or_straight(int rank_offset, int file_offset)
{
  int abs_rank_offset = abs(rank_offset);
  int abs_file_offset = abs(file_offset);

  bool diagonal = (abs_rank_offset == abs_file_offset);
  bool straight = ((abs_rank_offset == 0) ^ (abs_file_offset == 0));

  return (diagonal || straight);
}

/*
 * Create a U64 board with a line between source and target
 *
 * RETURN (U64 board)
 */
static U64 board_line_create(Square source, Square target)
{
  U64 board = 0ULL;

  int source_rank = (source / BOARD_FILES);
  int source_file = (source % BOARD_FILES);

  int target_rank = (target / BOARD_FILES);
  int target_file = (target % BOARD_FILES);

  int rank_offset = (target_rank - source_rank);
  int file_offset = (target_file - source_file);

  if(!line_is_diagonal_or_straight(rank_offset, file_offset)) return 0ULL;

  // This one-liner determies the sign of a number! :D
  int rank_scalor = (rank_offset > 0) - (rank_offset < 0);
  int file_scalor = (file_offset > 0) - (file_offset < 0);

  for(int rank = source_rank + rank_scalor,
          file = source_file + file_scalor;
      (rank != target_rank || file != target_file);
      rank += rank_scalor, file += file_scalor)
  {
    Square square = (rank * BOARD_FILES) + file;

    board = BOARD_SQUARE_SET(board, square);
  }

  return board;
}

/*
 * Initialize lookup line boards
 * from every square on the board to every other square on the board
 */
void board_lines_init(void)
{
  if(args.debug) info_print("Initializing board lines");

  for(Square sourceSquare = A8; sourceSquare <= H1; sourceSquare++)
  {
    for(Square targetSquare = A8; targetSquare <= H1; targetSquare++)
    {
      U64 board_line = board_line_create(sourceSquare, targetSquare);

      BOARD_LINES[sourceSquare][targetSquare] = board_line;
    }
  }
}

/*
 *
 */
int board_bit_amount_get(U64 bitboard)
{
  int amount;

  for(amount = 0; bitboard; amount++)
  {
    bitboard &= bitboard - 1;
  }

  return amount;
}

/*
 *
 */
Square board_first_square_get(U64 bitboard)
{
  if(!bitboard) return SQUARE_NONE;

  return board_bit_amount_get((bitboard & -bitboard) - 1);
}

/*
 * Get the piece that is on the specified square
 */
Piece square_piece_get(U64 boards[12], Square square)
{
  for(Piece piece = PIECE_WHITE_PAWN; piece <= PIECE_BLACK_KING; piece++)
  {
    if(BOARD_SQUARE_GET(boards[piece], square)) return piece;
  }

  return PIECE_NONE;
}

