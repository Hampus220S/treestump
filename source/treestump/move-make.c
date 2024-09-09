/*
 * Make move in position
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "../treestump.h"

extern const Castle SQUARE_CASTLES[];

#define MOVE_BOARD_CREATE(SOURCE, TARGET) ((1ULL << (SOURCE)) ^ (1ULL << (TARGET)))

/*
 *
 */
static void move_castle_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;


  Piece rookPiece = (sourcePiece == PIECE_WHITE_KING) ? PIECE_WHITE_ROOK : PIECE_BLACK_ROOK;

  Square rookSourceSquare = (targetSquare > sourceSquare) ? (sourceSquare + 3) : (sourceSquare - 4);

  Square rookTargetSquare = (targetSquare > sourceSquare) ? (targetSquare - 1) : (targetSquare + 1);



  U64 kingMoveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  U64 rookMoveBoard = MOVE_BOARD_CREATE(rookSourceSquare, rookTargetSquare);

  
  position->boards[sourcePiece] ^= kingMoveBoard;

  position->boards[rookPiece] ^= rookMoveBoard;


  position->covers[side] ^= (kingMoveBoard ^ rookMoveBoard) ;

  position->covers[SIDE_BOTH] ^= (kingMoveBoard ^ rookMoveBoard);



  position->castle &= (sourcePiece == PIECE_WHITE_KING) ? ~CASTLE_WHITE : ~CASTLE_BLACK;
}

/*
 *
 */
static void move_normal_quiet_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  position->boards[sourcePiece] ^= moveBoard;

  position->covers[side] ^= moveBoard;


  position->covers[SIDE_BOTH] ^= moveBoard;


  position->clock++;
}

/*
 * When an illegal move of capturing own piece, the function dont work 
 */
static void move_normal_capture_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = PIECE_SIDE_GET(sourcePiece);


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  Piece capturePiece = square_piece_get(position->boards, targetSquare);


  position->boards[sourcePiece] ^= moveBoard;

  position->covers[side] ^= moveBoard;


  if(capturePiece != PIECE_NONE)
  {
    Side captureSide = (side == SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;

    position->boards[capturePiece] &= ~(1ULL << targetSquare);

    position->covers[captureSide] &= ~(1ULL << targetSquare);
  }


  position->covers[SIDE_BOTH] &= ~(1ULL << sourceSquare);

  position->clock = 0;
}

/*
 * Clear castling rights based on what source piece was moved
 *
 * Maybe: Change arguments to just Move move
 *
 * RETURN (Castle castle)
 */
static Castle source_square_castle_clear(Castle castle, Square source_square, Piece source_piece)
{
  switch(source_piece)
  {
    case PIECE_WHITE_ROOK: case PIECE_BLACK_ROOK:
      castle &= ~SQUARE_CASTLES[source_square];
      break;

    case PIECE_WHITE_KING:
      castle &= ~CASTLE_WHITE;
      break;

    case PIECE_BLACK_KING:
      castle &= ~CASTLE_BLACK;
      break;

    default:
      break;
  }

  return castle;
}

/*
 * Clear castling rights based on what target piece was captured
 *
 * If the target square is a corner square - where a rook stands in the beginning
 * - clear that rook's castling right
 *
 * Maybe: Change arguments to just Move move
 *
 * RETURN (Castle castle)
 */
static Castle target_square_castle_clear(Castle castle, Square target_square)
{
  return castle & ~SQUARE_CASTLES[target_square];
}

/*
 *
 */
static void move_normal_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_normal_capture_make(position, move);
  }
  else move_normal_quiet_make(position, move);

  // The following will execute for both normal moves
  Square source_square = MOVE_SOURCE_GET(move);
  Piece  source_piece  = MOVE_PIECE_GET(move);

  position->castle = source_square_castle_clear(position->castle, source_square, source_piece);
}

/*
 *
 */
static void move_pawn_passant_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;



  Square passantSquare = (sourcePiece == PIECE_WHITE_PAWN) ? (targetSquare + BOARD_FILES) : (targetSquare - BOARD_FILES);

  Piece passantPawn = (sourcePiece == PIECE_WHITE_PAWN) ? PIECE_BLACK_PAWN : PIECE_WHITE_PAWN;

  Side passantSide = (side == SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);



  position->boards[passantPawn] &= ~(1ULL << passantSquare);

  position->boards[sourcePiece] ^= moveBoard;


  position->covers[side] ^= moveBoard;

  position->covers[passantSide] &= ~(1ULL << passantSquare);


  position->covers[SIDE_BOTH] ^= moveBoard;
  position->covers[SIDE_BOTH] &= ~(1ULL << passantSquare);
}

/*
 *
 */
static void move_pawn_double_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);
  
  // Move the pawn on the board
  position->boards[sourcePiece] ^= moveBoard;

  // Update the side cover/cover boards
  position->covers[side] ^= moveBoard;

  // Update the cover/cover boards for both sides
  position->covers[SIDE_BOTH] ^= moveBoard;


  position->passant = (sourcePiece == PIECE_WHITE_PAWN) ? (sourceSquare - BOARD_FILES) : (sourceSquare + BOARD_FILES);
}

/*
 *
 */
static void move_pawn_promote_quiet_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Piece promotePiece = MOVE_PROMOTE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);


  position->boards[promotePiece] |= (1ULL << targetSquare);

  position->boards[sourcePiece] &= ~(1ULL << sourceSquare);


  position->covers[side] ^= moveBoard;

  position->covers[SIDE_BOTH] ^= moveBoard;
}

/*
 *
 */
static void move_pawn_promote_capture_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Piece promotePiece = MOVE_PROMOTE_GET(move);

  Side side = PIECE_SIDE_GET(sourcePiece);


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  Piece capturePiece = square_piece_get(position->boards, targetSquare);


  position->boards[promotePiece] |= (1ULL << targetSquare);

  position->boards[sourcePiece] &= ~(1ULL << sourceSquare);


  position->covers[side] ^= moveBoard;


  if(capturePiece != PIECE_NONE)
  {
    Side captureSide = (side == SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;

    position->boards[capturePiece] &= ~(1ULL << targetSquare);

    position->covers[captureSide] &= ~(1ULL << targetSquare);
  }

  position->covers[SIDE_BOTH] &= ~(1ULL << sourceSquare);
} 

/*
 *
 */
static void move_pawn_promote_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_pawn_promote_capture_make(position, move);
  }
  else move_pawn_promote_quiet_make(position, move);

  // This is made for both cases of promote moves
  // position->passant = PIECE_NONE;
}

/*
 * This has the same code as move_normal_quiet_make*
 */
static void move_pawn_normal_quiet_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = (sourcePiece >= PIECE_WHITE_PAWN && sourcePiece <= PIECE_WHITE_KING) ? SIDE_WHITE : SIDE_BLACK;


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  position->boards[sourcePiece] ^= moveBoard;

  position->covers[side] ^= moveBoard;


  position->covers[SIDE_BOTH] ^= moveBoard;
}

/*
 * This has the same code as move_normal_capture_make*
 */
static void move_pawn_normal_capture_make(Position* position, Move move)
{
  Square sourceSquare = MOVE_SOURCE_GET(move);
  Square targetSquare = MOVE_TARGET_GET(move);

  Piece sourcePiece = MOVE_PIECE_GET(move);

  Side side = PIECE_SIDE_GET(sourcePiece);


  U64 moveBoard = MOVE_BOARD_CREATE(sourceSquare, targetSquare);

  Piece capturePiece = square_piece_get(position->boards, targetSquare);


  position->boards[sourcePiece] ^= moveBoard;

  position->covers[side] ^= moveBoard;


  if(capturePiece != PIECE_NONE)
  {
    Side captureSide = (side == SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;

    position->boards[capturePiece] &= ~(1ULL << targetSquare);

    position->covers[captureSide] &= ~(1ULL << targetSquare);
  }


  position->covers[SIDE_BOTH] &= ~(1ULL << sourceSquare);
}

/*
 *
 */
static void move_pawn_normal_make(Position* position, Move move)
{
  if(move & MOVE_MASK_CAPTURE)
  {
    move_pawn_normal_capture_make(position, move);
  }
  else move_pawn_normal_quiet_make(position, move);

  // This is made for both pawn normal moves
  // position->passant = PIECE_NONE;
}

/*
 *
 */
static void move_pawn_make(Position* position, Move move)
{
  if(move & MOVE_MASK_DOUBLE)
  {
    move_pawn_double_make(position, move);
  }
  else if(move & MOVE_MASK_PROMOTE)
  {
    move_pawn_promote_make(position, move);
  }
  else if(move & MOVE_MASK_PASSANT)
  {
    move_pawn_passant_make(position, move);
  }
  else move_pawn_normal_make(position, move);

  // This is made for every pawn move
  position->clock = 0;
}

/*
 * Make move in position
 * excpected that the move is legal and valid
 *
 * This function just executes, and does not validate
 */
void move_make(Position* position, Move move)
{
  position->clock++;
  position->passant = SQUARE_NONE;

  Piece sourcePiece = MOVE_PIECE_GET(move);

  // 2. Make more specific things based on type of move
  if(sourcePiece == PIECE_WHITE_PAWN || sourcePiece == PIECE_BLACK_PAWN)
  {
    move_pawn_make(position, move);
  }
  else if(move & MOVE_MASK_CASTLE)
  {
    move_castle_make(position, move);
  }
  else
  {
    move_normal_make(position, move);
  }


  Square target_square = MOVE_TARGET_GET(move);

  position->castle = target_square_castle_clear(position->castle, target_square);


  if(position->side == SIDE_BLACK) position->turns++;

  position->side = (position->side == SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;
}
