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

    int disponibilita = 10;   // taxi disponibili

    int socketfd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[200];

    // 1. Creazione socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("Errore creazione socket");
        exit(1);
    }

    // 2. Settaggio parametri del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    if (bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore bind");
        exit(1);
    }

    // 4. Listen
    if (listen(socketfd, 5) < 0) {
        perror("Errore listen");
        exit(1);
    }

    printf("SERVER TAXI in ascolto sulla porta %d...\n", PORT);

    while (1) {

        printf("\nIn attesa di un client...\n");

        // 5. Accept
        new_socket = accept(socketfd, (struct sockaddr*)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("Errore accept");
            continue;
        }

        printf("Connessione ricevuta da %s\n", inet_ntoa(client_addr.sin_addr));

        // 6. Ricezione richiesta
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer));
        printf("Richiesta ricevuta: %s\n", buffer);

        // 7. Controllo disponibilità
        char risposta[100];
        if (disponibilita > 0) {
            sprintf(risposta, "Taxi disponibile. Posti rimasti: %d", disponibilita - 1);
            disponibilita--;
        } else {
            sprintf(risposta, "Nessun taxi disponibile al momento.");
        }

        // 8. Invio risposta
        write(new_socket, risposta, strlen(risposta));

        close(new_socket);
    }

    return 0;
}
