#include <stdio.h>
#include <string.h>
 
#include<stdlib.h>
#include<unistd.h>

#include<sys/wait.h>
 
int main()
{

/* Code for split string into commands: https://www.includehelp.com/c-programs/c-program-to-split-string-by-space-into-words.aspx */

    char str[100];
    char commands[15][15]; //can store 15 commands of 15 characters
    int i,j,cnt;
    int estado;
    int t = 1;
 
    while(t){
 
    	printf("At your command: ");
    	fgets(str, 100, stdin);
 
 
 
	j=0; cnt=0;
    	for(i=0;i<=(strlen(str));i++)
    	{
        	// if space or NULL found, assign NULL into commands[cnt]
        	if(str[i]==' '||str[i]=='\0' || str[i] == '\r' /*|| str[i] == '$'*/ || str[i] == '\n')
        	{
            		commands[cnt][j]='\0';
            		cnt++;  //for next command
            		j=0;    //for next command, init index to 0
        	}
        	else
        	{
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
