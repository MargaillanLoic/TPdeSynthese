
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



int main(void)
{
	int nbCharCommande, status;
	char *stringIn=malloc(64*sizeof(char));
	char Etat[1];
	char message[64]={0};
	struct timespec tempsD, tempsF;
	write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", strlen("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"));
	
	while(1){
		write(STDOUT_FILENO, "enseash % ", strlen("enseash % "));
		nbCharCommande = read(STDIN_FILENO, stringIn, 64);
	
		
		stringIn[nbCharCommande-1] = '\0';
		clock_gettime(CLOCK_REALTIME, &tempsD);
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
			clock_gettime(CLOCK_REALTIME, &tempsF);
			float temps = (tempsF.tv_nsec - tempsD.tv_nsec)/1000000;
		
			if(WIFEXITED(status)){
				write(STDOUT_FILENO, "enseash [exit:", strlen("enseash [exit:")); 
				Etat[0] = (WEXITSTATUS(status))+'0'; 
				sprintf(message, "%d|%.2f ms",WEXITSTATUS(status), temps);
				write(STDOUT_FILENO,message,strlen(message));
				write(STDOUT_FILENO, Etat, 1); 
				write(STDOUT_FILENO,  "] %", strlen( "] %")); 

			}
				else if(WIFSIGNALED(status)){
				write(STDOUT_FILENO, "enseash [sign:", strlen("enseash [sign:"));
				Etat[0] = (WTERMSIG(status))+'0';
				sprintf(message, "%d|%.2f ms",WEXITSTATUS(status), temps);
				write(STDOUT_FILENO,message,strlen(message));
				write(STDOUT_FILENO, Etat, 1);
				write(STDOUT_FILENO,  "] %", strlen( "] %"));

			}
		}
		
		if(!strncmp("exit",stringIn,4) || (nbCharCommande == 0)){ 
			write(STDOUT_FILENO, "Bye bye ...\n", strlen("Bye bye ...\n"));
			exit(EXIT_SUCCESS); 
		}
	}
	exit(EXIT_SUCCESS); 
}
