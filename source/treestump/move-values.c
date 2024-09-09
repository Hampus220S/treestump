/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

const Move MOVE_MASK_SOURCE  = 0x00003f;
const Move MOVE_MASK_TARGET  = 0x000fc0;
const Move MOVE_MASK_PIECE   = 0x00f000;
const Move MOVE_MASK_PROMOTE = 0x0f0000;
const Move MOVE_MASK_CAPTURE = 0x100000;
const Move MOVE_MASK_DOUBLE  = 0x200000;
const Move MOVE_MASK_PASSANT = 0x400000;
const Move MOVE_MASK_CASTLE  = 0x800000;

const Move MOVE_NONE = 0;

// This lets you convert a corner square to a castle right
const Castle SQUARE_CASTLES[] = {
  [SQUARE_NONE] = 0,
  [A1] = CASTLE_WHITE_QUEEN,
  [H1] = CASTLE_WHITE_KING,
  [A8] = CASTLE_BLACK_QUEEN,
  [H8] = CASTLE_BLACK_KING
};

