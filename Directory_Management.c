#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#include "Directory_Management.h"
// Envia un directorio
DirLinkedList *Initialize(char *directory, int *n)
{
   // Reserva memoria para el nodo
   DirLinkedList *result = calloc(1, sizeof(DirLinkedList));
   // Establece que no hay previous dado que se inicializó
   result->previus = NULL;

   DirLinkedList *ptr = result;
   // Abrir la carpeta y almacenar los archivos que tiene
   DIR *currentFolder = opendir(directory);

   struct dirent *current_subfolder;
   // Leer la carpeta
   current_subfolder = readdir(currentFolder);
   // Siempre y cuando exista mas info en la carpeta
   while ((current_subfolder = readdir(currentFolder)) != NULL)
   {
      (*n)++;
      char *subfolder_path = calloc(1, sizeof(directory) + sizeof(current_subfolder->d_name) + 1);
      strcpy(subfolder_path, directory);
      strcat(subfolder_path, "/");
      strcat(subfolder_path, current_subfolder->d_name);
      // Rellenar con info de la subcarpeta
      if (stat(subfolder_path, &(ptr->state)) < 0)
      {
         continue;
      }
      // El nombre de la subcarpeta
      ptr->name = subfolder_path;
      // Se añade un nodo
      ptr->next = (DirLinkedList *)malloc(sizeof(DirLinkedList));
      ptr->next->previus = ptr;
      ptr = ptr->next;
   }

   ptr->previus->next = NULL;
   ptr = NULL;
   SetDate(result);
   return result;
}

void SetDate(DirLinkedList *list)
{
   while (list != NULL)
   {
      struct tm *time_subfolder = calloc(1, sizeof(struct tm));
      time_subfolder = localtime_r(&(list->state.st_atime), time_subfolder);
      list->mday = time_subfolder->tm_mday;
      list->month = time_subfolder->tm_mon;
      list->myear = time_subfolder->tm_year;
      free(time_subfolder);
      list = list->next;
   }
}
