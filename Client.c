#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>       // Timeout
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LENGTH_BUFFER 257

void all_mex(int nMex, void *vPtr1);
void check_args(int argc, char **argv);
void check_mex(char *buffer, int *rSptr);
void write_read(int *cMptr, int *rSptr, int simpleSocket);

int main(int argc, char* argv[]) 
{
    struct sockaddr_in simpleServer;
    int simpleSocket, returnStatus, cmex = 0;

    check_args(argc, argv);

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket != -1)
    {
        memset(&simpleServer, '\0', sizeof(simpleServer));
        simpleServer.sin_family = AF_INET;
        simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
        simpleServer.sin_port = htons(atoi(argv[2]));

        returnStatus = connect(simpleSocket, (struct sockaddr*)&simpleServer, sizeof(simpleServer));

        if (returnStatus == 0)
        {
            while(returnStatus != 1)
                write_read(&cmex, &returnStatus, simpleSocket);       // Invia e riceve messaggi dal server

            all_mex(11, &simpleSocket);
        }
        else
            all_mex(5, &simpleSocket);
    }
    else
        all_mex(1, NULL);

    return(EXIT_SUCCESS);
}


void all_mex(int nMex, void *vPtr)      // Contiene tutti i messaggi di errore e di console
{
    if(nMex >= 0 && nMex <= 7)
    {
        switch(nMex)
        { 	
		    case 0:
            { 
			    fprintf(stderr, "\n\nPer avviare il programma sono necessari 3 argomenti: <nome programma> <IPv4 server> <porta server>\n\n<nome programma> equivale a: %s\n<IPv4 server> equivale all'indirizzo IP del programma server a cui ci si vuole collegare (Es. 81.22.36.245)\n<porta server> corrisponde alla porta del server che possiede un valore numerico compreso tra 1024 e 65535\n\nGli argomenti, inoltre, devono essere separati fra di loro con degli spazi.\n\n\n", (char*) vPtr); 
			    break;
            }
		    case 1:
            {
                fprintf(stderr, "\n\nErrore: impossibile creare il socket.\n\n\n"); 
			    break;
            }
            case 2:
            {
                fprintf(stderr, "\n\nErrore: impossibile identificare le varie parti dell'indirizzo ip.\n\n\n");
                break;
            }
            case 3:
            { 
			    fprintf(stderr, "\n\nErrore: i valori di un indirizzo ip devono essere compresi tra 0 e 255.\n\n\n");
                break;
            }
            case 4:
            {
                fprintf(stderr, "\n\nErrore: i valori dell'indirizzo ip devono essere esclusivamente numerici.\n\n\n");
                break;
            }
            case 5:
            {
                fprintf(stderr, "\n\nErrore: impossibile connettersi all'indirizzo IP.\n\n\n");
                close(*((int*) vPtr));
                break;
            } 
		    case 6:
            {
                fprintf(stderr, "\n\nErrore: il valore della porta deve essere esclusivamente numerico.\n\n\n");
                break;
            }
            case 7:
            {
                fprintf(stderr, "\n\nErrore: il numero della porta deve essere compreso tra i valore 1024 e 65535.\n\n\n");
                break;
            }
        }

        exit(EXIT_FAILURE);
    }
    else
    {
        if(nMex >= 8 && nMex <= 10)
        {
            switch(nMex)
            {
                case 8:
                {
                    fprintf(stderr, "\n\nErrore: valore non accettabile della funzione select().\n\n");
                    break;
                }
                case 9:
                {
                    fprintf(stderr, "\n\nErrore: impossibile stabilire una connessione con il server.\n\n");
                    break;
                }
                case 10:
                {
                    fprintf(stderr, "\n\nErrore: il messaggio ricevuto supera il limite massimo di 256 caratteri.\n\n");
                    break;
                }
            }

            *((int*) vPtr) = 1;
        }
        else
        {
            switch(nMex)
            {
                case 11:
                {
                    fprintf(stdout, "\n");
                    close(*((int*) vPtr));
                    break;
                }
                case 12:
                {
                    fprintf(stdout, "\n\nAvviso: ritardo nella risposta dal server.\nAttendi oppure termina il programma.\n\n");
                    break;
                }
                case 13:
                {
                    fprintf(stdout, "\nInserisci qui la stringa numero %d: ", *((int*) vPtr));
                    break;
                }
                case 14:
                {
                    fprintf(stderr, "\n\nErrore: assenza spazio separatore fra ERR e testo, impossibile identificare il messaggio di errore.\n\n");
                    break;
                }
                case 15:
                {
                    fprintf(stderr, "Hai inserito una stringa troppo lunga. Riprova con una stringa di massimo %d caratteri.\n", MAX_LENGTH_BUFFER - 1);
                    break;
                }
                case 16:
                {
                    fprintf(stderr, "\n\nErrore: invio messaggio fallito, impossibile stabilire una connessione con il server.\n\n");
                    *((int*) vPtr) = -1;
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
    int cdot = 0, pnip = 0;
    int long unsigned checkarg;
    char nip[4], *argPtr = argv[0];

    // Controllo numero argomenti
    if(argc == 3)
    { 
        argPtr = argv[1];

        // Controllo indirizzo IP

        for(checkarg = 0; checkarg < strlen(argPtr); checkarg++)     // Controllo numero punti
            if(argPtr[checkarg] == '.')
                cdot++;
    
        if(cdot == 3)
        {
            for(checkarg = 0; checkarg <= strlen(argPtr); checkarg++)
            {
                if(argPtr[checkarg] == '.' || checkarg == strlen(argPtr))
                {
                    if(atoi(nip) >= 0 && atoi(nip) <= 255)       // Controllo range 0-255
                    {
                        memset(&nip, '\0', strlen(nip));
                        pnip = 0;
                    }
                    else
                        all_mex(3, NULL);
                }
                else
                {
                    if(isdigit(argPtr[checkarg]) == 0)      // Controllo caratteri numerici
                        all_mex(4, NULL);
                    else
                    {
                        nip[pnip] = argPtr[checkarg];
                        pnip++;
                    }
                }
            }
        }
        else
            all_mex(2, NULL);

        argPtr = argv[2];

        for(checkarg = 0; checkarg < strlen(argPtr); checkarg++)     // Controllo valori numerici della porta
            if(isdigit(argPtr[checkarg]) == 0)
                all_mex(6, NULL);

        if(atoi(argPtr) < 1024 || atoi(argPtr) > 65535)        // Controllo range della porta
            all_mex(7, NULL);
    }
    else
        all_mex(0, argv[0]);
}


void check_mex(char *buffer, int *rSptr)
{
    int long unsigned posbuffer = 0;

    if(buffer[0] == 'E' && buffer[1] == 'R' && buffer[2] == 'R')        // Conferma protocollo QUIT (QUIT <mex>\n)
    {
        if(buffer[3] == ' ')        // Spazio separatore tra protocollo e messaggio
        {
            posbuffer = 4;
            printf("\n\n");
        }
        else
            all_mex(14, NULL);

        *rSptr = 1;
    }

    while (posbuffer < strlen(buffer))        // Ciclo per stampare i caratteri del messaggio, senza protocollo ERR
    {
        printf("%c", buffer[posbuffer]);
        posbuffer++;
    }

    printf("\n");
}


void write_read(int *cMptr, int *rSptr, int simpleSocket)
{
    fd_set read_fds;
    struct timeval tv;
    int returnStatus, repeat = 1, max_fd = simpleSocket;
    char buffer[MAX_LENGTH_BUFFER];

    if(*cMptr > 0)
    {
        do
        {
            all_mex(13, cMptr);
            fgets(buffer, MAX_LENGTH_BUFFER, stdin);  // Legge la stringa da tastiera

            if(strlen(buffer) == MAX_LENGTH_BUFFER - 1 && buffer[MAX_LENGTH_BUFFER - 2] != '\n') 
                all_mex(15, NULL);
            else
            {
                if(strlen(buffer) == 5 && (buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T' && buffer[4] == '\n'))
                    *rSptr = 1;

                returnStatus = write(simpleSocket, buffer, strlen(buffer));     // Invia il messaggio al server

                if(returnStatus <= 0)      // Controllo la connessione con il server
                    all_mex(16, rSptr);

                repeat = 0;
            }
        }
        while(repeat == 1);
    }

    memset(&buffer, '\0', MAX_LENGTH_BUFFER);

    if(*rSptr != -1)
    {
        FD_ZERO(&read_fds);
        FD_SET(simpleSocket, &read_fds);
        tv.tv_sec = 10;  // Timeout di 10 secondi
        tv.tv_usec = 0;

        returnStatus = select(max_fd + 1, &read_fds, NULL, NULL, &tv);        // Avviso di timeout da sistemare

        if(returnStatus == -1) 
            all_mex(8, rSptr);
        else
        {
            if(returnStatus == 0) 
                all_mex(12, NULL);

            returnStatus = read(simpleSocket, buffer, MAX_LENGTH_BUFFER);      // Legge messaggio dal server

            if (returnStatus > 0)
            {
                if(buffer[strlen(buffer) - 1] == '\n')        // Controllo limite caratteri nel messaggio ricevuto
                {
                    check_mex(buffer, rSptr);       // Identifica il tipo di messaggio e stampa il contenuto
                    (*cMptr)++;
                }
                else
                    all_mex(10, rSptr);                 
            }
            else
                all_mex(9, rSptr);
        }
    }
}
