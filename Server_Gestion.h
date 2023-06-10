#ifndef Server_Gestion_H
#define Server_Gestion_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "URI_parser.h"
#include "Read_Socket.h"
#include "Directory_Management.h"
#include "HTML_Gestion.h"

void Server_Response(int new_socket, char *uri, char *buff, char *orig_path, char *button, char *method, DirLinkedList *list, int list_len);

void Get_Petition(int new_socket, char *uri, char *orig_path, DirLinkedList *list, int list_len, char *button);
#endif