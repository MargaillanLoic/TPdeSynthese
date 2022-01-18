
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>			// On ajoute la librairie suivante pour pouvoir utiliser les horloges



int main(void)
{
	int entree, status;
	char *stringIn=malloc(64*sizeof(char));
	char message[64]={0};				// chaine de caractère qui contiendra le temps écoulé pour l'execution d'une commande
	struct timespec start, stop;			// Structures définies dans time.h qui permettent d'accéder aux secondes et aux nanosecondes
	
	write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", strlen("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"));
	
	while(1){
		write(STDOUT_FILENO, "enseash % ", strlen("enseash % "));
		entree = read(STDIN_FILENO, stringIn, 64);
		stringIn[entree-1] = '\0';
		clock_gettime(CLOCK_REALTIME, &start); 		// On récupère le temps dans la structure start lorsqu'on lance une commande 
		
		int pid = fork();
		if(pid==0){
			
		//Dans le fils:
			int retour = execlp(stringIn, stringIn,NULL);
			if (retour ==-1){ 
				exit(EXIT_FAILURE);
			}
		}
		else if(pid!=0){
			
		//Dans le père:
			wait(&status);
			clock_gettime(CLOCK_REALTIME, &stop);				 // On récupère le temps dans la structure stop lorsqu'on finit la commande
			float temps = (stop.tv_nsec - start.tv_nsec)/1000000;		 // On divise ici le temps écoulé (en ns) pour afficher le résultat en ms
		
			if(WIFEXITED(status)){
				write(STDOUT_FILENO, "enseash [exit:", strlen("enseash [exit:")); 
				sprintf(message, "%d|%.2f ms",WEXITSTATUS(status), temps);
				write(STDOUT_FILENO,message,strlen(message));
				write(STDOUT_FILENO,  "] %", strlen( "] %")); 

			}
			else if(WIFSIGNALED(status)){
				write(STDOUT_FILENO, "enseash [sign:", strlen("enseash [sign:"));
				sprintf(message, "%d|%.2f ms",WEXITSTATUS(status), temps);
				write(STDOUT_FILENO,message,strlen(message));
				write(STDOUT_FILENO,  "] %", strlen( "] %"));

			}
		}
		
		if(!strncmp("exit",stringIn,4) || (entree == 0)){ 
			write(STDOUT_FILENO, "Bye bye ...\n", strlen("Bye bye ...\n"));
			exit(EXIT_SUCCESS); 
		}
	}
	exit(EXIT_SUCCESS); 
}
