#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h>


int main()
{
	//Ecriture dans la memoire paratagée
	int i=0;
		//ftok permet de generer la clé grace a/*==ftok()==*/
	key_t key = ftok("shareMemori", 65);
	
		//Recuperer un id pour la mémoire partagée grace a /*==shmget==*/
	int shareID = shmget(key, 1024, 0666|IPC_CREAT);
	
		//Attcher une variable sur cette memoire pour écrire dedans grace a /*== ==*/
	char* writter = (char*) shmat(shareID, (void*)0, 0);
	
		writter
		//Detacher writter de cette memoire grace a /*==shmdt==*/
	shmdt(writter);
		//Supprimer la mémoire partagée 
	shmctl(shareID, IPC_RMID, NULL);
	return 0;
}
