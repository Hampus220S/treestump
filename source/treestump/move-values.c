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

const Move MOVE_SHIFT_SOURCE  = 0;
const Move MOVE_SHIFT_TARGET  = 6;
const Move MOVE_SHIFT_PIECE   = 12;
const Move MOVE_SHIFT_PROMOTE = 16;

const Move MOVE_NONE = 0;
