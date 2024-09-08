/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef MOVE_H
#define MOVE_H

#include "../treestump.h"

extern void make_move(Position* position, Move move);

extern void perft_test(Position position, int depth);

extern Move best_move(Position position, int depth, int nodes, int movetime, MoveArray searchmoves);

extern char* move_string(char* moveString, Move move);

extern Move string_move(const char moveString[]);

extern Move complete_move(U64 boards[12], Move move);

#endif // MOVE_H
