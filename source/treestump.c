/*
 * treestump
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-08
 */

#include "treestump.h"

static char doc[] = "treestump";

static char args_doc[] = "";

static struct argp_option options[] =
{
  { "debug", 'd', 0, 0, "Print debug messages" },
  { 0 }
};

struct args
{
  bool debug;
};

struct args args =
{
  .debug = false
};

/*
 * This is the option parsing function used by argp
 */
static error_t opt_parse(int key, char* arg, struct argp_state* state)
{
  struct args* args = state->input;

  switch(key)
  {
    case 'd':
      args->debug = true;
      break;

    case ARGP_KEY_ARG:
      break;

    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

/*
 *
 */
static void init_all(void)
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
static bool stdin_string(char* string)
{
  fflush(stdin);

  char buffer[1024];

  if(fgets(buffer, sizeof(buffer), stdin) == NULL) return false;

  return (sscanf(buffer, "%[^\n]%*c", string) == 1);
}

/*
 * Keyboard interrupt
 */
static void sigint_handler(int signum)
{
  if(args.debug) info_print("Keyboard interrupt");
}

/*
 * Broken pipe
 */
static void sigpipe_handler(int signum)
{
  if(args.debug) error_print("Broken pipe");
}

/*
 *
 */
static void sigusr1_handler(int signum) { }

/*
 * Setup handler for specified signal
 *
 * The signal will be handled, in comparision to using the signal() function
 */
static void signal_handler_setup(int signum, void (*handler) (int))
{
  struct sigaction sig_action;

  sig_action.sa_handler = handler;
  sig_action.sa_flags = 0;
  sigemptyset(&sig_action.sa_mask);

  sigaction(signum, &sig_action, NULL);
}

/*
 * Setup handlers for different signals that can be omitted
 */
static void signals_handler_setup(void)
{
  signal_handler_setup(SIGPIPE, sigpipe_handler);

  signal_handler_setup(SIGINT,  sigint_handler);

  signal_handler_setup(SIGUSR1, sigusr1_handler);
}

static struct argp argp = { options, opt_parse, args_doc, doc };

/*
 * This is the main function
 */
int main(int argc, char* argv[])
{
  argp_parse(&argp, argc, argv, 0, 0, &args);

  signals_handler_setup();

  info_print("Start of main");

  init_all();

  Position position;
  fen_parse(&position, FEN_START);

  char uci_string[256];

  do
  {
    memset(uci_string, '\0', sizeof(uci_string));

    stdin_string(uci_string);

    uci_parse(&position, uci_string);
  }
  while(strcmp(uci_string, "quit") != 0);

  info_print("End of main");

  return 0;
}
