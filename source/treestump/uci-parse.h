/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef UCI_PARSE_H
#define UCI_PARSE_H

#include "../treestump.h"

extern const char* FEN_START;

extern bool fen_parse(Position* position, const char fenString[]);

extern void uci_parse(Position* position, const char uciString[]);

#endif // UCI_PARSE_H
