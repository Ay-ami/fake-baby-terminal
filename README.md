# fake-baby-terminal

Write a C or C++ program called "mysh" which replaces the command shell in Unix.  After started, it prints a prompt “#” and reads a command line terminated by newline. This line should be parsed out into a command and all its arguments. In other words, tokenize it.
Does not support "special" characters like quotation marks, backslashes, and tab characters. And does not support file paths with spaces in them.
•	Maximum 10 command line arguments by defalut though it is a variable defined up top so it could be changed 


This shell interprets the following commands:

#  movetodir directory

It is used to change the current directory 
ex: movetodir documents/projects

# whereami

Prints the current directory.

# history [-c]

Without the parameter, prints out the recently typed commands (with their arguments). If “-c” is passed, it clears the list of recently typed commands 

# byebye

terminates mysh

# start program [parameters]

The argument “program” is the program to execute. If the argument starts with a “/” (such as /usr/bin/xterm) it will be interpreted as a full path. Otherwise, the program will be interpreted as a relative path starting from the current directory. 

ex: run /usr/bin/xterm –bg green  would bring up a terminal with a green background. 

# background program [parameters]

Similar to the run command, but it immediately prints the PID of the program it started and returns the prompt. 

# exterminate PID

Immediately terminate the program with the specific PID 

# repeat n command [parameters]

n is the number of times the command must be run, command as the full path to the program to execute, and the others as parameters.  The command starts the specified number of program instances, prints the PIDs of the created processes and then return to the prompt, without waiting for the processes to terminate

ex: repeat 4 /usr/bin/xterm 
would bring up 4 terminals and print out something like:
parent started child with PID of: 31012, 31013, 31014, 31015,

# exterminateall

Immediately terminates all the programs previously started by the mysh shell which had not been previously terminated by it, or by exterminate. It should output something like this:

Exterminating PIDs: 31012, 31013, 31014, 31015,

