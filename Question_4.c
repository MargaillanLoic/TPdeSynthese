#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

//Dans 


int main(void)
{
	int entree, status;
	char *stringIn=malloc(64*sizeof(char));

	char Etat[1];
	write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", strlen("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"));
	
	while(1){
		write(STDOUT_FILENO, "enseash % ", strlen("enseash % "));
		entree = read(STDIN_FILENO, stringIn, 64);
	
		
		stringIn[entree-1] = '\0';
		int pid = fork();
		if(pid==0){
		//Dans le fils:
			int retour = execlp(stringIn, stringIn,NULL);					// Comme expliqué dans la Q3, le 
			if (retour ==-1){ 
				exit(EXIT_FAILURE);
			}
		}
		else if(pid!=0){
		//Dans le père:
			wait(&status);
			
			if(WIFEXITED(status)){								// WIFEXITED(status) == true si le fils s’est terminé normalement
				write(STDOUT_FILENO, "enseash [exit:", strlen("enseash [exit:")); 
				Etat[0] = (WEXITSTATUS(status))+'0'; 					// WEXITSTATUS(status) renvoie dans ce cas la valeur de sortie du fils
				write(STDOUT_FILENO, Etat, 1); 
				write(STDOUT_FILENO,  "] %", strlen( "] %")); 
			}
				else if(WIFSIGNALED(status)){						// WIFSIGNALED(status) == true si le fils s’est terminé à cause d’un signal
				write(STDOUT_FILENO, "enseash [sign:", strlen("enseash [sign:"));
				Etat[0] = (WTERMSIG(status))+'0';					// WTERMSIG(status) renvoie alors le numéro du signal qui a causé la fin du fils
				write(STDOUT_FILENO, Etat, 1);
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
