**Simple Shell with Pipes**

** Purpose and Learning Outcomes:**
- Understanding processes and the use of fork, exec, and wait functions
- Understanding error handling
- Reading user input
- Tokenizing input
- Getting user input and processing end-of-file condition
- Understanding and using Vectors (arrays of pointers)
- Comprehensive use of redirection of standard input and output and the use of pipes

**General expectations for all projects:** 
- All projects are done in C in the Ubuntu Linux Virtual Machine.
- Code must be neat, with proper and consistent indentation and well documented. 
- Keep line to around 80 characters per line, and not line greater than 100 characters.
- Comments must describe the logic and reason for the code and not repeat the code.  
- Variables must have meaningful names and be in a consistent format (I do not care if you use camelCase or under_scores in variables but be consistent.
- You must use `make` to compile the program.
- You must use `make run` (sometimes with RUNOPTIONS) to execute the program.
- In addition, each file must have the standard header as defined below.
- All source files and writeup must be in the main branch of the github.
  
For this assignment you will implement your own shell that runs on top of the regular command-line interpreter for Linux.  

Your shell should read lines of user input, then parse and execute the commands by forking/creating new processes.  For each command, your shell should call fork() followed by execvp().  Following each command, your shell should wait for its child process to complete, and then print the child PID and the **return result from the child process**.  The user should be able to specify the command to execute by giving a path to the executable file (e.g. `/bin/ls`) or by using path expansion to locate the executable file (i.e. searching each directory in the PATH environment variable).  (Note that the execvp() function perform this processing automatically; you do not need to program it yourself.)  

If your shell encounters an error while reading a line of input it should report the error and exit.  If your shell encounters EOF while reading a line of input, it should exit gracefully without reporting an error.
Ensure that you do not overflow a 177 byte buffer when fetching the line of input (functions that do not accept the size of your buffer are not able to prevent overflows whereas functions that do accept a size generally do; be sure to check the manpage of any function you use carefully).  You do not need to report an error if the user's input line is larger than the 177 byte buffer; just use the truncated input as the command.

You can test the EOF by running `make run < commands.txt` or from the prompt enter `CTRL-D`

Before your shell forks a new process to call `execvp()`, it should parse the input string and separate it into a collection of substrings representing the executable file and any command-line arguments.  If the user entered an empty line, report an error and fetch a new line of input.  Your code must handle at least four command-line arguments (in addition to the name of the executable file itself) for each command.  


You should store pointers to the substrings in an array (similar to the “argv” array passed to main()) and pass this array of arguments to execvp().  Note that the number of command-line arguments is variable; this is indicated in the array by including a NULL pointer in the array after the last substring.  (This means that if the user specifies N substrings, your array must hold N + 1 pointers where the last pointer is NULL.)  If the user enters the **exit** command, your shell should terminate (returning to the regular shell).

Note: your shell does not need to support `cd` (change directory).

Piping - You shell must also support piping.  An example of using pipes in the Linux shell would be `cat myfile.txt | wc -l` which would copy the file myfile.txt to standard output which is redirected as the standard input to wc (wordcount) which will then display how many lines were in the file myfile.txt.  The pipe character `|` seperates different commands and the output (stdout) of the program on the left becomes the input (stdin) for the program on the right.  There is **no limit** to the number of commands that can be piped together.  (Just limited by the command line length specified above).  

Hint: get your shell working without pipes first, then go back and add pipes.  While this may require a little more work to change your code to support pipes, it ensures you do the rest of the shell correctly so you are not trying to debug multiple things at once.  Some functions you will need include `pipe` and `dup2`.

Your program must also accept a command line argument which is the prefix prompt.  If no value is specified use “> ” as the prompt.
