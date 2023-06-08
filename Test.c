// Testing del server
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"

void launch(struct Server *server)
{
    // Crear el buffer
    char buffer[1024];
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // Longitud de la direccion
    int address_len = sizeof(server->address);

    while (1)
    {
        printf(" Esperando conexion \n");
        int new_socket;
        new_socket = accept(server->socket,
                            (struct sockaddr *)&server->address,
                            (socklen_t *)&address_len);

        // Leer el mensaje del cliente
        int valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        // Enviar el mensaje al cliente
        write(new_socket, hello, strlen(hello));

        // Cerrar conexion
        close(new_socket);
    }
}

int main(int argc, char **argv)
{
#pragma region Initialize
    if (argc < 3)
    {
        // Envia un mensaje de error
        fprintf(stderr, "Usage: %s <Port> <Directory>", argv[0]);
        exit(1);
    }
    char *endptr;
    // Convertir el puerto a un entero
    int port = strtol(argv[1], &endptr, 0);

    // Es que no se pudo convertir completamente el valor
    //  Por tanto lanzar exepcion
    if (*endptr != '\0')
    {
        fprintf(stderr, "%s: Invalid port %s", argv[0], argv[1]);
        exit(1);
    }
    // Verificar si se puede cambiar de directorio
    if (chdir(argv[2]) < 0)
    {
        fprintf(stderr, "%s: Invalid directory %s", argv[0], argv[2]);
        exit(1);
    }

#pragma endregion

    struct Server server = server_constructor(AF_INET, SOCK_STREAM,
                                              0, INADDR_ANY, 80, 10, launch);
    server.launch(&server);
}
