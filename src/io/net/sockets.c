
#include <sys/socket.h>
#include <netinet/in.h>


int Eco_Net_OpenServerSocketV4(int port, int listening)
{
    int                 value;
    int                 sock_fd;
    struct sockaddr_in  server;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1) return -1;

    value = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port        = htons(port);

    if (bind(sock_fd, (struct sockaddr*) &server, sizeof(server)) < 0)
        return -1;
    
    listen(sock_fd, listening);

    return sock_fd;
}
