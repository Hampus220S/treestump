/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "piece-intern.h"

U64 ATTACKS_BISHOP[BOARD_SQUARES][512];
U64 ATTACKS_ROOK  [BOARD_SQUARES][4096];

/*
 *
 */
U64 index_cover_create(int index, U64 attackMask, int bitAmount)
{
  U64 cover = 0ULL;

  for(int amount = 0; amount < bitAmount; amount++)
  {
    int square = board_first_square_get(attackMask);

    attackMask = BOARD_SQUARE_POP(attackMask, square);

    if(index & (1 << amount)) cover |= (1ULL << square);
  }
  return cover;
}

/*
 * Create board of attacks for a bishop
 * at the supplied square with the supplied cover (obsticles)
 */
U64 attacks_bishop_create(Square square, U64 cover)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1, file = targetFile + 1; rank < BOARD_RANKS && file < BOARD_FILES; rank++, file++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & cover) break;
  }

  for (int rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file < BOARD_FILES; rank--, file++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & cover) break;
  }

  for (int rank = targetRank + 1, file = targetFile - 1; rank < BOARD_RANKS && file >= 0; rank++, file--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & cover) break;
  }

  for (int rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & cover) break;
  }

  return attacks;
}

/*
 * Create board of attacks for a rook
 * at the supplied square with the supplied cover (obsticles)
 */
U64 attacks_rook_create(Square square, U64 cover)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1; rank < BOARD_RANKS; rank++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));

    if ((1ULL << (rank * BOARD_FILES + targetFile)) & cover) break;
  }

  for (int rank = targetRank - 1; rank >= 0; rank--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));

    if ((1ULL << (rank * BOARD_FILES + targetFile)) & cover) break;
  }

  for (int file = targetFile + 1; file < BOARD_FILES; file++)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));

    if ((1ULL << (targetRank * BOARD_FILES + file)) & cover) break;
  }

  for (int file = targetFile - 1; file >= 0; file--)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));

    if ((1ULL << (targetRank * BOARD_FILES + file)) & cover) break;
  }

  return attacks;
}

/*
 *
 */
static int magic_index_create(U64 cover, U64 magic_number, int relevant_bits)
{
  return ((cover * magic_number) >> (BOARD_SQUARES - relevant_bits));
}

/*
 * Initialize lookup attacks for a rook
 * at every square and with every case of cover
 */
static void attacks_rook_init(void)
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    int relevantBits = RELEVANT_BITS_ROOK[square];

    int coverIndicies = (1 << relevantBits);

    for (int index = 0; index < coverIndicies; index++)
    {
      U64 cover = index_cover_create(index, MASKS_ROOK[square], relevantBits);

      int magicIndex = magic_index_create(cover, MAGIC_NUMBERS_ROOK[square], relevantBits);

      ATTACKS_ROOK[square][magicIndex] = attacks_rook_create(square, cover);
    }
  }
}

/*
 * Initialize lookup attacks for a bishop
 * at every square and with every case of cover
 */
static void attacks_bishop_init(void)
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    int relevantBits = RELEVANT_BITS_BISHOP[square];

    int coverIndicies = (1 << relevantBits);

    for (int index = 0; index < coverIndicies; index++)
    {
      U64 cover = index_cover_create(index, MASKS_BISHOP[square], relevantBits);

      int magicIndex = magic_index_create(cover, MAGIC_NUMBERS_BISHOP[square], relevantBits);

      ATTACKS_BISHOP[square][magicIndex] = attacks_bishop_create(square, cover);
    }
  }
}

/*
 *
 */
void attacks_init(void)
{
  info_print("Initializing attacks");

  attacks_rook_init();

  attacks_bishop_init();
}

/*
 *
 */
static int cover_index_bishop_get(Square square, U64 cover)
{
  U64 coverIndex = cover;

  coverIndex &= MASKS_BISHOP[square];

  coverIndex *= MAGIC_NUMBERS_BISHOP[square];

  coverIndex >>= BOARD_SQUARES - RELEVANT_BITS_BISHOP[square];

  return (int) coverIndex;
}

/*
 *
 */
static int cover_index_rook_get(Square square, U64 cover)
{
  U64 coverIndex = cover;

  coverIndex &= MASKS_ROOK[square];

  coverIndex *= MAGIC_NUMBERS_ROOK[square];

  coverIndex >>= BOARD_SQUARES - RELEVANT_BITS_ROOK[square];

  return (int) coverIndex;
}

/*
 *
 */
U64 attacks_bishop_get(Square square, Position position)
{
  int coverIndex = cover_index_bishop_get(square, position.covers[SIDE_BOTH]);

  return ATTACKS_BISHOP[square][coverIndex];
}

/*
 *
 */
U64 attacks_rook_get(Square square, Position position)
{
  int coverIndex = cover_index_rook_get(square, position.covers[SIDE_BOTH]);

  return ATTACKS_ROOK[square][coverIndex];
}

/*
 *
 */
U64 attacks_queen_get(Square square, Position position)
{
  U64 queenAttacks = 0ULL;

  queenAttacks |= attacks_bishop_get(square, position);

  queenAttacks |= attacks_rook_get(square, position);

  return queenAttacks;
}

/*
 *
 */
U64 attacks_pawn_get(Square square, Side side)
{
  return MASKS_PAWN[side][square];
}

/*
 *
 */
U64 attacks_knight_get(Square square)
{
  return MASKS_KNIGHT[square];
}

/*
 *
 */
U64 attacks_king_get(Square square)
{
  return MASKS_KING[square];
}

/*
 * Lookup a board of attacks for a piece in position
 *
 * RETURN (U64 board)
 */
U64 attacks_get(Square square, Position position)
{
  switch(square_piece_get(position.boards, square))
  {
    case PIECE_WHITE_KING: case PIECE_BLACK_KING:
      return attacks_king_get(square);

    case PIECE_WHITE_KNIGHT: case PIECE_BLACK_KNIGHT:
      return attacks_knight_get(square);

    case PIECE_WHITE_BISHOP: case PIECE_BLACK_BISHOP:
      return attacks_bishop_get(square, position);

    case PIECE_WHITE_ROOK: case PIECE_BLACK_ROOK:
      return attacks_rook_get(square, position);

    case PIECE_WHITE_QUEEN: case PIECE_BLACK_QUEEN:
      return attacks_queen_get(square, position);

    case PIECE_WHITE_PAWN:
      return attacks_pawn_get(square, SIDE_WHITE);

    case PIECE_BLACK_PAWN:
      return attacks_pawn_get(square, SIDE_BLACK);

    default:
      break;
  }

  return 0ULL;
}
