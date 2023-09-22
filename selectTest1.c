#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>

#define MAXBUF 256
#define NUM_OF_CLIENTS 1000

// This function simulates client behavior.
void child_process(void) {
  sleep(2);
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd, num = 1;
  srandom(getpid());

  // Create client socket and connect to server
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  connect(sockfd, (struct sockaddr * ) & addr, sizeof(addr));

  printf("child {%d} connected \n", getpid());

  // Continue sending messages at random intervals
  while (1) {
    int sl = (random() % 10) + 1;
    num++;
    sleep(sl);
    sprintf(msg, "Test message %d from client %d", num, getpid());
    n = write(sockfd, msg, strlen(msg)); // Send message to the server
  }
}

int main() {
  char buffer[MAXBUF];
  int fds[NUM_OF_CLIENTS];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, n, i, max = 0;;
  int sockfd;
  fd_set rset;

  // Create multiple child processes
  for (i = 0; i < NUM_OF_CLIENTS; i++) {
    if (fork() == 0) {
      child_process();
      exit(0);
    }
  }

  // Create server socket, bind and start listening
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  listen(sockfd, NUM_OF_CLIENTS);

  // Accept all incoming connections
  for (i = 0; i < NUM_OF_CLIENTS; i++) {
    memset(&client, 0, sizeof(client));
    addrlen = sizeof(client);
    fds[i] = accept(sockfd, (struct sockaddr*)&client, &addrlen);
    if (fds[i] > max)
      max = fds[i];
  }

  // Server starts processing messages
  while (1) {
    FD_ZERO(&rset); //Reset the file descriptor set

    // Add all active connection descriptors to the set
    for (i = 0; i < NUM_OF_CLIENTS; i++) {
      FD_SET(fds[i], &rset);
    }

    puts("round again");

    // Monitor active connections for incoming data
    select(max + 1, &rset, NULL, NULL, NULL);

    // Process all incoming messages
    for (i = 0; i < NUM_OF_CLIENTS; i++) {
      if (FD_ISSET(fds[i], &rset)) {
        memset(buffer, 0, MAXBUF);
        read(fds[i], buffer, MAXBUF); // Read incoming message
        puts(buffer); // Display message
      }
    }
  }
  return 0;
}
// Note: This code does not include error handling, and it doesn't handle client disconnections.