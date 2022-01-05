# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

void accueil(void){
char* message = "$ ./enseash \nBienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'. \nenseash % \n";
write(STDOUT_FILENO,message,strlen(message));
}

int main(void){
	accueil();
}

//On utilise la fonction write pour afficher dans la console la chaine de caractère message
//STDOUT_FILENO 
