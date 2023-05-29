# UPO-Reti_1
Il progetto di quest'anno è un semplice calcolatore di media e varianza campionaria. Il client invia tramite uno o più messaggi dei numeri interi positivi su cui eseguire le operazioni, e quando il server gli riceve calcola e memorizza la media e la varianza dei campioni. Una volta che il client inserisce il carattere '0', il server invia la media e la varianza che ha calcolato e chiude la connessione.

I programmi, sviluppati in linguaggio C, sono stati compilati e testati in un ambiente linux (Ubuntu 23.04).
La compilazione è stata effettuata da terminale con il comando "gcc file.c -Wall -Wextra -std=c11 -pedantic -g -o file", e non sono stati generati ne errori o avvisi.

Nell'implementazione del mio server ho aggiunto alcune operazioni non strettamente necessarie, come l'orario locale (stampato a terminale quando si avvia il server, e inviato come messaggio di benvenuto al client), le operazioni del server stampate a terminale, e alcuni controlli di overflow/underflow nel caso l'utente inserisca numeri troppo grandi.

Una volta che avrò superato l'esame pubblicherò il codice dei programmi.
Nel frattempo, nei commit precedenti, è possibile trovare il progetto dell'anno scorso.
