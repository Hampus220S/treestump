/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef UCI_H
#define UCI_H

#include "../treestump.h"

extern const char* FEN_START;


extern int fen_parse(Position* position, const char* fen_string);

extern int uci_parse(Position* position, const char* uci_string);


extern char* move_string_create(char* string, Move move);

extern Move  move_string_parse(Position position, const char* string);

#endif // UCI_H
