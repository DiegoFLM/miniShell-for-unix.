#include <stdio.h>
#include <string.h>
 
#include<stdlib.h>
#include<unistd.h>
 
int main()
{

/* Code for split string into commands: https://www.includehelp.com/c-programs/c-program-to-split-string-by-space-into-words.aspx */

    char str[100];
    char commands[15][15]; //can store 15 commands of 15 characters
    int i,j,cnt;
 
    printf("Enter a string: ");
    fgets(str, 100, stdin);
 
    j=0; cnt=0;
    for(i=0;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into commands[cnt]
        if(str[i]==' '||str[i]=='\0')
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
    printf("\nOriginal String is: %s",str);
    printf("\nStrings (commands) after split by space:\n");
    
        
    printf("\nNumber of commands: %d\n", cnt);
        
    int argc;    
    char *cmd[5];    
        
    argc = 5;
    
    for(i=0;i < cnt;i++){
        printf("%s\n",commands[i]);
    	cmd[i] = commands[i];
    	printf("%s\n",cmd[i]);
    }
    cmd[i] = NULL;
    printf("\nAfter for, i: %d\n", i);
    
    
    
    pid_t sonsPid;
    sonsPid = fork();    
        
    if (sonsPid < 0){
    	//fork failed
    	fprintf(stderr, "fork failed\n");
    	exit(1);
    } else if (sonsPid == 0){
    	//child, commands will be executed here:
    	
    	execvp(cmd[0], cmd); /*Sons process deads and is replaced by the commands execution*/
    	printf("\nExecvp failed to execute.\n");
    	
    } else {
	    //printf("\nDad is here\n");
    }
    
    
    	
        
        
    return 0;
}
