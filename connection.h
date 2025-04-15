#ifndef CONNECTION_H
#define CONNECTION_H

#include <pthread.h>

#define LISTEN_BACKLOG 5
#define BUFFER_SIZE 1024
#define PORT 8888

typedef struct {
  int v_flag;
  int shutdown_flag;
} ServerState;

void *handle_connection(int *sock_fd);

void run_server(int socket_fd, ServerState *state);

#endif
