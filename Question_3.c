// Exit : Ajout d'une fonctionnalité pour sorti du shell

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void main(){
	char* message = "$ ./enseash \nBienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'. \n";
	write(STDOUT_FILENO,message,strlen(message));
	
	char *prompt="enseash % ";
	char *stringIn=malloc(64*sizeof(char));
	int continuer=1;					// variable qui controle la boucle
	while(continuer){
		write(STDOUT_FILENO, prompt, strlen(prompt));
		read(STDIN_FILENO, stringIn,64);
		if ((strncmp(stringIn, "fortune", strlen("fortune"))==0)){				// Attention, en procédant comme ceci, seule la commande fortune peut être 
			pid_t pid=fork();								// executée. On change la manière de procéder par la suite pour executer 
			if (pid==0){									// toutes les commandes
				execlp("fortune","f",NULL);
			}
			else{
				wait(NULL);
			}	
			
		}else if ((strncmp(stringIn, "exit", strlen("exit"))==0)){				// on ajoute les 3 lignes qui suivent et on modifie la condition du while
			write(STDOUT_FILENO,"Bye bye...\n$\n", strlen("Bye bye...\n$\n"));		// ce qui permet de sortir de la boucle si on écrit "exit"
			continuer=0;
		}
	}
}
