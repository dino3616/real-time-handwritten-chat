#ifndef MODULE_SOCKET_SOCKET_H
#define MODULE_SOCKET_SOCKET_H

extern int *connect_with_socket(char *host_name, int port, int *socket_fd);

extern int *listen_with_socket(char *host_name, int port, int *socket_fd);

extern void close_socket(int socket_fd);

#endif
