/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

#include "piece-intern.h"

U64 BISHOP_LOOKUP_ATTACKS[BOARD_SQUARES][512];
U64 ROOK_LOOKUP_ATTACKS[BOARD_SQUARES][4096];

/*
 *
 */
U64 create_index_cover(int index, U64 attackMask, int bitAmount)
{
  U64 cover = 0ULL;

  for(int amount = 0; amount < bitAmount; amount++)
  {
    int square = board_ls1b_index(attackMask);

    attackMask = BOARD_SQUARE_POP(attackMask, square);

    if(index & (1 << amount)) cover |= (1ULL << square);
  }
  return cover;
}

/*
 *
 */
U64 calculate_bishop_attacks(Square square, U64 block)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1, file = targetFile + 1; rank < BOARD_RANKS && file < BOARD_FILES; rank++, file++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & block) break;
  }

  for (int rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file < BOARD_FILES; rank--, file++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & block) break;
  }

  for (int rank = targetRank + 1, file = targetFile - 1; rank < BOARD_RANKS && file >= 0; rank++, file--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & block) break;
  }

  for (int rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + file));

    if ((1ULL << (rank * BOARD_FILES + file)) & block) break;
  }

  return attacks;
}

/*
 *
 */
U64 calculate_rook_attacks(Square square, U64 block)
{
  U64 attacks = 0ULL;

  int targetRank = square / BOARD_FILES;
  int targetFile = square % BOARD_FILES;

  for (int rank = targetRank + 1; rank < BOARD_RANKS; rank++)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));

    if ((1ULL << (rank * BOARD_FILES + targetFile)) & block) break;
  }

  for (int rank = targetRank - 1; rank >= 0; rank--)
  {
    attacks |= (1ULL << (rank * BOARD_FILES + targetFile));

    if ((1ULL << (rank * BOARD_FILES + targetFile)) & block) break;
  }

  for (int file = targetFile + 1; file < BOARD_FILES; file++)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));

    if ((1ULL << (targetRank * BOARD_FILES + file)) & block) break;
  }

  for (int file = targetFile - 1; file >= 0; file--)
  {
    attacks |= (1ULL << (targetRank * BOARD_FILES + file));

    if ((1ULL << (targetRank * BOARD_FILES + file)) & block) break;
  }

  return attacks;
}

/*
 *
 */
void init_rook_lookup_attacks()
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    int relevantBits = ROOK_RELEVANT_BITS[square];

    int coverIndicies = (1 << relevantBits);

    for (int index = 0; index < coverIndicies; index++)
    {
      U64 cover = create_index_cover(index, ROOK_LOOKUP_MASKS[square], relevantBits);

      int magicIndex = (cover * ROOK_MAGIC_NUMBERS[square]) >> (BOARD_SQUARES - relevantBits);

      ROOK_LOOKUP_ATTACKS[square][magicIndex] = calculate_rook_attacks(square, cover);
    }
  }
}

/*
 *
 */
void init_bishop_lookup_attacks()
{
  for (Square square = 0; square < BOARD_SQUARES; square++)
  {
    int relevantBits = BISHOP_RELEVANT_BITS[square];

    int coverIndicies = (1 << relevantBits);

    for (int index = 0; index < coverIndicies; index++)
    {
      U64 cover = create_index_cover(index, BISHOP_LOOKUP_MASKS[square], relevantBits);

      int magicIndex = (cover * BISHOP_MAGIC_NUMBERS[square]) >> (BOARD_SQUARES - relevantBits);

      BISHOP_LOOKUP_ATTACKS[square][magicIndex] = calculate_bishop_attacks(square, cover);
    }
  }
}

/*
 *
 */
void init_piece_lookup_attacks()
{
  init_rook_lookup_attacks();

  init_bishop_lookup_attacks();
}

/*
 *
 */
int bishop_cover_index(Square square, U64 cover)
{
  U64 coverIndex = cover;

  coverIndex &= BISHOP_LOOKUP_MASKS[square];

  coverIndex *= BISHOP_MAGIC_NUMBERS[square];

  coverIndex >>= BOARD_SQUARES - BISHOP_RELEVANT_BITS[square];

  return (int) coverIndex;
}

/*
 *
 */
int rook_cover_index(Square square, U64 cover)
{
  U64 coverIndex = cover;

  coverIndex &= ROOK_LOOKUP_MASKS[square];

  coverIndex *= ROOK_MAGIC_NUMBERS[square];

  coverIndex >>= BOARD_SQUARES - ROOK_RELEVANT_BITS[square];

  return (int) coverIndex;
}

/*
 *
 */
U64 bishop_lookup_attacks(Square square, Position position)
{
  int coverIndex = bishop_cover_index(square, position.covers[SIDE_BOTH]);

  return BISHOP_LOOKUP_ATTACKS[square][coverIndex];
}

/*
 *
 */
U64 rook_lookup_attacks(Square square, Position position)
{
  int coverIndex = rook_cover_index(square, position.covers[SIDE_BOTH]);

  return ROOK_LOOKUP_ATTACKS[square][coverIndex];
}

/*
 *
 */
U64 queen_lookup_attacks(Square square, Position position)
{
  U64 queenAttacks = 0ULL;

  queenAttacks |= bishop_lookup_attacks(square, position);

  queenAttacks |= rook_lookup_attacks(square, position);

  return queenAttacks;
}

/*
 *
 */
U64 pawn_lookup_attacks(Square square, Side side)
{
  return PAWN_LOOKUP_MASKS[side][square];
}

/*
 *
 */
U64 knight_lookup_attacks(Square square)
{
  return KNIGHT_LOOKUP_MASKS[square];
}

/*
 *
 */
U64 king_lookup_attacks(Square square)
{
  return KING_LOOKUP_MASKS[square];
}

/*
 * Lookup a board of attacks for a piece in position
 *
 * RETURN (U64 board)
 */
U64 piece_lookup_attacks(Square square, Position position)
{
  switch(boards_square_piece(position.boards, square))
  {
    case PIECE_WHITE_KING: case PIECE_BLACK_KING:
      return king_lookup_attacks(square);

    case PIECE_WHITE_KNIGHT: case PIECE_BLACK_KNIGHT:
      return knight_lookup_attacks(square);

    case PIECE_WHITE_BISHOP: case PIECE_BLACK_BISHOP:
      return bishop_lookup_attacks(square, position);

    case PIECE_WHITE_ROOK: case PIECE_BLACK_ROOK:
      return rook_lookup_attacks(square, position);

    case PIECE_WHITE_QUEEN: case PIECE_BLACK_QUEEN:
      return queen_lookup_attacks(square, position);

    case PIECE_WHITE_PAWN:
      return pawn_lookup_attacks(square, SIDE_WHITE);

    case PIECE_BLACK_PAWN:
      return pawn_lookup_attacks(square, SIDE_BLACK);

    default:
      break;
  }

  return 0ULL;
}
