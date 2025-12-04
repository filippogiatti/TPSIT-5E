#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1450

int main() {

    int socketfd;
    struct sockaddr_in server_addr;
    char partenza[50], arrivo[50], richiesta[200], risposta[200];

    // 1. Creazione socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("Errore creazione socket");
        exit(1);
    }

    // 2. Inserimento IP del server da tastiera
    char ipserver[20];
    printf("Inserisci IP del server: ");
    scanf("%s", ipserver);

    // 3. Parametri server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ipserver, &server_addr.sin_addr);

    // 4. Connessione
    if (connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore connessione");
        exit(1);
    }

    // 5. Input dati utente
    printf("Città di partenza: ");
    scanf("%s", partenza);

    printf("Città di arrivo: ");
    scanf("%s", arrivo);

    sprintf(richiesta, "Partenza: %s - Arrivo: %s", partenza, arrivo);

    // 6. Invio richiesta
    write(socketfd, richiesta, strlen(richiesta));

    // 7. Ricezione risposta
    memset(risposta, 0, sizeof(risposta));
    read(socketfd, risposta, sizeof(risposta));

    printf("Risposta server: %s\n", risposta);

    close(socketfd);
    return 0;
}
