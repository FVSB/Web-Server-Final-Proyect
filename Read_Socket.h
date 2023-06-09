#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifndef READ_WRITE_SOCKET_H
#define READ_WRITE_SOCKET_H
#include <sys/types.h>

char *ReadRequest(int fd, int size);

#endif
