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


void  separator(char string[], char *Instructions[], const char breaker[] ){
  char *command = strtok(string, breaker);
  int n = 0;
  while(command != NULL)
  {
    Instructions[n] = command;
    n++;
    command = strtok(NULL, breaker);
    Instructions[n] = NULL;
  }
}


void redirection(char *cmd[] , char *out[]){
  int file = open(out[0], O_CREAT | O_WRONLY |O_APPEND); 
  dup2(file, STDOUT_FILENO);  
  close(file);
  execvp(cmd[0], cmd);
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
    printf("Pipe execvp failed\n");
  
  }else if (pid == -1){
    perror("fork() failed");
    exit(EXIT_FAILURE);
  
  }else{
    close(fd[READ]);
    dup2(fd[WRITE], STDOUT_FILENO);
    execvp(cmd[0], cmd);
  }
}


int main(void) {
  printf("mini Welcome");

  char  input[150];
  
  
  int t = 1; 
  int state1, state2, state3;

  while(t){

    memset(input, '\0', 150); //Resets the array


    printf("\nAt your command: ");
  
    scanf(" %[^\n]", input); //Gets the users input.
  
    if( strstr(input, "exit") != NULL ){
      t = 0;
    }
  
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
      
        printf("\n:(\n");

      }
    }



  }


  return 0;
}





