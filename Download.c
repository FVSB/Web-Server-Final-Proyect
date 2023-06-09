#include "Download.h"

int Download(int fd, char *filename, int size)
{
    char *buff = calloc(1, 1024);

    sprintf(buff, "HTTP/1.1 200 OK\r\n");
    sprintf(buff, "%sMIME-Version: 1.0\r\n", buff);
    sprintf(buff, "%sContent-Type: application/octet-stream\r\n", buff);
    sprintf(buff, "%sContent-Disposition: attachment; filename=\"%s\"\r\n", buff, strrchr(filename, '/') + 1);
    sprintf(buff, "%sContent-Length: %d \r\n\r\n", buff, size);

    // Enviar la confirmación
    write(fd, buff, strlen(buff));

    free(buff);

    // Abrir el archivo
    int filefd = open(filename, O_RDONLY, 0);
    off_t offset = 0;

    while (size > 0)
    {
        int curr_size = size > 1048576 ? 1048576 : size; // 1 MB (1024 * 1024)

        if (sendfile(fd, filefd, &offset, curr_size) < 0)
        {
            // Si fue interrumpida la copia por una señal, volver a empezar
            if (errno == EINTR)
            {
                curr_size = 0;
            }
            else
            {
                printf("Error sending file");
                close(filefd);
                return -1;
            }
        }

        size -= curr_size;
    }
    // Enviar final de la instrucción
    write(fd, "\r\n", 2);
    close(filefd);
    return 1;
}