#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"

static ServerState *server_state = NULL;

void *handle_connection(int *sock_fd) {
  int client_fd = *sock_fd;
  free(sock_fd);
  // handle message
  printf("Handling connection on %d\n", client_fd);

  char buffer[BUFFER_SIZE];

  while (1) {
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
      perror("Thread read error");
      break;
    } else if (bytes_read == 0) {
      // break if connection is closed
      printf("Thread [%ld]: Client disconnected on socket %d\n", pthread_self(),
             client_fd);
      break;
    }

    buffer[bytes_read] = '\0';
    if (server_state->v_flag == 1) {
      printf("Thread [%ld]: Received message: %s\n", pthread_self(), buffer);
    }
    // only closes the connection and does not exit the server, not sure how to
    // cleanly exit server
    if (strcmp(buffer, "quit\n") == 0 || strcmp(buffer, "quit\r\n") == 0) {
      server_state->shutdown_flag = 1;
      printf("Shutdown command received. Server will exit after closing "
             "connection.\n");
      break;
    }

    ssize_t bytes_written = write(client_fd, buffer, bytes_read);
    if (bytes_written < 0) {
      perror("Thread write error");
      break;
    }
  }
  printf("Closing connection on %d\n", client_fd);
  close(client_fd);
  return NULL;
}

void run_server(int socket_fd, ServerState *state) {
  server_state = state;

  if (socket_fd < 0) {
    fprintf(stderr, "Failed to create listening socket\n");
    return;
  }

  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  while (server_state->shutdown_flag == 0) {

    int *client_fd_buf = malloc(sizeof(int));

    *client_fd_buf = accept(socket_fd, (struct sockaddr *)&client_address,
                            &client_address_len);

    printf("Accepted connection on %d\n", *client_fd_buf);

    pthread_t thread;
    pthread_create(&thread, NULL, (void *(*)(void *))handle_connection,
                   (void *)client_fd_buf);
  }
  close(socket_fd);
}