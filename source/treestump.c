/*
 * treestump
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "treestump.h"

/*
 *
 */
void init_all(void)
{
  init_piece_lookup_masks();

  init_bishop_rook_relevant_bits();

  init_piece_lookup_attacks();

  init_board_lookup_lines();

  init_random_keys();
}

/*
 *
 */
bool stdin_string(char* string)
{
  fflush(stdin);

  char buffer[1024];

  if(fgets(buffer, sizeof(buffer), stdin) == NULL) return false;

  return (sscanf(buffer, "%[^\n]%*c", string) == 1);
}

/*
 *
 */
int main(int argc, char* argv[])
{
  info_print("Start of main");

  init_all();

  Position position;
  fen_parse(&position, FEN_START);

  char uciString[256];

  do
  {
    memset(uciString, 0, sizeof(uciString));

    stdin_string(uciString);

    uci_parse(&position, uciString);
  }
  while(strcmp(uciString, "quit") != 0);

  info_print("End of main");

  return 0;
}
