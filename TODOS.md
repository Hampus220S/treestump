# Todos
* Rename functions, variables and files
* Create test cases for different tricky positions and their perft results

* Rename functions from convention (_create, _free, object_verb)
* Implement status codes (Unix convention) instead of binary return value
* Implement multithreading in search function
* Impelment SIGUSR2 between threads?
* Implement one thread for handling stdin and one thread for handling stdout (thinking)
* Implement SIGUSR1 between threads
* Implement SIGINT signal handler
* Create headers to prevent many extern statements at the top
* use switch case instead of if statements
* rename amount to move_count ex

* string_square => string_square_parse
* string_move + create_move => string_move_parse_and_complete 
* Make PIECE_NONE have value 0, then it will default to PIECE_NONE instead of PIECE_WHITE_PAWN

* refactore code in move_legal (make it more effective without repeating code validation)
* rename move_string to string_move_create
  - then create string.h with string_move_create/parse and string_square_create/parse
