#ifndef READ_WRITE_SOCKET_H
#define READ_WRITE_SOCKET_H 
#include <sys/types.h>

char * ReadRequest(int fd, int size);
int WriteResponse(int fd, void *usrbuf, size_t n);

#endif
