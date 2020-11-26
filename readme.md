                                    INTERACTIVE USER DEFINED SHELL
GOAL:

The goal of the assignment is to create a user-defined interactive shell program that can create and manage new processes.
Implemented a shell that supports a semi-colon separated list of commands (using strtok) Also, supports '&' operator which lets a program run in the background after printing the process id of the newly created process.Handles background , foreground process ,signals sent to them and i/o redirection and command pipelines.
code is written in modular fashion.

TO COMPILE :
    make
    
RUN:
    ./a.out

PROMPT:

When you execute your code, a shell prompts like this;
<username@system_name:curr_dir>

COMMANDS THAT ARE CUSTOM IMPLEMENTED:

1)echo 
2)quit
3)cd { cd<dirname>  cd.  cd.. cd ~  and cd -{bonus})
4)pwd
5) ls { all variations }
6)pinfo
7)setenv
8)unsetenv
9)overkill
10)History
11)jobs
12)fg
13)bg
IMPLEMENTATION:

commands will be passed onto a for loop one by one , check if its  custom commands  else use fork execute  execvp in child , if command has & and ask parent to wait , else continue.

custom commands are called from main () , each command's code is written in separate file .
ECHO:
using echo you can print on terminal
example; echo hai gives hai as output

PWD :
gives path of current working directory

PINFO [pid]:
gives info about process pid , status , vm size (using contents in /proc/pid/status file)and executable path.

LS:
works along with flags -l , -a , -al and directories
shows directory permissions and list all directories and files
uses stat to check permissions
opendir and readdir are used to read files in given directory

CD:
opens the specified directory


ls and cd both work with absolute and relative paths

Background and Foreground processes :
Any command invoked with "&" is treated as background process.builtin commands are executed by execvp ,and signal is used for knowing the status of background process.


HISTORY:
displays last 10 commands used (least recent to most) 

SETENV var [value]:
sets the environment variable var to value.

UNSETENV var:
destroys the environment variable var ,if it exists.

OVERKILL:
kills all backprocesses at once 

JOBS:
displays all currently running bg processes.
os version used:
ubuntu 18.04

QUIT:
terminates the shell program

I/O REDIRECTION:
implemented < (used for input), > (for output) , >> (for appending into output) ,which are usually used to redirect input /output or both .If output file doesnt exists it creates one.
Both input and output redirection works simultaneously.


COMMAND PIPELINES:
pipe redirects output of left command as input to right,this shell can support multiple pipes.


I/O redirection with COMMAND PIPELINES:
this shell program can handle io redirection within command pipelines

SIGNALS:
CTRL +C:
interrupts currently running foreground job by sending SIGINT signal.

CTRL +Z:
pushes any currently running foreground job into background ,changes its state from running to stopped.

FG:
brings background processes to fg and changes state from running to stopped

BG:
starts the stopped processes in bg and no affect on running processes



some assumptions/demerits:
 1)commands are of size less than or equal to 1024 works
 2)in proc/pid/status file hardcodes numbers to get desired output (it wasnt mentioned in manpage that file structure changes through diff. versions of linux )
3)max of 20 ; separated commands can be passed at once
4)error handling for all syscalls havent been done
5)need to pass valid arugments ( for custom commands especially)





