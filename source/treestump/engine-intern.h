/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#ifndef ENGINE_INTERN_H
#define ENGINE_INTERN_H

extern const int PIECE_SCORES[12];

extern void create_moves(MoveArray* moveArray, Position position);

extern int position_score_get(Position position);

extern void guess_order_moves(MoveArray* moveArray, Position position);

#endif // ENGINE_INTERN_H
