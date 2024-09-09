/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#ifndef ENGINE_INTERN_H
#define ENGINE_INTERN_H

extern const int PIECE_SCORES[12];

extern int position_score_get(Position position);


extern void moves_create(MoveArray* moveArray, Position position);

extern void moves_guess_order(MoveArray* moveArray, Position position);

#endif // ENGINE_INTERN_H
