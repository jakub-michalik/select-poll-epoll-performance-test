#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define SERVER_PORT 9000

int main() {
    int listeningSocket, connfd, maxfd, nready, client[MAX_CLIENTS], i, max_clients = MAX_CLIENTS, sd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    fd_set readfds;

    // Initialize all the client_socket[] to 0
    for (i = 0; i < max_clients; i++) {
        client[i] = 0;
    }

    // Create a master socket
    if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(listeningSocket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("binding failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d \n", SERVER_PORT);

    if (listen(listeningSocket, 3) < 0) {
        perror("listening failed");
        exit(EXIT_FAILURE);
    }

    while (1) {

        FD_ZERO(&readfds);
        FD_SET(listeningSocket, &readfds);
        maxfd = listeningSocket;

        for (i = 0; i < max_clients ; i++) {
            if (client[i] > 0)
              FD_SET(client[i], &readfds);
            if (client[i] > maxfd)
              maxfd = client[i];
        }

        nready = select(maxfd+1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(listeningSocket, &readfds)) {
            if ((connfd = accept(listeningSocket, NULL, NULL)) < 0){
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < max_clients; i++) {
                if (client[i] == 0) {
                    client[i] = connfd;
                    break;
                }
            }
        }

        // If something happened on other sockets
        for (i = 0; i < max_clients; i++) {
            sd = client[i];
            if (FD_ISSET(sd, &readfds)) {
                memset(&buffer, 0, sizeof(buffer));
                if (recv(sd, buffer, sizeof(buffer), 0) == 0) {
                    close(sd);
                    client[i] = 0;
                } else {
                    printf("Client: %s\n", buffer);
                }
            }
        }
    }

    return 0;
}
