/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H

#include "../treestump.h"

extern bool split_string_delim(char (*stringArray)[128], const char string[], int length, const char delim[], int amount);

#endif // STRING_HANDLER_H
