/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

const Castle CASTLE_WHITE_KING  = 0b0001;
const Castle CASTLE_WHITE_QUEEN = 0b0010;
const Castle CASTLE_BLACK_KING  = 0b0100;
const Castle CASTLE_BLACK_QUEEN = 0b1000;

const Castle CASTLE_WHITE = (CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
const Castle CASTLE_BLACK = (CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
