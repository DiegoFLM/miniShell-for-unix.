/**
 * @file miniShell.c
 * @author Diego Ledesma
 * @code 1928161
 * @author José David Barona Hernández 
 * @code 1727590
 * @brief intérprete de comandos que permita la ejecución de aplicaciones de un sistema Linux
 * @version 0.1
 * @date 2021-10-01
 */



#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1


void  separator(char string[], char *instructions[], const char breaker[] ){
  char *command = strtok(string, breaker);
  int n = 0;
  while(command != NULL)
  {
    instructions[n] = command;
    n++;
    command = strtok(NULL, breaker);
    instructions[n] = NULL;
  }
}


void redirection(char *cmd[] , char *out[]){
  int file = open(out[0], O_CREAT | O_WRONLY |O_APPEND); 
  dup2(file, STDOUT_FILENO);  
  close(file);
  execvp(cmd[0], cmd);
  exit(EXIT_FAILURE);
  
}


void pipeLine(char *cmd[], char *args[]){
  pid_t pid;
  int fd[2];
  if (pipe(fd) == -1) {
    perror("Pipe fork failed");
    exit(EXIT_FAILURE);
  }
  pid = fork();
  if (pid == 0){
    close(fd[WRITE]); 
    dup2(fd[READ], STDIN_FILENO);
    execvp(args[0], args);
    exit(EXIT_FAILURE);
  
  }else if (pid == -1){
    perror("fork() failed");
    exit(EXIT_FAILURE);
  
  }else{
    close(fd[READ]);
    dup2(fd[WRITE], STDOUT_FILENO);
    execvp(cmd[0], cmd);
    exit(EXIT_FAILURE);
  }
}


int main(void) {
  printf("mini Welcome");

  char  input[100];
  
  
  int t = 1; 
  int state1, state2, state3;

  while(t){

    memset(input, '\0', 100); //Resets the array


    printf("\nAt your command: ");
  
    scanf(" %[^\n]", input); /*Gets the users input; 
    the %[^\n] conversion specification matches a string 
    of all characters not equal to the new line character ('\n') 
    and stores it (plus a terminating '\0' character) in str.*/

    int a = 0;
    while (input[a] != '\0'){
      printf("\n%c\n", input[a]);
      a++;
    }
  
    if( strstr(input, "exit") != NULL ){
      t = 0;
    }else{
  
    if( strstr(input, "|") != NULL ){
      char *pipe[3];
      separator(input, pipe, "|");
    
  
    
      char *firstcmd[4];  
      char *secondcmd[4];
      separator(pipe[0], firstcmd, " ");
      separator(pipe[1], secondcmd, " ");


      pid_t sonsPidPipe;
      sonsPidPipe = fork();  

      if (sonsPidPipe < 0){
        //fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
      }else if (sonsPidPipe == 0){
        pipeLine(firstcmd, secondcmd);
      }else{
        //printf("\nDad is here\n");
        wait(&state1);
     		
        if(WIFEXITED(state1)){
    	    if(WEXITSTATUS(state1)){
            printf("\n:(\n");
    	    }else{
            printf("\n:)\n");
          }    	
	      }
      }

    }else if( strstr(input, ">") != NULL){
      char *redirect[3];
      separator(input, redirect, ">");
      char *cmd[4];
      char *out[2];
      out[0] = redirect[1];
      out[1] = NULL;
      separator(redirect[0], cmd, " ");

      //Fork
      pid_t sonsPidRed;
      sonsPidRed = fork();  

      if (sonsPidRed < 0){
        //fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
      }else if (sonsPidRed == 0){
        redirection(cmd, out);
      }else{

        //printf("\nDad is here\n");
        wait(&state2);
     		
        if(WIFEXITED(state2)){
    	    if(WEXITSTATUS(state2)){
            printf("\n:(\n");
    	    }else{
            printf("\n:)\n");
          }
	      }
    
      }
    
    }else{
      char *args[7];
      separator(input, args, " ");

      pid_t sonsPidOthers;
      sonsPidOthers = fork();  

      if (sonsPidOthers < 0){
        //fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
      }else if (sonsPidOthers == 0){
        execvp(args[0], args);
        exit(EXIT_FAILURE);
      }else{

        //printf("\nDad is here\n");
        wait(&state3);
     		
        if(WIFEXITED(state3)){
    	    if(WEXITSTATUS(state3)){
            printf("\n:(\n");
    	    }else{
            printf("\n:)\n");
          }    	
	      }
      }

    }

    }

  }

  return 0;
}





