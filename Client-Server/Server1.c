#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#define Serverport 1450

int main() {

    struct sockaddr_in servizio;

    servizio.sin_family = AF_INET;
    servizio.sin_addr.s_addr = htonl(INADDR_ANY);
    servizio.sin_port = htons(Serverport);

    char stringa[20];
    int socketfd, soa;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketfd, (struct sockaddr *)&servizio, sizeof(servizio));
    listen(socketfd, 10);

    for(;;) {
        printf("Server in ascolto...\n");
        fflush(stdout);

        socklen_t addrlen = sizeof(servizio);
        soa = accept(socketfd, (struct sockaddr *)&servizio, &addrlen);

        read(soa, stringa, sizeof(stringa));
        printf("Ricevuta stringa: %s\n", stringa);

        close(soa);
    }

    return 0;
}
