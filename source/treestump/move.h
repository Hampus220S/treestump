/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef MOVE_H
#define MOVE_H

#include "../treestump.h"

typedef int Move;

extern const Move MOVE_MASK_SOURCE;
extern const Move MOVE_MASK_TARGET;
extern const Move MOVE_MASK_PIECE;
extern const Move MOVE_MASK_PROMOTE;
extern const Move MOVE_MASK_CAPTURE;
extern const Move MOVE_MASK_DOUBLE;
extern const Move MOVE_MASK_PASSANT;
extern const Move MOVE_MASK_CASTLE;

extern const Move MOVE_NONE;

#define MOVE_SHIFT_SOURCE  0
#define MOVE_SHIFT_TARGET  6
#define MOVE_SHIFT_PIECE   12
#define MOVE_SHIFT_PROMOTE 16

#define MOVE_SOURCE_GET(MOVE)  (((MOVE) & MOVE_MASK_SOURCE)  >> MOVE_SHIFT_SOURCE)
#define MOVE_TARGET_GET(MOVE)  (((MOVE) & MOVE_MASK_TARGET)  >> MOVE_SHIFT_TARGET)
#define MOVE_PIECE_GET(MOVE)   (((MOVE) & MOVE_MASK_PIECE)   >> MOVE_SHIFT_PIECE)
#define MOVE_PROMOTE_GET(MOVE) (((MOVE) & MOVE_MASK_PROMOTE) >> MOVE_SHIFT_PROMOTE)

#define MOVE_SOURCE_SET(SOURCE)   (((SOURCE)  << MOVE_SHIFT_SOURCE)  & MOVE_MASK_SOURCE)
#define MOVE_TARGET_SET(TARGET)   (((TARGET)  << MOVE_SHIFT_TARGET)  & MOVE_MASK_TARGET)
#define MOVE_PIECE_SET(PIECE)     (((PIECE)   << MOVE_SHIFT_PIECE)   & MOVE_MASK_PIECE)
#define MOVE_PROMOTE_SET(PROMOTE) (((PROMOTE) << MOVE_SHIFT_PROMOTE) & MOVE_MASK_PROMOTE)

extern void move_make(Position* position, Move move);


extern Move move_double_create(Square sourceSquare, Square targetSquare, Piece piece);

extern Move move_promote_create(U64 boards[12], Square pawn_square, Square promote_square, Piece pawn_piece, Piece promote_piece);

extern Move move_castle_create(Square sourceSquare, Square targetSquare, Piece piece);

extern Move move_normal_create(U64 boards[12], Square sourceSquare, Square targetSquare, Piece piece);

extern Move move_create(U64 boards[12], Square sourceSquare, Square targetSquare, Piece promotePiece);


extern bool move_is_legal(Position position, Move move);

#endif // MOVE_H
