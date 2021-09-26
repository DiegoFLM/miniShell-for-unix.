#include <stdio.h>
#include <string.h>
#include <strings.h>

 
#include<stdlib.h>
#include<unistd.h>

#include<assert.h>
#include<fcntl.h>


#include<sys/wait.h>

#define READ 0
#define WRITE 1
 
int main()
{

/* Code for split string into commands: https://www.includehelp.com/c-programs/c-program-to-split-string-by-space-into-words.aspx */

    char str[100];
    char commands[15][15]; //can store 15 commands of 15 characters
    int i,j,cnt;
    int estado;
    int t = 1;
    
    //PIPE
    int fd[2];
    int pipeTotal;
    int pipeLocs[10];
    
    //REDIRECT
    int redirectTotal;
    int redirectLocs[5];
    int file;
    
    
    
    
    if (pipe(fd) == -1){
    	perror("Creating pipe");
    	exit(EXIT_FAILURE);
    }
    
    
    
    
    
 	
    while(t){
 
    	printf("At your command: ");
    	fgets(str, 100, stdin);
 
 
 	int pipeLoc[10], redirectLoc[5];
	j=0; cnt=0; pipeTotal = 0, redirectTotal = 0;
    	for(i=0;i<=(strlen(str));i++)
    	{
        	// if space or NULL found, assign NULL into commands[cnt]
        	if(str[i]==' '||str[i]=='\0' || str[i] == '\r' /*|| str[i] == '$'*/ || str[i] == '\n')
        	{
        		if(str[i - 1]==' ' && str[i] == ' ') {
        		}else {
        			commands[cnt][j]='\0';
            			cnt++;  //for next command
            			j=0;    //for next command, init index to 0
            		}
        	}else if(str[i] == '|'){
        		pipeLocs[pipeTotal] = cnt - 1;
        		pipeTotal++;
        		
        	}else if (str[i] == '>'){
        		redirectLocs[redirectTotal] = cnt - 1;
        		redirectTotal++;
        		
        		
        	}
        	else{
            		commands[cnt][j]=str[i];
            		j++;
        	}
    	}
    	
        
    	int argc;    
    	char *cmd[5];    
        
    	argc = 5;
    
    	for(i=0;i < cnt;i++){
    		cmd[i] = commands[i];
    	}
    	cmd[i - 1] = NULL;
    
    
    
    	pid_t sonsPid;
    	sonsPid = fork();    
    	
    	if (strcmp(cmd[0], "exit") == 0){
    		t = 0;
    	}
    	
    	//PIPE
    	if (pipeTotal > 0){
    		/*PIPE*/
    		for (int pipeCounter = 0; pipeCounter < pipeTotal; pipeCounter++){
    			if (sonsPid == -1){
    				perror("fork() failed");
    				exit(EXIT_FAILURE);
    			}else if(sonsPid == 0){ //Child process
    				//Close the pipe write descriptor.
    				close(fd[WRITE]);
    				//Redirect STDIN to read from the pipe.
    				dup2(fd[READ], STDIN_FILENO);
    				
    			}
    		}	
    		continue;	
    	} else if (redirectTotal > 0) {		//Redirection
    		file = open("output.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    		assert(file != -1);
    		dup2(file, STDOUT_FILENO);
		
		if (sonsPid < 0){
    			//fork failed
    			fprintf(stderr, "fork failed\n");
    			exit(1);
    		} else if (sonsPid == 0){
    			//child, commands will be executed here:
    	
    			execvp(cmd[0], cmd); /*Sons process deads and is replaced by the commands execution*/
    			printf("\nExecvp failed to execute.\n");
    			exit(EXIT_FAILURE);
    	
    		} else {
    		    //printf("\nDad is here\n");
    		    wait(&estado);
    	        if(WIFEXITED(estado)){
    	            if(WEXITSTATUS(estado)){
                        printf(":(");
    	            }else{
                        printf(":)");
                    }
	    	
	    	}
    	}
		
		
		    		
    		close(file);
    		return 0;
    		
    		continue;
    	}
    	
    	continue;
    	
    	
    	
    	
    	
    	
    	
    	
    	
        
    	if (sonsPid < 0){
    		//fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
    	} else if (sonsPid == 0){
    		//child, commands will be executed here:
    	
    		execvp(cmd[0], cmd); /*Sons process deads and is replaced by the commands execution*/
    		printf("\nExecvp failed to execute.\n");
    		exit(EXIT_FAILURE);
    	
    	} else {
    	    //printf("\nDad is here\n");
    	    wait(&estado);
            if(WIFEXITED(estado)){
                if(WEXITSTATUS(estado)){
                        printf(":(");
                }else{
                        printf(":)");
                }
	    	
	    }
    	}
    
    
    }
        
        
    return 0;
}







