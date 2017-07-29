#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> 


#define PORT 2222
int affiche_adresse_socket(int sock);

int main()
    {
    int descripteurSocket;
    struct sockaddr_in pointDeRencontreLocal;
    struct sockaddr_in pointDeRencontreDistant;
    socklen_t longueurAdresse;
    char messageRecu[255];
    char messageEnvoi[255]; 
    int ecrits;
    int lus; 
    int retour;
    /*Creer socket*/
        descripteurSocket = socket(PF_INET, SOCK_DGRAM, 0); 
            
                    if(descripteurSocket < 0) 
                                            {
                                                perror("socket"); 
                                                exit(-1); 
                                             }
         printf("Socket cree\n");
            printf("port %d\n", PORT);
    /*Binding*/ 
         
         longueurAdresse = sizeof(struct sockaddr_in);
         memset(&pointDeRencontreLocal, 0x00, longueurAdresse);
         pointDeRencontreLocal.sin_family= PF_INET;
       pointDeRencontreLocal.sin_addr.s_addr = htonl(INADDR_ANY);
       
        
         pointDeRencontreLocal.sin_port= htons(0); 
        
        if((bind(descripteurSocket, 
            (struct sockaddr *)&pointDeRencontreLocal, 
            longueurAdresse))< 0)
            {
                perror("bind");
                exit(-2);
            }
            printf("Socket Bindé avec succès !\n");

affiche_adresse_socket(descripteurSocket);
   longueurAdresse = sizeof(pointDeRencontreDistant);
            memset(&pointDeRencontreDistant, 0x00, longueurAdresse);
             
             pointDeRencontreDistant.sin_family = PF_INET;
            pointDeRencontreDistant.sin_port = htons(PORT);

            inet_aton("127.0.0.1", &pointDeRencontreDistant.sin_addr);
    while(1) {
            memset(messageEnvoi, 0x00, 255*sizeof(char));
            if (fgets(messageEnvoi, 255, stdin) == NULL)
            break;
            messageEnvoi[strlen(messageEnvoi)-1] = '\0';
            ecrits = sendto(descripteurSocket, messageEnvoi,
             strlen(messageEnvoi), 0,
              (struct sockaddr *)&pointDeRencontreDistant,
               longueurAdresse);
            switch(ecrits)
                            {
                            case -1 : 
                                     perror("sendto");
                                     close(descripteurSocket);
                                     exit(-3);
                            case 0 :
  fprintf(stderr, "aucun message envoye !\n\n");
                 close(descripteurSocket);
                                     return 0;
                            default: 
                if(ecrits != strlen(messageEnvoi))
            fprintf(stderr, "Erreur d'envoie\n\n");
                                        else
                    printf(" \t client : %s \n\n", messageEnvoi);
                            }
        memset(&pointDeRencontreDistant, 0x00, longueurAdresse);
        memset(messageRecu, 0x00, 255*sizeof(char));
        lus = recvfrom(descripteurSocket, messageRecu, 
            sizeof(messageRecu), 0, 
            (struct sockaddr*)&pointDeRencontreDistant,
             &longueurAdresse);
        switch(lus){
                    case -1 : 
                                perror("recvfrom");
                                close(descripteurSocket);
                                exit(-3);
                     case 0 :
                         fprintf(stderr, "aucun message recu !\n\n");
                                close(descripteurSocket);
                                return 0;
                     default: 
         printf("\t serveur: %s  \n\n", messageRecu);
                    }
     }
        close(descripteurSocket);

              return 0;
}

int affiche_adresse_socket(int sock)
{
    struct sockaddr_in adresse;
    int longueur;
    longueur = sizeof(struct sockaddr_in);
if (getsockname(sock, (struct sockaddr*)&adresse, &longueur) < 0){
                fprintf(stderr, "Erreur getsockname\n");
                return -1;}
     
   printf("IP = %s, Port = %u\n", inet_ntoa(adresse.sin_addr),
    ntohs(adresse.sin_port));
    
    return 0;
}
