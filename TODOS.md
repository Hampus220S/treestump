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
* use switch case instead of if statements
* rename amount to move_count ex
* extract square score from files intead of hard coded values
- trust that position.side always is either SIDE_WHITE or SIDE_BLACK and use !side to get opposite
- add () around variables in macros

* refactore code in move_legal (make it more effective without repeating code validation)

Future
- create real move strings, not just source square target square promote
