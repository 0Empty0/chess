
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int sock = 0;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error \n");
    return -1;
  }

  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
    printf("Invalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connection Failed \n");
    return -1;
  }

  char message[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];

  while (true) {
    printf("Enter message: ");
    scanf("%s", message);

    if (strcmp(message, "exit") == 0) {
      printf("exit.");
      return 0;
    }

    send(sock, message, strlen(message), 0);
    printf("Message sent\n");

    if (read(sock, buffer, 1024) > 0) {
      printf("Server: %s\n", buffer);
    }

    memset(buffer, 0, strlen(buffer));
  }

  close(sock);
  return 0;
}
