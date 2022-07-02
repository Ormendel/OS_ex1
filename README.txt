Overall:

This assignment was meant to teach us how to program a basic shell.

Compile and run by using the command: make (or make all if you would like)
Remove unnecessary files by using the command: make clean

If you want to check the part of the client-server please type: TCP PORT
Make sure you have 2 terminal windows open: the first is the main program which simulates shell 
Run by ./run
The second terminal window is for the server - run by ./server

For all other parts of the assigment you can use only one terminal (please type ./run)


Short explanations for our helper functions:

    1. get_current_dir - simply uses getcwd command and return the current directory you are at.

    2. echo_out - simulates the basic echo command of linux, with a given string after (like echo hello -> hello)

    3. delete_file - simulates rm command of linux, with a given string after, that represents the name of the file we would like to delete
    (like DELETE 1.txt). 
    This function also outputs if we were managed to delete the file successfully.

    4. copyFile - simulates mv command of linux, with 2 given strings after that represents the name of the files:
    one is the source (which we are copying from) - must exsists!
    the second is the destination (which we are copying to to) - not necessary exsists
    This function also outputs if we were managed to copy the source file to the destination file successfully.

    5. displayFiles - simulates dir command of linux, with no arguments.
    This function outputs to the user all of the files that exsists inside the directory you are currently at,
    and recrusively all of the files inside sub-directories of the current directory.

    6. change_directory - simulates cd command of linux.
    This function also outputs if we were managed to change our directory successfully.

    7. findFirstSpace - gets the first space inside a command - useful for the entire assignment.

    8. findCMD - uses findFirstSpace function in order to retrieve the "key" command - such as ECHO or CD


BUGS & ERRORS:
    1. Using the "exit" command by 2 options: system("exit") or execvp function is not working.
    We tried to solve this but unfortunately failed.

    2. Using "EXIT" after series of commands will achieve its goal and return to terminal only after
    the second or even third time, and that's because of the getchar function, which you can see at
    the end of main.c (line 466)

    3. While performing TCP PORT, we can send commands combined from only one word (with no space),
    that works great (like pwd, cal, date etc.).
    However, when we send to the server command that combines more than one word (sperated by a space character ' ')
    our program seprates the command like strtok():
    For example cal -3 will be:
    first will be execution of cal (ok)
    later execution of -3 (error)


Submitted by: 311382360 Eran Levy _ 315524389 Or Mendel