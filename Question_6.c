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
	int entree, status;
	char *stringIn=malloc(64*sizeof(char));
	char *message=malloc(64*sizeof(char));		
	struct timespec start, stop;		

  const char Delimitation[1]=" ";                           // Permet dans la suite de distinguer la commande de ses arguments.
  char *token=malloc(64*sizeof(char));                      // Permet dans la suite de travailler sur la chaine de caractère de la commande.
	char *ArgList[64];                                        // Contient dans la suite la liste des arguments (et la commande).
  
	write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", strlen("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"));
	
	while(1){
		entree = read(STDIN_FILENO, stringIn, 64);
		stringIn[entree-1] = '\0';
		clock_gettime(CLOCK_REALTIME, &start); 		
		
   		int i =0;
		token=strtok(stringIn,Delimitation);               // "strtok" coupe la chaine de caractère contenue dans "stringIn" jusqu'à "Delimitation", puis la stock
		while (token!=NULL){                               // dans "token". Cette commande modifie la chaine de caractère original "stringIn".
			ArgList[i]=token;                                             
			token=strtok(NULL,Delimitation);
			i++;                                       // A la fin de la boucle, les éléments de "ArgList" sont les différentes chaines de caractères séparées 
		}                                                  // d'un espace de la commande initialement entrée dans le shell.
		ArgList[i] =(char*) NULL;                          // Le reste de la liste est défini comme "NULL".
    
		int pid = fork();
		if(pid==0){
			
		//Dans le fils: On utilise maintenant "execvp(const char *I,char *const J[])", qui execute la commande défini dans I, avec la liste de ses arguments défini dans J.
			int retour = execvp(ArgList[0], ArgList);        //  Le premier élément de "ArgList" contient la commande.
			if (retour ==-1){ 
       				write(STDOUT_FILENO, "ERROR \n", strlen("ERROR \n"));
				exit(EXIT_FAILURE);
			}
		}
		else if(pid!=0){
			
		//Dans le père:
			wait(&status);
			clock_gettime(CLOCK_REALTIME, &stop);				 
			float temps = (stop.tv_nsec - start.tv_nsec)/1000000;		
		
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
    
   		free(token);                                  // On libère ensuite la mémoire allouée à "token" et "stringIn".
		free(stringIn);
	}
	exit(EXIT_SUCCESS); 
}
