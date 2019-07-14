

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
    int i, j;
    //boucle pour créer 3 processus fils
    pid_t pid1 = fork();             
    if (pid1 == 0) {                 
        printf("Processus numero 1, pid = : (%d)\n", getpid());
        return 0;                    
    }                                
                                     
    pid_t pid2 = fork();             
    if (pid2 == 0) {                 
        printf("Processus numero 2, pid = : (%d)\n", getpid());
        return 0;                    
    }   
       

    pid_t pid3 = fork();             
    if (pid3 == 0) {                 
        printf("Processus numero 3, pid = : (%d)\n", getpid());
        return 0;                    
    } 

    return 0;
}
