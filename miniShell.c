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

int main(void){

	char input[100];
	scanf("%[^\n]",input); //se busca el símbolo pipeline | o redireccionamiento >

	if(strstr(input, "|") != NULL){
		char *pipe[3]
		split(input,pipe,"|"); //separamos el input en dos subcadenas separadas con el simbolo de pipeline |

		char *comand1[4];
		char *comand2[4];

		split(pipe[0],comand1, " ");
		split(pipe[1],comand2, " ");
		pipeFunction(comand1,comand2);
	}
	else if(strstr(input, ">") != NULL){
		char *redirect[3];
		split(input,redirect, ">");/*Se separa la entrada en dos subcadenas, una de ellas contiene un comando
                                   junto con sus argumentos, la otra contiene el nombre del archivo  donde
                                   se va a redireccionar la salida*/
		char *comand[4];
		char *out[2];

		out[0] = redirect[1];
		out[1] = NULL;

		split(redirect[0], comand, " ");
		redirectfunction(comand,out);

	}else{
		char *args[7];
		split(input,args," ");
		execvp(args[0],args);

	}
	return 0;
}
