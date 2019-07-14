/*
*
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MY_ADDR "127.0.0.1"
#define MY_PORT 56789
#define LISTEN_BACKLOG 50

int main(int argc, char *argv[])
{
	 int sfd; // socket du serveur
   int cfd; // socket du client
   struct sockaddr_in my_addr; // socket addr du serveur 
   struct sockaddr_in peer_addr; // socket addr d'un client

   socklen_t peer_addr_size; // la taille du sock
   pid_t child, pid1, pid2;
   
   char buffer[10];
   sfd = socket(AF_INET, SOCK_STREAM, 0); // création de la socket
    /*memoire partagée */
        // ftok to generate unique key 
        key_t key = ftok("shmfile",65); 
        int i, status;
	      
        // shmget returns an identifier in shmid 
        int shmid = shmget(key,1024,0666|IPC_CREAT); 
        // shmat to attach to shared memory 
        char *str = (char*) shmat(shmid,(void*)0,0); 
        sprintf(str, "%d", 5); // initialisation de la memoire partagée a 5
       /*memoire partagée */  
       
   if (sfd < 0)
   { 
      // Une erreur s'est produite la socket n'a pas pu être créer
        printf("Le SE n'a pas pu créer la socket %d\n", sfd);
        exit(-1);
   }
    memset(&my_addr, 0, sizeof(struct sockaddr_in));
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons (MY_PORT);
    inet_aton(MY_ADDR, (struct in_addr *)&my_addr.sin_addr.s_addr);
    
     if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) < 0)
      {
         printf("bind error\n"); // l'association a echouée
         exit(-1);
      }
      if (listen(sfd, LISTEN_BACKLOG) < -1)
               perror("listen\n");
      // on se bloque en attendant les connexion des client
      peer_addr_size = sizeof(struct sockaddr_in);
      while(1)
      {
          cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);  
          if (cfd < 0)
          {
              perror("accept\n");
              exit(-1); // Une erreur s'est produite
          }
          /* Nous créons un fils pour gérer ce client */
          child = fork();
          if(child < 0)
          { 
            // le fils n'a pas pu être créé
            perror("errreur de création du fils\n");
          }
          if(child==0)
          {
             /* Nous sommes dans le fils nous attendons la requête du client */
             printf("indentité du client %d\n", peer_addr.sin_port);
              while(read(cfd, buffer, 10))
                 printf("%s \n", buffer);
                	
                pid1 = fork();
                 if(pid1 < 0)
                  { 
                    // le fils n'a pas pu être créé
                    perror("errreur de création du fils\n");
                  }
                  if(pid1 == 0)
                  { 
                    printf("Je suis le fils 1 \n");
                    sprintf(str, "%d", atoi(str)+1);
                    printf("Fils 1 val a : %s \n", str);
                  }
                  else
                  { 
                    sleep(5);
                     pid2 = fork();
                   if(pid2 < 0)
                    { 
                      // le fils n'a pas pu être créé
                      perror("errreur de création du fils\n");
                    }
                    if(pid2 == 0)
                    { 
                      printf("Je suis le fils 2 \n");
                  
                      sprintf(str, "%d", atoi(str)+2);
                      printf("Fils 2 val a : %s \n", str);
                    }
                    else
                     { 
                      printf("Je suis le pere \n");
                      pid1 = wait(&status);
                      printf("Fils %d status : %d \n", pid1, status);
                      pid2 = wait(&status);
                      printf("FIls %d status : %d \n", pid2, status);
                      printf("Val final : %s \n", str);
                    }
                     
                }
                

          }
          
          else
          {
              //detach from shared memory 
	                    shmdt(str); 
	                    // destroy the shared memory 
	                    shmctl(shmid,IPC_RMID,NULL);
            /*Dans le père: le père attent un autre client*/
            close(cfd);
          }
      }

}






















