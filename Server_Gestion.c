#include <stdio.h>
#include "Server_Gestion.h"

void Server_Response(int new_socket, char *uri, char *buff,
                     char *orig_path, char *button, char *method, DirLinkedList *list,
                     int list_len)
{
    printf("Entro a respuesta de la peticion");
    uri = Uri_parser(uri);
    buff = NULL;
    printf(" Check 1");
    // Si es la misma dirección que se paso como entrada
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
        printf("invi");
        Get_Petition(new_socket, uri, orig_path, list, list_len, button);
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
        {
            nxt = '0';
        }
        HTMLresponse(list, uri, connected_fd, button, nxt);
    }

    else
    {
        HTMLerror(new_socket, method, "501", "Not Implemented", "Web Server doesn't implement this method");
    }
}

void Get_Petition(int new_socket, char *uri, char *orig_path, DirLinkedList *list, int list_len, char *button)
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
        // Implementar las descargas de archivos
    }
    else if (strstr(uri, orig_path) != NULL)
    {
        HTMLerror(new_socket, uri, "404", "Not Found", "Web Server could'n find this file");
    }
}