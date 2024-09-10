/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef UCI_H
#define UCI_H

#include "../treestump.h"

extern const char* FEN_START;


extern bool fen_parse(Position* position, const char fenString[]);

extern int  uci_parse(Position* position, const char* uci_string);


extern bool string_split_at_delim(char (*stringArray)[128], const char string[], int length, const char delim[], int amount);


extern char* move_string_create(char* string, Move move);

extern Move  move_string_parse(U64 boards[12], const char* string);

#endif // UCI_H
