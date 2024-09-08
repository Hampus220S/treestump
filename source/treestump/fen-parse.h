/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef FEN_PARSE_H
#define FEN_PARSE_H

#include "../treestump.h"

extern const char* FEN_START;

extern bool fen_parse(Position* position, const char fenString[]);

#endif // FEN_PARSE_H
