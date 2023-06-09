#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "URI_parser.h"
#include "Read_Socket.h"
#include "Directory_Management.h"

int HTMLresponse(DirLinkedList *list, char *path, int fd, char *button, char nxt);

void HTMLerror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);