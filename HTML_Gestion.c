
#include "HTML_Gestion.h"
int HTMLresponse(DirLinkedList *list, char *path, int fd, char *button, char nxt)
{

    int size = 5000;
    char *buff = calloc(1, size);
    char *buff_2 = malloc(1000);

    sprintf(buff, "%s<html> <head><meta charset=\"utf-8\"> Directorio %s</head> <body> <table> <tr> <th>Name <form method=\"post\" action=\"%s~0~%c\"><input type=\"submit\"  value=\" &uarr;&darr; \"></form> </th> <th>Size<form method=\"post\" action=\"%s~2~%c\"><input type=\"submit\"  value=\" &uarr;&darr; \"></form></th><th>Last Modif Date<form method=\"post\" action=\"%s~4~%c\"><input type=\"submit\"  value=\" &uarr;&darr; \"></form></th>", buff, path, button + 1, nxt, button + 1, nxt, button + 1, nxt);
    sprintf(buff, "%s<th>Type<form method=\"post\" action=\"%s~6~%c\"><input type=\"submit\"  value=\" &uarr;&darr; \"></form></th></tr>", buff, button + 1, nxt);
    while (list != NULL)
    {
        if ((int)strlen(buff) >= (size - 500))
        {
            size *= 2;
            buff = realloc(buff, size);
        }
        int stsize = list->state.st_size;
        char *type = "folder";
        // Si es una carpeta
        if (S_ISDIR(list->state.st_mode))
            // Tamaño es 0
            stsize = 0;
        else
            type = "file";
        sprintf(buff, "%s<tr><td><a href=\"%s\">%s</a></td><td>%d bytes</td><td>%d-%d-%d</td><td>%s</td></tr>",
                buff, list->name, strrchr(list->name, '/') + 1, stsize, list->mday, list->month + 1, list->myear + 1900,
                type);
        DirLinkedList *ptr = list;
        list = list->next;
        free(ptr->name);
        free(ptr);
    }

    sprintf(buff, "%s</table></body></html>\r\n\r\n", buff);

    int len = strlen(buff);
    sprintf(buff_2, "HTTP/1.1 200 OK\r\n");
    sprintf(buff_2, "%sMIME-Version: 1.0\r\n", buff_2);
    sprintf(buff_2, "%sContent-Type: text/html\r\n", buff_2);
    sprintf(buff_2, "%sContent-Length: %d \r\n\r\n", buff_2, len);
    // Se lleva la pag a entero
    int result = write(fd, buff_2, strlen(buff_2)) + write(fd, buff, len);
    free(buff);
    free(buff_2);
    return result;
}

void HTMLerror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[500], body[1000];

    sprintf(body, "<html><title>Error</title>");
    sprintf(body, "%s<body bgcolor="
                  "ffffff"
                  ">\r\n",
            body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>Web Server</em>\r\n", body);

    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    sprintf(buf, "%sContent-type: text/html\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));

    write(fd, buf, strlen(buf));
    write(fd, body, strlen(body));
}