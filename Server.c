#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "Server.h"

// Implementacion de la estructura Server

struct Server server_constructor(int domain, int service, int protocol, unsigned long interface,
                                 int port, int backlog, void (*launch)(struct Server *server, char **orig_path))
{
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    server.launch = launch;

    // Crear el socket
    server.socket = socket(domain, service, protocol);
    if (server.socket == -1)
    {
        perror("Failed to create socket");
        exit(1);
    }

    // Verificar la disponibilidad de la dirección IP
    struct sockaddr_in tempAddress;
    tempAddress.sin_family = domain;
    tempAddress.sin_port = htons(port);
    tempAddress.sin_addr.s_addr = htonl(interface);

    if (bind(server.socket, (struct sockaddr *)&tempAddress, sizeof(tempAddress)) == -1)
    {
        if (errno == EADDRINUSE)
        {
            printf("La dirección IP ya está en uso.\n");
            close(server.socket);
            exit(1);
        }
        else
        {
            perror("Failed to bind socket");
            close(server.socket);
            exit(1);
        }
    }

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    // Esperar a que se conecte el cliente
    if (listen(server.socket, backlog) == -1)
    {
        perror("Failed to listen on socket");
        close(server.socket);
        exit(1);
    }

    server.launch = launch;

    return server;
}
