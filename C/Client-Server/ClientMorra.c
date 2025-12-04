#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

void stampa_mossa(int m) {
    if (m == 0) printf("Sasso");
    else if (m == 1) printf("Carta");
    else if (m == 2) printf("Forbice");
}

int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;

    // Creazione socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore creazione socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Inserimento IP server
    char ip[20];
    printf("Inserisci IP del server: ");
    scanf("%s", ip);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("Errore indirizzo IP");
        exit(1);
    }

    // Connessione
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Errore connessione");
        exit(1);
    }

    printf("Connesso al server. Iniziamo a giocare!\n");
    printf("Digitare: 0 = Sasso | 1 = Carta | 2 = Forbice | 9 = Esci\n\n");

    int scelta_client, scelta_server;
    int risultato, punti_server, punti_client;

    while (1) {

        // Scelta del client
        printf("La tua scelta: ");
        scanf("%d", &scelta_client);

        if (scelta_client == 9) {
            write(sock, &scelta_client, sizeof(int));
            printf("Hai deciso di terminare la partita.\n");
            break;
        }

        if (scelta_client < 0 || scelta_client > 2) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }

        // Invio mossa al server
        write(sock, &scelta_client, sizeof(int));

        // Ricezione mossa del server
        if (read(sock, &scelta_server, sizeof(int)) <= 0) break;

        printf("Il server ha scelto: ");
        stampa_mossa(scelta_server);
        printf("\n");

        // Ricezione risultato
        read(sock, &risultato, sizeof(int));
        read(sock, &punti_server, sizeof(int));
        read(sock, &punti_client, sizeof(int));

        // Interpretazione risultato
        if (risultato == 0) {
            printf("Risultato: PAREGGIO!\n");
        } else if (risultato == 1) {
            printf("Risultato: Vince il SERVER!\n");
        } else {
            printf("Risultato: VINCI TU!\n");
        }

        // Mostro punteggio
        printf("Punteggio → Server: %d | Tu: %d\n", punti_server, punti_client);
        printf("-------------------------------------------\n");
    }

    close(sock);
    return 0;
}
