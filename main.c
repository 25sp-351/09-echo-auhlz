#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"
#include "server_socket.h"

int main(int argc, char *argv[]) {
  int port = PORT;
  ServerState state;

  // check for arguments
  for (int ix = 0; ix < argc; ix++) {
    if (strcmp(argv[ix], "-v") == 0) {
      state.v_flag = 1;
    }
    if (strcmp(argv[ix], "-p") == 0 && ix + 1 < argc) {
      sscanf(argv[ix + 1], "%d", &port);
      if (port < 1024 || port > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port);
        return 1;
      }
      // skip the next argument
      ix++;
    }
  }

  int socket_fd = create_listening_socket(port);
  run_server(socket_fd, &state);
  close(socket_fd);
  return 0;
}