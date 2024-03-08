#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int server;
  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("socket bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server, 2) < 0) {
    perror("socket listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Waiting for incoming connections...\n");

  int addrlen = sizeof(address);
  int client_count = 0, client_sockets[] = {0, 0};

  while (client_count < 2) {
    if ((client_sockets[client_count] = accept(
             server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    printf("Client %d connected\n", client_count + 1);
    client_count++;
  }

  char buffer[BUFFER_SIZE];
  int read_size;

  while (true) {
    for (int i = 0; i < client_count; i++) {
      if ((read_size = read(client_sockets[i], buffer, BUFFER_SIZE)) == 0) {
        printf("exit.");
        return 0;
      }

      printf("Message from client %d: %s\n", i + 1, buffer);
      send(client_sockets[i], buffer, strlen(buffer), 0);

      memset(buffer, 0, BUFFER_SIZE);
    }
  }

  close(server);

  return 0;
}
