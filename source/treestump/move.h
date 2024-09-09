/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef MOVE_H
#define MOVE_H

#include "../treestump.h"

extern void move_make(Position* position, Move move);

extern void perft_test(Position position, int depth);

extern Move best_move(Position position, int depth, int nodes, int movetime, MoveArray searchmoves);

extern Move move_create(U64 boards[12], Square sourceSquare, Square targetSquare, Piece promotePiece);

extern bool square_is_attacked(Position position, Square square, Side side);

#endif // MOVE_H
