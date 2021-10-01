#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>


#define READ 0
#define WRITE 1


//ADDED LINES
#include <sys/types.h>
#include <sys/wait.h>


//Funcion utilizada para dividir las cadenas de caracteres dependiendo del delimitador.
void  split(char string[], char *stringdiv[], const char delimiter[] ){
	char *token = strtok(string, delimiter);
	int n = 0;
	while(token != NULL){
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
 	}else if (pid == -1){
  	perror("fork() failed");
  	exit(EXIT_FAILURE);
 	}else{
 	//Esta seccion es ejecutada por el padre
  	close(fd[READ]);
  	dup2(fd[WRITE], STDOUT_FILENO);
  	execvp(comand[0], comand);
 	}
}






int main(void) {

	int t, estado;
	t = 1;		
	
  	printf("At your command: \n");
  
  	while(t){	
  
  
  
		printf(">>>  ");
	  	char  entrada[100];//Se crea un array de cadenas de caracteres para almacenar la entrada
	  	scanf("%[^\n]", entrada);//Se pide la entrada al usuario, por teclado
	  	/*Se busca en la cadena de entrada ya sea el simbolo de pipe o el de redireccionamiento para manejar cada caso por 		separado.
	  	Este if se ejecuta si en la cadena se encuentra el simbolo de pipe*/
  
  
  
	  	//ADDED
	  	getchar();
	  	int counter = 0;
  
	  	/*
	  	while(entrada[counter] != '\0'){
	  		printf("%c \n", entrada[counter]);
	  		counter++;
	  	}	*/
  
  
  
	  	pid_t sonsPid;		//ADDED LINES
	    	sonsPid = fork();    
    	
	    	//if (strcmp(cmd[0], "exit") == 0){
	    	char *commandsArray[15];
	    	split(entrada, commandsArray," ");
	    	if (strcmp(commandsArray[0], "exit") == 0)
	    		t = 0;
	    	//}
    	
    	
if (sonsPid < 0){ 			//ADDED LINES
    		//fork failed
    		fprintf(stderr, "fork failed\n");
    		exit(1);
    	} else if (sonsPid == 0){
    		//child, commands will be executed here:  	
  	
  	
  	
  	
  	
  	
  	
  	
  	
  	
  	
  	
  	
  
	  	if( strstr(entrada, "|") != NULL ){
	   		char *pipe[3];
	   		split(entrada, pipe, "|");//Se separa la entrada en las dos subcadenas delimitadas por el simbolo|
		                             //Cada una de ellas contiene un comando y los argumentos
	                            
	   		while(pipe[counter] != NULL){
	  			printf("%s \n", pipe[counter]);
	  			counter++;
	  		}	 
                             
		   	char *firstComand[4];  
		   	char *secondComand[4];
		   	split(pipe[0], firstComand, " ");//Se separa la cadena por palabras
		   	split(pipe[1], secondComand, " ");
		   	pipefunction(firstComand, secondComand);
	  	}else if( strstr(entrada, ">") != NULL){//Se ejecuta si se encuentra el simbolo de direccionamiento
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
  

//ADDED LINES
} else {
    	    //printf("\nDad is here\n");
    	    printf("\nBig Daddy is here\n");
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




