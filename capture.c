#include <stdio.h>
#include <string.h>
#include <strings.h>

#include<stdlib.h>
#include<unistd.h>

#include<assert.h>
#include<fcntl.h>

#include<sys/wait.h>


void capture(char** cmd){
	char str[100];
    	char commands[15][15]; //can store 15 commands of 15 characters
	int i,j,cnt;
    	int estado;
	
	
	printf("At your command: ");
    	
    	bzero(str, 100);
    	fgets(str, 100, stdin);
 
 	
 	int pipeLoc[10], redirectLoc[5];
	j=0; cnt=0;
    	for(i=0;i<=(strlen(str));i++){
        	// if space or NULL found, assign NULL into commands[cnt]
        	if (str[i] == '\n'){
        		commands[cnt][j] = '\0';
        		cnt++;	
        		break;
        	}
        	
        	if(str[i]==' '||str[i]=='\0' || str[i] == '\r' || str[i] == '>'){
        		
        		
        		
        		commands[cnt][j]='\0';
            		cnt++;  //for next command
            		j=0;    //for next command, init index to 0
            	
            	}else{
            		commands[cnt][j]=str[i];
            		j++;
        	}
    	}
    	   
    	//char *cmd[15];    
        
    	
    	
    	for(i=0;i < cnt;i++){
    		cmd[i] = commands[i];
    		printf("%s\n", commands[i]);
    	}
    	
    	cmd[i] = NULL;
    	
    	/*for(){
    	
    	}*/
    	
}


int main(){
	
	char *com[15];
	capture(com);
	
	

	return 0;
}



