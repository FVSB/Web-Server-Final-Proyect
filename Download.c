#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

int Download(int fd, char *filename, long int size)
{
    char *buff = calloc(1, 8192); // Búfer más grande, ajustable según tus necesidades

    sprintf(buff, "HTTP/1.1 200 OK\r\n");
    sprintf(buff, "%sMIME-Version: 1.0\r\n", buff);
    sprintf(buff, "%sContent-Type: application/octet-stream\r\n", buff);
    sprintf(buff, "%sContent-Disposition: attachment; filename=\"%s\"\r\n", buff, strrchr(filename, '/') + 1);
    sprintf(buff, "%sContent-Length: %ld \r\n\r\n", buff, size);

    // Enviar la confirmación
    write(fd, buff, strlen(buff));

    free(buff);

    // Abrir el archivo
    int filefd = open(filename, O_RDONLY);
    if (filefd == -1)
    {
        printf("Error opening file\n");
        return -1;
    }

    off_t offset = 0;
    ssize_t bytes_sent;
    ssize_t chunk_size = 1024 * 1024; // Tamaño del pedazo en bytes (1 MB)

    while (size > 0)
    {
        if (size < chunk_size)
            chunk_size = size;

        bytes_sent = sendfile(fd, filefd, &offset, chunk_size);
        if (bytes_sent == -1)
        {
            printf("Error sending file\n");
            close(filefd);
            return -1;
        }

        size -= bytes_sent;
    }

    // Enviar final de la instrucción
    write(fd, "\r\n", 2);
    close(filefd);
    return 1;
}
