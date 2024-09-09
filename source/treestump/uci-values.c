/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-09
 */

#include "../treestump.h"

const char* SQUARE_STRINGS[BOARD_SQUARES] = 
{
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", 
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", 
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" 
};

const char PIECE_SYMBOLS[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

const Piece SYMBOL_PIECES[] =
{
  ['P'] = PIECE_WHITE_PAWN,
  ['N'] = PIECE_WHITE_KNIGHT,
  ['B'] = PIECE_WHITE_BISHOP,
  ['R'] = PIECE_WHITE_ROOK,
  ['Q'] = PIECE_WHITE_QUEEN,
  ['K'] = PIECE_WHITE_KING,
  ['p'] = PIECE_BLACK_PAWN,
  ['n'] = PIECE_BLACK_KNIGHT,
  ['b'] = PIECE_BLACK_BISHOP,
  ['r'] = PIECE_BLACK_ROOK,
  ['q'] = PIECE_BLACK_QUEEN,
  ['k'] = PIECE_BLACK_KING
};

const char* FEN_START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

