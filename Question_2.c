#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void main(){
	char* message = "$ ./enseash \nBienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'. \n";
	write(STDOUT_FILENO,message,strlen(message));
	
	char *prompt="enseash % ";
	char *stringIn=malloc(64*sizeof(char));		// on alloue de la mémoire pour pouvoir écrire nos commandes
	while(1){
		write(STDOUT_FILENO, prompt, strlen(prompt));		
		read(STDIN_FILENO, stringIn,64);			
		if ((strncmp(stringIn, "fortune", strlen("fortune"))==0)){
			pid_t pid=fork();					// on duplique le processus pour que le fils seulement éxecute la fonction fortune
			if (pid==0){
				execlp("fortune","f",NULL);			// permet d'éxecuter la fonction fortune dans le shell
			}
			else{
				wait(NULL);					// le processus parent attend que le processus fils ait finit pour continuer
			}	
		}
	}
}




