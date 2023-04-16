#include <ctype.h>
#include <locale.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_LENGTH_BUFFER 257

//Per ora faccio un echo server infinito, che restituisce il messaggio ricevuto da un client fino a quando non riceve QUIT\n
//I messaggi sono al massimo di 256 caratteri
//Mando ERR come protocollo per arrestare il client in caso di errore

void all_mex(int nMex, void *vPtr1, void *vPtr2, void *vPtr3);
void check_args(int argc, char **argv);
void print_localtime();
void read_write(int *cMptr, int *rSptr, int simpleChildSocket);

int main(int argc, char* argv[])
{
    struct sockaddr_in simpleServer;
    struct sockaddr_in clientName = {0};
    unsigned int clientNameLength;
    int simpleSocket, simpleChildSocket, simplePort, returnStatus, cMex;

    check_args(argc, argv);

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(simpleSocket != -1) 
    {
        simplePort = atoi(argv[1]);

        memset(&simpleServer, '\0', sizeof(simpleServer));
        simpleServer.sin_family = AF_INET;
        simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
        simpleServer.sin_port = htons(simplePort);

        returnStatus = bind(simpleSocket, (struct sockaddr*)&simpleServer, sizeof(simpleServer));

        if(returnStatus == 0)
        {
            returnStatus = listen(simpleSocket, 5);

            if(returnStatus != -1)
            {
                print_localtime();     // Stampa a terminale l'ora locale del server quando esso viene eseguito.

                while(1)
                {
                    clientNameLength = sizeof(clientName);
                    all_mex(8, NULL, NULL, NULL);

                    simpleChildSocket = accept(simpleSocket, (struct sockaddr*)&clientName, &clientNameLength);

                    if(simpleChildSocket != -1)
                    {
                        all_mex(9, NULL, NULL, NULL);

                        cMex = 0;
                        returnStatus = 0;

                        while(returnStatus == 0)
                            read_write(&cMex, &returnStatus, simpleChildSocket);
                        
                        all_mex(13, &simpleChildSocket, NULL, NULL);
                    }
                    else
                        all_mex(6, &simpleSocket, NULL, NULL);
                }

                all_mex(7, &simpleSocket, NULL, NULL);
            }
            else
                all_mex(5, &simpleSocket, NULL, NULL);
        }
        else
            all_mex(4, &simpleSocket, NULL, NULL);
    }
    else
        all_mex(3, NULL, NULL, NULL);    

    return(EXIT_SUCCESS);
}


void all_mex(int nMex, void *vPtr1, void *vPtr2, void *vPtr3)      // Contiene tutti i messaggi di errore e di console
{
    if(nMex >= 0 && nMex <= 7)
    {
        if(nMex >= 0 && nMex <= 3)
        {
            switch(nMex)       // Exit
            { 	
		        case 0:
                { 
			        fprintf(stderr, "\n\nPer avviare il programma sono necessari 2 argomenti: <nome programma> <porta>\n\n<nome programma> equivale a: %s\n<porta> corrisponde ad un valore numerico compreso tra 1024 e 65535\n<numero tentativo>\n\nGli argomenti, inoltre, devono essere separati fra di loro con degli spazi.\n\n\n", (char*) vPtr1); 
			        break;
                }
		        case 1:
                {
                    fprintf(stderr, "\n\nErrore: il valore della porta deve essere esclusivamente numerico.\n\n\n");
                    break;
                }
                case 2:
                {
                    fprintf(stderr, "\n\nErrore: il numero della porta deve essere compreso tra i valore 1024 e 65535.\n\n\n");
                    break;
                }
                case 3:
                { 
			        fprintf(stderr, "\n\nErrore: impossibile creare il socket.\n\n\n"); 
			        break;
                }
            }
	    }
        else
        {
            switch(nMex)       // Close(socket) + exit
            { 
                case 4:
                {
                    fprintf(stderr, "\n\nErrore: impossibile eseguire il bind.\n\n\n");
                    break;
                } 
		        case 5:
                {
                    fprintf(stderr, "\n\nErrore: impossibile ascoltare le richieste di connessione.\n\n\n");
                    break;
                } 
		        case 6:
                {
                    fprintf(stderr, "\n\nErrore: impossibile accettare le connesioni\n\n\n");
                    break;
                }
                case 7:
                {
                    fprintf(stderr, "\n\nErrore sconosciuto, ciclo infinito terminato\n\n\n");
                    break;
                }

                close(*((int*) vPtr1));
            }
        }

        exit(EXIT_FAILURE);
    }
    else
    {
        if(nMex >= 8 && nMex <= 13)
        {
            switch(nMex)       // Messaggi a terminale
            {
                case 8:
                {
                    fprintf(stdout, "\n\n\nIn attesa di connessione...\n\n");
                    break;
                }
                case 9:
                {
                    fprintf(stdout, "\n\nConnessione effetuata.\n\n");
                    break;
                }
                case 10:
                {
                    fprintf(stdout, "\n\nIn attesa di risposta...\n\n");
                    break;
                }
                case 11:
                {
                    fprintf(stdout, "\nMessaggio (QUIT) corretto.\n");
                    break;
                }
                case 12:
                {
                    fprintf(stdout, "\nMessaggio inviato.\n");
                    break;
                }
                case 13:
                {
                    close(*((int*) vPtr1));
                    fprintf(stdout, "\n\nConnessione terminata.\n");
                    break;
                } 
            }
        }
        else
        {
            switch(nMex)       // Errori + stampa a terminale contenuto buffer e numero caratteri
            {
                case 14:
                {
                    fprintf(stderr, "\nErrore: impossibile stabilire una connessione con il client.\n");
                    *((int*) vPtr1) = -1;
                    break;
                }
                case 15:
                {
                    fprintf(stderr, "\nErrore: carattere '\\n' assente\n");
                    *((int*) vPtr1) = 1;
                    break;
                }
                case 16:
                {
                    fprintf(stderr, "\nErrore: invio messaggio fallito, impossibile stabilire una connessione con il client.\n");
                    *((int*) vPtr1) = 1;
                    break;
                }
                case 17:
                {
                    fprintf(stdout, "\nMessaggio di benvenuto: %sNumero caratteri: %lu + '\\n'.\n", (char*) vPtr1, strlen((char*) vPtr1) - 1);
                    break;
                }
                case 18:
                {
                    fprintf(stdout, "\nMessaggio (%d) ricevuto.\nContenuto: %s", *((int*) vPtr1), (char*) vPtr2);
                    break;
                }
                case 19:
                {
                    fprintf(stdout, "Numero caratteri: %lu + '\\n'.\n", strlen((char*) vPtr1) - 1);
                    *((int*) vPtr2) = 0;
                    break;
                }
                case 20:
                {
                    fprintf(stdout, "\nMessaggio finale: Fine comunicazione, messaggi totali inviati: %d\nNumero caratteri: %lu + '\\n'.\n", *((int*) vPtr1), strlen((char*) vPtr2) - 1);
                    *((int*) vPtr3) = 1;
                    break;
                }
                default:   
                {
			        fprintf(stderr, "\n\nErrore: Eccezione non prevista\n\n");
                    exit(EXIT_FAILURE); 
                }
            }
        }
    }
}


void check_args(int argc, char **argv)      // Funzione per controllare gli argomenti in input
{
    char* port_str = argv[1];

    if(argc != 2)      // Controllo numero argomenti
        all_mex(0, argv[0], NULL, NULL);

    for(int long unsigned checkarg = 0; checkarg < strlen(port_str); checkarg++)     // Controllo valori numerici della porta
        if(isdigit(port_str[checkarg]) == 0)
            all_mex(1, NULL, NULL, NULL);

    if(atoi(port_str) < 1024 || atoi(port_str) > 65535)        // Controllo range della porta (1024-65535)
        all_mex(2, NULL, NULL, NULL);
}


void print_localtime()   // Semplice funzione che stampa a schermo il tempo locale
{
    time_t ticks = time(NULL);
    struct tm *tm = localtime(&ticks);
    char date[MAX_LENGTH_BUFFER];

    setlocale(LC_TIME, "it_IT.utf8");   // Imposta l'ora locale in italiano
    strftime(date, MAX_LENGTH_BUFFER, "%A %d %B %Y, ore %H:%M:%S", tm);  // Salva la data nella stringa date
    fprintf(stdout, "\n\nServer inizializzato con successo in data: %s.\n", date);
}


void read_write(int *cMptr, int *rSptr, int simpleChildSocket)      //Funzione per inviare messaggi di benvenuto e per ricevere ed elaborare i messaggi dei client
{
    char buffer[MAX_LENGTH_BUFFER];
    memset(&buffer, '\0', MAX_LENGTH_BUFFER);

    if(*cMptr == 0)
    {
        sprintf(buffer, "\nConnessione stabilita. Benvenuto/a! Per ora questo programma fa da echo.\nSe vuoi terminare il programma premi CTRL+C oppure scrivi QUIT + invio.\n");
        all_mex(17, buffer, NULL, NULL);
    }
    else
    {
        all_mex(10, NULL, NULL, NULL);

        *rSptr = read(simpleChildSocket, buffer, MAX_LENGTH_BUFFER);

        if(*rSptr > 0)
        {
            all_mex(18, cMptr, buffer, NULL);

            if(buffer[strlen(buffer) - 1] == '\n')
            {
                all_mex(19, buffer, rSptr, NULL);
                
                // Qui al posto di QUIT dovrei inserire una funzione per modificare il buffer a seconda del suo formato

                if(strlen(buffer) == 5 && (buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T'))
                {
                    memset(&buffer, '\0', MAX_LENGTH_BUFFER);

                    all_mex(11, NULL, NULL, NULL);
                    sprintf(buffer, "\n\nFine comunicazione, messaggi totali inviati: %d\n", *cMptr);

                    all_mex(20, cMptr, buffer, rSptr);
                }
            }
            else
            {
                all_mex(15, rSptr, NULL, NULL);
                sprintf(buffer, "ERR Carattere d'invio finale non presente, possibilmente a causa di assenza o per overflow di caratteri.\n");
            }
        }
        else
            all_mex(14, rSptr, NULL, NULL);
    }    

    if(*rSptr != -1)
    {
        int check = write(simpleChildSocket, buffer, strlen(buffer));
                                
        if (check == -1)
            all_mex(16, rSptr, NULL, NULL);
        else
            all_mex(12, NULL, NULL, NULL);
    }
    else
        *rSptr = 1;

    if(*rSptr != 1)
        (*cMptr)++;
}
