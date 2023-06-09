// Aca es la declaración del server

#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Server
{
   int domain;
   int service;
   int protocol;
   unsigned long interface;
   int port;
   int backlog;

   struct sockaddr_in address;

   void (*launch)(struct Server *server, char **orig_path);

   int socket;
};

struct Server server_constructor(int domain, int service, int protocol,
                                 unsigned long interface, int port, int backlog, void (*launch)(struct Server *server, char **orig_path));

#endif /*Server_h*/