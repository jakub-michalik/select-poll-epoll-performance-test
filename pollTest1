#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define SERVER_PORT 9000

int main() {
    int listeningSocket, connfd, i;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1, current_size = 0;

    for(i = 0; i < MAX_CLIENTS; i++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }

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

    fds[0].fd = listeningSocket;
    fds[0].events = POLLIN;

    while(1) {

        int res = poll(fds, nfds, -1);

        if(res > 0) {
            if(fds[0].revents & POLLIN){
                connfd = accept(listeningSocket, NULL, NULL);
                fds[current_size+1].fd = connfd;
                fds[current_size+1].events = POLLIN;
                current_size++;
                nfds++;
            }

            for(i = 1; i <= current_size; i++)
            {
                if(fds[i].revents & POLLIN){
                    memset(&buffer, 0, sizeof(buffer));
                    if (recv(fds[i].fd, buffer, sizeof(buffer), 0) == 0) {
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    } else {
                        printf("Client: %s\n", buffer);
                    }
                }
            }
        }
    }

    return 0;
}
