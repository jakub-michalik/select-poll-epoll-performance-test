#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_EVENTS 5
#define BUFFER_SIZE 1024
#define SERVER_PORT 9000

int main() {
    int listeningSocket, connfd, epollfd, nfds, i;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    struct epoll_event ev, events[MAX_EVENTS];

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

    epollfd = epoll_create1(0);
    if(epollfd == -1){
        perror("epoll_create failed");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listeningSocket;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listeningSocket, &ev) == -1) {
        perror("epoll_ctl: listeningSocket");
        exit(EXIT_FAILURE);
    }

    for(;;) {

        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(nfds == -1){
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
        }

        for(i = 0; i < nfds; i++) {
            if(events[i].data.fd == listeningSocket){
                connfd = accept(listeningSocket, (struct sockaddr *)NULL ,NULL);

                if(connfd == -1){
                    perror("accept");
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
                    perror("epoll_ctl: connfd");
                    exit(EXIT_FAILURE);
                }
            } else {
                memset(buffer, 0, sizeof(buffer));
                if(read(events[i].data.fd, buffer, sizeof(buffer)) <= 0){
                    close(events[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                } else {
                    printf("Client: %s\n", buffer);
                }
            }
        }
    }

    return 0;
}
