#include "Thread_Server.h"

void *handle_client(void *data)
{
    int size = 100;
    char *buffer;
    char method[10];
    char *uri = calloc(1, 1000);
    char version[10];
    int list_len = 0;
    char *button;
    DirLinkedList *list;

    ThreadData *thread_data = (ThreadData *)data;
    struct Server *server = thread_data->server;
    char *orig_path = thread_data->orig_path;
    int new_socket = thread_data->new_socket;

    // Leer el mensaje del cliente
    buffer = ReadRequest(new_socket, size);
    printf("%s\n", buffer);
    // Separar Salida en 3 cadenas
    sscanf(buffer, "%s %s %s", method, uri, version);
    printf("Entrar a responder \n");
    Server_Response(new_socket, uri, buffer, orig_path, button, method, list, list_len);
    // Cerrar la conexión
    close(new_socket);

    // Liberar la memoria de los datos del hilo
    free(thread_data);

    pthread_exit(NULL);
}