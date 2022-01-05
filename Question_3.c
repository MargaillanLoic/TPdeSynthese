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
	int continuer=1;
	while(continuer){
		write(STDOUT_FILENO, prompt, strlen(prompt));
		read(STDIN_FILENO, stringIn,64);
		if ((strncmp(stringIn, "fortune", strlen("fortune"))==0)){
			pid_t pid=fork();
			if (pid==0){
				execlp("fortune","f",NULL);
			}
			else{
				wait(NULL);
			}	
			
		}else if ((strncmp(stringIn, "exit", strlen("exit"))==0)){
			write(STDOUT_FILENO,"Bye bye...\n$\n", strlen("Bye bye...\n$\n"));
			continuer=0;
		}
	}
}
