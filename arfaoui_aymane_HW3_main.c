/**************************************************************
* Class::  CSC-415-03 Spring 2024
* Name::Aymane Arfaoui
* Student ID::923812458
* GitHub-Name::Aymane-Arfaoui
* Project:: Assignment 3 – Simple Shell with Pipes
*
* File:: arfaoui_aymane_HW3_main.c
*
* Description::
*COMPLETE LATER ------------------------------------------------
*Implement a simple shell that run on top of the regular 
*command line interpreter on Linux. Must handle piping.
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

//define our readen and writeend
#define READEND 0
#define WRITENED 1

//somewhat similar structure to assignment2 buffer
void parseFunc(char *cmdline, char **argv) {
    while (*cmdline != '\0') {
        //replace spaces and new line with \0
        while (*cmdline == ' ' || *cmdline == '\n')
         *cmdline++ = '\0'; 
         //update array of poitners
         *argv++ = cmdline;
         while (*cmdline != '\0' && *cmdline != ' ' && *cmdline != '\n')
         cmdline++;
    }
    *argv = '\0';
}

int numPipes(char **argv){
    for (int i = 0; argv[i] != NULL; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            // Found a pipe 
            return 1;
        }
    }
    return 0;
}

int executeFuncWithPipes(char **argv){
        // Create an array of everything before the pipe 
        int pipeIndex = -1;

        for(int i =0; argv[i] != NULL; ++i){
             if (strcmp(argv[i], "|") == 0){
                pipeIndex = i;
                break;
            }
        }
        if (pipeIndex == -1){
            return -1;
        }
        //Separate arguments from pipe before and after and the pipe.
         char *argvBeforePipe[pipeIndex +1]; 
         char *argvAfterPipe[pipeIndex+1];

         for(int i = 0; i < pipeIndex; i++){
            argvBeforePipe[i] = argv[i];
         }
         argvBeforePipe[pipeIndex] = NULL;

         for(int i = 0; i < pipeIndex; i++){
            argvAfterPipe[i] = argv[pipeIndex+1 +i];
         }
            argvAfterPipe[pipeIndex] = NULL;
        
         //file descriptor initialization
          int fd[2];
          if(pipe(fd) == -1){
            return 1;
          }

          int pid1 = fork();
          int status;

          if (pid1 < 0){
            return 2;
          }

          if (pid1 == 0){
            //child 
            printf("\nChild PID: %d\nReturn Status: %d\n", pid1, status);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(argvBeforePipe[0], argvBeforePipe);
          }
            int pid2 = fork();
                if (pid2 < 0){
                    return 3;
                }

                if (pid2 == 0){
                    // child process ()
                    printf("\nChild PID: %d\nReturn Status: %d\n", pid2, status);
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    execvp(argvAfterPipe[0], argvAfterPipe);
                }
            close(fd[0]);
            close(fd[1]);

            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            return 0;
}


void executeFunc(char **argv) {
    pid_t parent_pid = getpid();
    pid_t pid = fork();
    int status;

    if ((pid) < 0) {
        perror("forking faild for child");
        exit(1);
    } else if (pid == 0) {
        // Child 
        printf("\nChild PID: %d\nReturn Status: %d\n", pid, status);
        // argv is an array of strings
        //execute commands
        if (execvp(argv[0], argv) < 0) { //argv[0] because name of the path
            perror("Exec failed\n");
            exit(1);
            if(numPipes(argv)){
                printf("We got a pipe !");
            }
        }

        //End of Child
    } else {
        // Parent 
        wait(&status);
    }
}

int main(int argc, char *argv1[]) {
//array of characters
 char cmdline[178];

 //array of pointers to char
 char *argv[178/2];

    while (1) {
        printf("%s",argv1[1]);
        if (fgets(cmdline, sizeof(cmdline), stdin) == NULL) {
            // End of file 
            printf("End of file reached. Exiting.\n");
            exit(0);
        }
    if (cmdline[0] == '\n') continue;
     cmdline[strcspn(cmdline, "\n")] = '\0';
    parseFunc(cmdline, argv);
        if (strcmp(argv[0], "exit") == 0)
    exit(0);


    // Dealing with pipes 
   if (numPipes(argv) > 0) {
    int pipeCount = numPipes(argv);

    while (pipeCount > 0) {
        executeFuncWithPipes(argv);

        // Find next pipe and update argv
        int nextPipe = -1;
        for (int i = 0; argv[i] != NULL; ++i) {
            if (strcmp(argv[i], "|") == 0) {
                nextPipe = i;
                break;
            }
        }
        // update argv to remove everything before the second part (needs work)
        if (nextPipe!= -1) {
            for (int i =nextPipe;argv[i] !=NULL; ++i) {
                argv[i -nextPipe] =argv[i+ 1];
            }

            argv[nextPipe] = NULL;
            pipeCount--;
        }

        
    }
} else {
    //dealing with no pipes
    executeFunc(argv);
}
}
    return 0;
}