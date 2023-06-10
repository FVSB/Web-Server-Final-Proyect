// Testing del server
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "URI_parser.h"
#include "Read_Socket.h"
#include "Directory_Management.h"
#include "HTML_Gestion.h"
#include "Server_Gestion.h"
#include "Thread_Server.h"

void launch(struct Server *server, char *orig_path)
{

    // Longitud de la dirección
    int address_len = sizeof(server->address);

    while (1)
    {
        printf("Esperando conexión\n");
        int new_socket;
        new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_len);
        printf("Conexión aceptada\n");

        // Crear una estructura para pasar los datos al hilo
        ThreadData *thread_data = malloc(sizeof(ThreadData));
        thread_data->server = server;
        thread_data->orig_path = orig_path;
        thread_data->new_socket = new_socket;

        // Crear el hilo para manejar el cliente
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)thread_data);
        pthread_detach(thread); // Liberar recursos automáticamente cuando el hilo termine
    }

    close(server->socket);
}

int main(int argc, char **argv)
{
    char *orig_path = argv[2];
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
    if (chdir(orig_path) < 0)
    {
        fprintf(stderr, "%s: Invalid directory %s", argv[0], argv[2]);
        exit(1);
    }

#pragma endregion

#pragma region Server Constructor
    struct Server server = server_constructor(AF_INET, SOCK_STREAM,
                                              0, INADDR_ANY, port, 10, launch);

    server.launch(&server, orig_path);
#pragma endregion
}
