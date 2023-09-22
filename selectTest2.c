#include <stdio.h>
<<<<<<< HEAD
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define MAXBUF 256
#define NUM_OF_CLIENTS 1000

void child_process(void) {
  sleep(2);
  char msg[MAXBUF];
  struct sockaddr_in addr = {
    0
  };
  int n, sockfd, num = 1;
  srandom(getpid());
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sockfd, (struct sockaddr * ) & addr, sizeof(addr));

  printf("child {%d} connected \n", getpid());
  while (1) {
    int sl = (random() % 10) + 1;
    num++;
    sleep(sl);
    sprintf(msg, "Test message %d from client %d", num, getpid());
    n = write(sockfd, msg, strlen(msg)); /* Send message */
  }

}

int main() {
  char buffer[MAXBUF];
  int fds[NUM_OF_CLIENTS];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, n, i, max = 0;;
  int sockfd, commfd;
  fd_set rset;
  for (i = 0; i < NUM_OF_CLIENTS; i++) {
    if (fork() == 0) {
      child_process();
      exit(0);
    }
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset( & addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd, (struct sockaddr * ) & addr, sizeof(addr));
  listen(sockfd, NUM_OF_CLIENTS);

  for (i = 0; i < NUM_OF_CLIENTS; i++) {
    memset( & client, 0, sizeof(client));
    addrlen = sizeof(client);
    fds[i] = accept(sockfd, (struct sockaddr * ) & client, & addrlen);
    if (fds[i] > max)
      max = fds[i];
  }

  while (1) {
    FD_ZERO( & rset);
    for (i = 0; i < NUM_OF_CLIENTS; i++) {
      FD_SET(fds[i], & rset);
    }

    puts("round again");
    select(max + 1, & rset, NULL, NULL, NULL);

    for (i = 0; i < NUM_OF_CLIENTS; i++) {
      if (FD_ISSET(fds[i], & rset)) {
        memset(buffer, 0, MAXBUF);
        read(fds[i], buffer, MAXBUF);
        puts(buffer);
      }
    }
  }
  return 0;
}
=======
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
>>>>>>> 7cd77e2 (Improvement in the select tests. Adding Dynamic)
