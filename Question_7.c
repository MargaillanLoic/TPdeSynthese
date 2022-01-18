#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>			
#include <fcntl.h>

int main(void){
	  int entree, status;
	  char *stringIn=malloc(64*sizeof(char));
  	char *message=malloc(64*sizeof(char));		
  	struct timespec start, stop;		

    const char Delimitation[1]=" ";                         
    char *token=malloc(64*sizeof(char));                  
	  char *ArgList[64];    
    int redirect = 0;               // variable qui indique si il y a une redirection ou non.
   	int retour;                     // variable précedement défini dans une boucle, transformée en variable globale (pas important).
  	FILE *fichier;                  // en cas de redirection.
  
  	write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", strlen("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"));
	
  	while(1){
	    	entree = read(STDIN_FILENO, stringIn, 64);
	    	stringIn[entree-1] = '\0';
	    	clock_gettime(CLOCK_REALTIME, &start); 		
		
      	int i =0;
	    	token=strtok(stringIn,Delimitation);              
	    	while (token!=NULL){  
           if (strcmp(token,"<") == 0) {               // L'apparition d'un de ces deux caractères ("<" ou ">") indique qu'il y a une redirection :
		      		redirect = 1;                             // on définit la variable comme True.
		       }
		  	   else if (strcmp(token,">") == 0) {
	      			redirect = 1;
	     		 }
			     ArgList[i]=token;                                             
			     token=strtok(NULL,Delimitation);
			     i++;                                      
		    }                                            
		    ArgList[i] =(char*) NULL;                   
    
		    int pid = fork();
		    if(pid==0){
			
		    //Dans le fils: 
          if (!redirect){
		    		retour = execvp(ArgList[0], ArgList);               // S'il n'y a pas de redirection, on execute la commande comme avant.
		    	}
          else if (redirect) {
		      		char *filename = ArgList[i-1];                    // Le nom du fichier sur lequel on souhaite travailler est le dernier argument de la chaine de caractère.
		      		fichier = fopen(filename,w+);                     // Ouverture du fichier en mode lecture/écriture.
			        //....
              //....                                            // Il faut alors écrire ici le code permettant d'écrire dans le fichier
              //....                                            // le flux de sortie "stdout", en prenant en compte la commande d'entrée 
              //....                                            // initiale.
          }
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
    
    		free(token);                                
     		free(stringIn);
  	}
	  exit(EXIT_SUCCESS); 
}
