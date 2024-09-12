# Todos
* rename amount to move_count ex
* extract square score from files intead of hard coded values
* Rename functions from convention (_create, _free, object_verb)
* Add '=' symbol to BOARD_SQUARE_SET/GET/POP (skip the assignment part) (if it works everywhere)
  - I now think the best way is to create new macro with this feature
* Loosen up the strict naming and allow names like source_square, not square_source

Future
* create real move strings, not just source square target square promote
* Create test cases for different tricky positions and their perft results

* Implement status codes (Unix convention) instead of binary return value
* Implement multithreading in search function
* Impelment SIGUSR2 between threads?
* Implement one thread for handling stdin and one thread for handling stdout (thinking)
* Implement SIGUSR1 between threads
* Implement SIGINT signal handler

Goal:
* Prioritize if-statements in move_legal to eliminate move as fast as possible
* check as little as possible in move_pseudo_legal
* maybe?: create custom move_pseudo_legal and move_legal that only checks some things: 

Priorities:
(things that moves_create do not prevent)
* Check empty squares when pawn move
* Check empty squares between castle
* Add more in case perft results fail :D
