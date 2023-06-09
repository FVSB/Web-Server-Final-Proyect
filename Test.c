// Testing del server
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "Directory_Management.h"
#include "Read_Write_Socket.h"
void launch(struct Server *server, char **orig_path)

{
#pragma region Variables
    struct sockaddr_in client_socket;
    int client_len;
    int size = 100;
    char *buffer;
    char method[10];
    char *uri = calloc(1, 1000);
    char *last_folder_path = calloc(1, 1000);
    char version[10];
    int list_len = 0;
    char *button;
    DirLinkedList *list;
#pragma endregion

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
        buffer = ReadRequest(new_socket, size);
        printf("%s\n", &buffer);
        // Separar Salida en 3 cadenas
        sscanf(buffer, "%s %s %s", method, uri, version);

        Server_Response(new_socket, uri, buffer, orig_path, button, method, list, list_len);

        // Cerrar conexion
        printf("Conexion cerrada\n");

        close(new_socket);
        close(server->socket);
    }
}
void Server_Response(int new_socket, char *uri, char *buff, char **orig_path, char *button, char method, DirLinkedList *list, int list_len)
{
#pragma region Comparer

    // Arreglo que contiene los comparadores
    int (*comparer[8])(DirLinkedList *, DirLinkedList *);
    comparer[0] = NameCompare;
    comparer[1] = NameRCompare;
    comparer[2] = SizeCompare;
    comparer[3] = SizeRCompare;
    comparer[4] = DateMCompare;
    comparer[5] = DateMRCompare;
    comparer[6] = TypeCompare;
    comparer[7] = TypeRCompare;

#pragma endregion
    uri = Uri_parser(uri);
    buff = NULL;
    // Si es la nisma dirección que se paso como entrada
    if (strcmp(uri, orig_path) != 0)
    {
        // última aparación del /
        button = strrchr(uri, '/');
    }
    else
        button = uri;
    // Si el tipo de petición es Get
    if (strcmp(method, "GET") == 0)
    {
        // Piensa si estas en la carpeta raiz
        if (strcmp(uri, "/") == 0)
        { // Por defecto se entraria a la raiz que se introdujo al iniciar el programa
            uri = strcpy(uri, orig_path);
        }
        struct stat folder;
        // Rellenar la estructura
        if (stat(uri, &folder) == 0)
        {
            // Comprobar que sea carpeta
            if (S_ISDIR(folder.st_mode))
            {
                // Inicializa
                list = Initialize(uri, &list_len);
                // Genera el html con la respuesta
                HTMLresponse(list, uri, new_socket, button, '0');
            }
            else // En caso de ser un archivo que se pueda descargar
            {
                Download(new_socket, uri, folder.st_size);
            }
        }
        else if (strstr(uri, orig_path) != NULL)
        {
            HTMLerror(new_socket, uri, "404", "Not Found", "Web Server could'n find this file");
        }
    }
    // Caso petición "Post"
    else if (strcmp(method, "POST") == 0)
    {
        char *p = strchr(uri, '~');
        char *c = strchr(p + 1, '~');
        *c = '\0';
        c++;
        int opt = (int)*(p + 1) - 48;
        opt += (((int)*(c)-48) % 2);
        *p = '\0';
        list = Initialize(uri, &list_len);
        SortBy(&list, list_len, comparer[opt]);
        char nxt = '1';
        if (opt % 2)
            nxt = '0';
        HTMLresponse(list, uri, new_socket, button, nxt);
    }
    else
    {
        HTMLerror(new_socket, method, "501", "Not Implemented", "Web Server doesn't implement this method");
    }
}
int main(int argc, char **argv)
{

    char **orig_path = argv[2];
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
