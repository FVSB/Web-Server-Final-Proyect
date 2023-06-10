#ifndef Thread_Server_H
#define Thread_Server_H
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
#include <pthread.h>
// Estructura para pasar los datos al hilo
typedef struct
{
    struct Server *server;
    char *orig_path;
    int new_socket;
} ThreadData;

void *handle_client(void *data);
#endif