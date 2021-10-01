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
    int t;
    
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
    	
    	bzero(str, 100);
    	fgets(str, 100, stdin);
 
 	char commands[15][15];
 	int pipeLoc[10], redirectLoc[5];
	j=0; cnt=0; pipeTotal = 0, redirectTotal = 0;
    	for(i=0;i<=(strlen(str));i++)
    	{
        	// if space or NULL found, assign NULL into commands[cnt]
        	if(str[i]==' '||str[i]=='\0' || str[i] == '\r' /*|| str[i] == '$'*/ || str[i] == '\n' || str[i] == '>'){
        		if(str[i - 1]==' ' && str[i]=='>'){
        			redirectTotal++;
        			redirectLocs[redirectTotal] = cnt - 1;
        			//cnt--;
        		}else if ((str[i] == ' ') && (str[i - 1] == '>')) {
        			cnt++;
        			/*if (str[i] == '>'){
	        		redirectTotal++;
        			redirectLocs[redirectTotal] = cnt - 1;
        			
        			}*/
        		}else {
        			commands[cnt][j]='\0';
            			cnt++;  //for next command
            			j=0;    //for next command, init index to 0
            		}
        	}else if(str[i] == '|'){
        		pipeLocs[pipeTotal] = cnt - 1;
        		pipeTotal++;
        		
        	}/*else if (str[i] == '>'){
        		redirectTotal++;
        		redirectLocs[redirectTotal] = cnt - 1;
        		
        		
        		
        	}*/else{
            		commands[cnt][j]=str[i];
            		j++;
        	}
    	}
    	
    	
        
    	  
    	char *cmd[15];    
        
    	
    	if (redirectTotal > 0){
    		//cnt--;
    	}
    
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
    		
		printf("\nENTRAMOS REDIRECT\n");
		/*int file = open("output.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    		assert(file != -1);
    		dup2(file, STDOUT_FILENO);*/  //What before was to be sent to STDOUT_FILENO, now will be sent to file
		
		if (sonsPid < 0){
    			//fork failed
    			fprintf(stderr, "fork failed\n");
    			exit(1);
    		} else if (sonsPid == 0){
    			//child, commands will be executed here:
    			
    			
    			for (int x = 0; x < 10; x++){
    				/*for (int y = 0; y < 10; y++){
    					printf("%c",cmd[x][y]);
    				}*/
    				printf("%s", cmd[x]);
				printf("\n");    			
    			}
    			
    			
    			int file = open("output.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    			assert(file != -1);
    			dup2(file, STDOUT_FILENO);
    			
    			
    			printf("\nEntered: redirect-sonsPid\n");
    			execvp(cmd[0], cmd); //Sons process deads and is replaced by the commands execution
    			printf("\nExecvp failed to execute.\n");
    			exit(EXIT_FAILURE);
    			
    		} else {
    		    //printf("\nDad is here\n");
    		    wait(&estado);
    		    //close(file);
    		    //dup2(STDOUT_FILENO, file);
    		    		
    	        if(WIFEXITED(estado)){
    	        	//close(file);		//LLA
    	            	if(WEXITSTATUS(estado)){
                        	//close(file);
                        	printf("\n:(\n");
    	            	}else{
                        	//close(file);
                        	printf("\n:)\n");
                    	}
	    	
	    	}
    	}
		//close(file);
    		continue;
    	}
    	
    	//continue;
    	
    	
    	
    	
    	
    	
    	
    	
    	
        
    	if (sonsPid < 0){
    		//fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
    	} else if (sonsPid == 0){
    		//child, commands will be executed here:
    	
    		for (int x = 0; x < 10; x++){
    				/*for (int y = 0; y < 10; y++){
    					printf("%c",cmd[x][y]);
    				}*/
    				printf("%s", cmd[x]);
				printf("\n");    			
    			}
    	
    		printf("Normal execvp");
    		execvp(cmd[0], cmd); /*Sons process deads and is replaced by the commands execution*/
    		printf("\nExecvp failed to execute.\n");
    		exit(EXIT_FAILURE);
    	
    	} else {
    	    //printf("\nDad is here\n");
    	    printf("Something is not working");
    	    wait(&estado);
            if(WIFEXITED(estado)){
                if(WEXITSTATUS(estado)){
                        printf("\n:(\n");
                }else{
                        printf("\n:)\n");
                }
	    	
	    }
    	}
    
    
    }
        
        
    return 0;
}










#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>


#define READ 0
#define WRITE 1

//Funcion utilizada para dividir las cadenas de caracteres dependiendo del delimitador.
void  split(char string[], char *stringdiv[], const char delimiter[] ){
 char *token = strtok(string, delimiter);
 int n = 0;
 while(token != NULL)
 {
  stringdiv[n] = token;
  n++;
  token = strtok(NULL, delimiter);
  stringdiv[n] = NULL;
 }
}
//Funcion que se encarga de redireccionar
void redirectfunction(char *comand[] , char *out[]){
 int file = open(out[0], O_CREAT | O_WRONLY |O_APPEND); //Se abre el archivo donde se guardara la salida del comando a ejecutar
 dup2(file, STDOUT_FILENO); //Permite que se guarde la salida en el archivo file, en lugar de mostrarse por pantalla
 close(file);
 execvp(comand[0], comand);
}

//Funcion que se encarga de pipe funcion
void pipefunction(char *comand[], char *args[]){
 pid_t pid;
 int fd[2];
 if (pipe(fd) == -1) {
  perror("Creating pipe");
  exit(EXIT_FAILURE);
 }
 pid = fork();
 if (pid == 0){
 //Esta seccion de codigo es ejecutada por el hijo
   close(fd[WRITE]); //Se cierra el descriptor de escritura del pipe
   dup2(fd[READ], STDIN_FILENO);//Permite leer la salida que retorna el padre
   execvp(args[0], args);
   printf("Esto no se deberia imprimir\n");
 }
 else if (pid == -1){
  perror("fork() failed");
  exit(EXIT_FAILURE);
 }
 else{
 //Esta seccion es ejecutada por el padre
  close(fd[READ]);
  dup2(fd[WRITE], STDOUT_FILENO);
  execvp(comand[0], comand);
 }
}


int main(void) {
  printf("**********MiniShell**********\n");
  printf(">>>  ");
  char  entrada[100];//Se crea un array de cadenas de caracteres para almacenar la entrada
  scanf("%[^\n]", entrada);//Se pide la entrada al usuario, por teclado
  /*Se busca en la cadena de entrada ya sea el simbolo de pipe o el de redireccionamiento para manejar cada caso por separado.
  Este if se ejecuta si en la cadena se encuentra el simbolo de pipe*/
  if( strstr(entrada, "|") != NULL ){
   char *pipe[3];
   split(entrada, pipe, "|");//Se separa la entrada en las dos subcadenas delimitadas por el simbolo|
                             //Cada una de ellas contiene un comando y los argumentos
   char *firstComand[4];  
   char *secondComand[4];
   split(pipe[0], firstComand, " ");//Se separa la cadena por palabras
   split(pipe[1], secondComand, " ");
   pipefunction(firstComand, secondComand);
  }
  else if( strstr(entrada, ">") != NULL){//Se ejecuta si se encuentra el simbolo de direccionamiento
   char *redirect[3];
   split(entrada, redirect, ">");/*Se separa la entrada en dos subcadenas, una de ellas contiene un comando
                                   junto con sus argumentos, la otra contiene el nombre del archivo  donde
                                   se va a redireccionar la salida*/
   char *comand[4];
   char *out[2];
   out[0] = redirect[1];
   out[1] = NULL;
   split(redirect[0], comand, " ");
   redirectfunction(comand, out);
  }else{//Se ejecuta para cualquier comando distinto a casos anteriores
    char *args[7];
    split(entrada, args, " ");
    execvp(args[0], args);
  }
  return 0;
}








