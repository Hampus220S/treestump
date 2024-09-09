/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

U64 MASKS_BISHOP [BOARD_SQUARES];
U64 MASKS_ROOK   [BOARD_SQUARES];
U64 MASKS_PAWN[2][BOARD_SQUARES];
U64 MASKS_KNIGHT [BOARD_SQUARES];
U64 MASKS_KING   [BOARD_SQUARES];

int RELEVANT_BITS_BISHOP[BOARD_SQUARES];
int RELEVANT_BITS_ROOK  [BOARD_SQUARES];

const U64 NOT_A_FILE  = 18374403900871474942ULL;
const U64 NOT_H_FILE  = 9187201950435737471ULL;
const U64 NOT_HG_FILE = 4557430888798830399ULL;
const U64 NOT_AB_FILE = 18229723555195321596ULL;

/*
 *
 */
static U64 mask_pawn_create(Square square, Side side)
{
  U64 attacks = 0ULL;

  U64 bitboard = BOARD_SQUARE_SET(0ULL, square);

  if(side == SIDE_WHITE)
  {
    if((bitboard >> 7) & NOT_A_FILE) attacks |= (bitboard >> 7);
    if((bitboard >> 9) & NOT_H_FILE) attacks |= (bitboard >> 9);
  }
  else
  {
    if((bitboard << 7) & NOT_H_FILE) attacks |= (bitboard << 7);
    if((bitboard << 9) & NOT_A_FILE) attacks |= (bitboard << 9);    
  }
  return attacks;
}

/*
 *
 */
static U64 mask_knight_create(Square square)
{
  U64 attacks = 0ULL;

  U64 bitboard = BOARD_SQUARE_SET(0ULL, square);

  if((bitboard >> 17) & NOT_H_FILE)  attacks |= (bitboard >> 17);
  if((bitboard >> 15) & NOT_A_FILE)  attacks |= (bitboard >> 15);
  if((bitboard >> 10) & NOT_HG_FILE) attacks |= (bitboard >> 10);
  if((bitboard >> 6)  & NOT_AB_FILE) attacks |= (bitboard >> 6);

  if((bitboard << 17) & NOT_A_FILE)  attacks |= (bitboard << 17);
  if((bitboard << 15) & NOT_H_FILE)  attacks |= (bitboard << 15);
  if((bitboard << 10) & NOT_AB_FILE) attacks |= (bitboard << 10);
  if((bitboard << 6)  & NOT_HG_FILE) attacks |= (bitboard << 6);

  return attacks;
}

/*
 *
 */
static U64 mask_king_create(Square square)
{
  U64 attacks = 0ULL;

  U64 bitboard = BOARD_SQUARE_SET(0ULL, square);

  if (bitboard >> 8)                attacks |= (bitboard >> 8);
  if ((bitboard >> 9) & NOT_H_FILE) attacks |= (bitboard >> 9);
  if ((bitboard >> 7) & NOT_A_FILE) attacks |= (bitboard >> 7);
  if ((bitboard >> 1) & NOT_H_FILE) attacks |= (bitboard >> 1);

  if (bitboard << 8)                attacks |= (bitboard << 8);
  if ((bitboard << 9) & NOT_A_FILE) attacks |= (bitboard << 9);
  if ((bitboard << 7) & NOT_H_FILE) attacks |= (bitboard << 7);
  if ((bitboard << 1) & NOT_A_FILE) attacks |= (bitboard << 1);

  return attacks;
}

/*
 *
 */
static U64 mask_bishop_create(Square square)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));
  }
  for (int rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) 
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));
  }
  for (int rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; rank++, file--) 
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));
  }
  for (int rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) 
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));
  }
  return attacks;
}

/*
 *
 */
static U64 mask_rook_create(Square square)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1; rank <= 6; rank++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));
  }
  for (int rank = targetRank - 1; rank >= 1; rank--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));
  }
  for (int file = targetFile + 1; file <= 6; file++)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));
  }
  for (int file = targetFile - 1; file >= 1; file--)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));
  }
  return attacks;
}


/*
 *
 */
void masks_init(void)
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    MASKS_PAWN[SIDE_WHITE][square] = mask_pawn_create  (square, SIDE_WHITE);

    MASKS_PAWN[SIDE_BLACK][square] = mask_pawn_create  (square, SIDE_BLACK);

    MASKS_KNIGHT          [square] = mask_knight_create(square);

    MASKS_KING            [square] = mask_king_create  (square);

    MASKS_ROOK            [square] = mask_rook_create  (square);

    MASKS_BISHOP          [square] = mask_bishop_create(square);
  }
}

/*
 *
 */
void relevant_bits_init(void)
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    RELEVANT_BITS_BISHOP[square] = board_bit_amount(MASKS_BISHOP[square]);

    RELEVANT_BITS_ROOK  [square] = board_bit_amount(MASKS_ROOK  [square]);
  }
}
