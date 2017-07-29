#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

#define PORT 2222 

int Exists(struct sockaddr_in  addr);
 int AjouterClientCo(struct sockaddr_in addr);
int affiche_adresse_socket(int sock);

struct sockaddr_in ADRESSES_CONNECTES[10];
int nb_co =0;

int main()
{
   int descripteurSocket;
   struct sockaddr_in pointDeRencontreLocal;
   struct sockaddr_in pointDeRencontreDistant;
   socklen_t longueurAdresse;
   char messageRecu[255];
   char messageEnvoi[255];
     int lus; 
     int ecrits;
     int retour;
/*Creation socket*/
        descripteurSocket = socket(PF_INET, SOCK_DGRAM, 0);        
           if(descripteurSocket < 0) {
        perror("socket"); 
        exit(-1); 
                                     }
            printf("Socket cree \n");
            printf("Port %d \n",PORT);


/*Bind*/
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&pointDeRencontreLocal, 0x00, longueurAdresse);
     pointDeRencontreLocal.sin_family= PF_INET;
    pointDeRencontreLocal.sin_addr.s_addr = htonl(INADDR_ANY); 
     pointDeRencontreLocal.sin_port= htons(PORT); 
     if((bind(descripteurSocket, 
    (struct sockaddr *)&pointDeRencontreLocal, longueurAdresse))< 0)
                {
                perror("bind");
                exit(-2);
                }
            printf("\n listenning.... \n");

affiche_adresse_socket(descripteurSocket);
 
 while (1){
        longueurAdresse = sizeof(pointDeRencontreDistant);
        memset(&pointDeRencontreDistant, 0x00, longueurAdresse);
        memset(messageRecu, 0x00, 255*sizeof(char));
        lus = recvfrom(descripteurSocket, 
            messageRecu, sizeof(messageRecu), 0,
             (struct sockaddr*)&pointDeRencontreDistant,
              &longueurAdresse);
        
        switch(lus){
                    case -1 : 
                                perror("recvfrom");
                                close(descripteurSocket);
                                exit(-3);
                     case 0 :
            fprintf(stderr, "aucun message n'a ete recu !\n\n");
        close(descripteurSocket);
                                return 0;
                     default: 
     printf("\nclient [ %s via %u ]: %s  \n\n",
        inet_ntoa(pointDeRencontreDistant.sin_addr),
	ntohs(pointDeRencontreDistant.sin_port), messageRecu);
                    }
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
         fprintf(stderr, "aucun message n'a ete envoye !\n\n");
                             close(descripteurSocket);
                                     return 0;
                            default: /* envoi de n octets */
                          if(ecrits != strlen(messageEnvoi))
                  fprintf(stderr, "Erreur d'envoie\n\n");
                                        else
   printf(" \tserveur [ %s via %u ] : %s \n\n",
    inet_ntoa(pointDeRencontreLocal.sin_addr),
	ntohs(pointDeRencontreLocal.sin_port), messageEnvoi);
                            }
}
                close(descripteurSocket);

            return 0;
}


int Exists(struct sockaddr_in  addr){
   int ite;
   int R = 0;
  for( ite=0;ite<10;ite++){
    if(ADRESSES_CONNECTES[ite].sin_addr.s_addr==addr.sin_addr.s_addr)
     R=1;
  }
 return R;
}

 int AjouterClientCo(struct sockaddr_in addr){
 if(Exists(addr)!=0) {
                        if(nb_co >= 10) {
                            printf("Client rejeté");
                                        return 0; 
                                        }
                                        else{
                             ADRESSES_CONNECTES[nb_co+1] = addr;
                                    nb_co=nb_co +1;
                                        return 1;
                                        }
                     }else  return 2;             
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
