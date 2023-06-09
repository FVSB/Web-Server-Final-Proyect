#include <unistd.h>
#include <errno.h>
#include "Read_Write_Socket.h"

char *ReadRequest(int fd, int n)
{

   size_t nleft = n;
   ssize_t nread = 0;
   // Asignar memoria al buffer
   char *usrbuf = calloc(1, n);

   char *bufp = usrbuf;
   // Tratar de leer la informacion
   do
   {
      // Cant maxima de bites a leer
      nread = read(fd, bufp, nleft);
      // Si la cant de bites leidos coincide con la cant de bites que se pueden leer
      if (nread == nleft)
      {
         // Si termina en "\r\n\r\n" entonces ya se leyó todo  la informacion y se para
         if (strncmp(bufp + nread - 4, "\r\n\r\n", 4) == 0)
            break;
         // En caso contrario es que falto por leer posiblemente por capacidad entonces se
         //  duplica la cant de bites a leer
         n *= 2;
         // Se aumenta la memoria
         usrbuf = realloc(usrbuf, n);
         // Se actualiza el buffer a la mitad de la memoria que existia antes
         bufp = usrbuf + n / 2;
         nleft = n / 2;
      }

      else if (nread == -1)
      {
         if (errno == EINTR)
            nread = 0;
         else
            return -1;
      }
      // Si no se leyó mas bites o se alcanzo el final del mensaje se sale del bucle
      else if (nread == 0 || strncmp(bufp + nread - 4, "\r\n\r\n", 4) == 0)
      {
         break;
      }
      else
      {
         nleft -= nread;
         bufp += nread;
      }
   } while (1);

   return usrbuf;
}

int WriteResponse(int fd, void *usrbuf, size_t n)
{
   size_t nleft = n;
   ssize_t nwritten;
   char *bufp = usrbuf;

   while (nleft > 0)
   {
      if ((nwritten = write(fd, bufp, nleft)) <= 0)
      {
         if (errno == EINTR) /* Interrupted by sig handler return */
            nwritten = 0;
         else
            /* and call write() again */
            return -1;
         /* errno set by write() */
      }
      nleft -= nwritten;
      bufp += nwritten;
   }
   return n;
}
