#include <stdlib.h>
#include <string.h>

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
      time_subfolder = localtime_r(&(list->state.st_atim), time_subfolder);
      list->mday = time_subfolder->tm_mday;
      list->month = time_subfolder->tm_mon;
      list->myear = time_subfolder->tm_year;
      free(time_subfolder);
      list = list->next;
   }
}
void SwapDirLinkedList(DirLinkedList **current, DirLinkedList **next)
{
   DirLinkedList *temp;

   (*current)->next = (*next)->next;
   if ((*next)->next != NULL)
      (*next)->next->previus = *current;

   (*next)->next = *current;
   (*next)->previus = (*current)->previus;
   (*current)->previus = *next;

   if ((*next)->previus != NULL)
      (*next)->previus->next = *next;

   temp = *current;
   *current = (*next)->next;
   *next = temp->next;
}

void SortBy(DirLinkedList **list, int n, int (*comparer)(DirLinkedList *, DirLinkedList *))
{

   DirLinkedList *current = *list;
   DirLinkedList *next = current->next;
   DirLinkedList *temp;
   while (n-- > 0)
   {
      while (current->next != NULL)
      {
         if (comparer(current, next))
         {

            SwapDirLinkedList(&current, &next);
         }
         else
         {
            current = current->next;
            next = current->next;
         }
      }
      while (current->previus != NULL)
      {
         current = current->previus;
      }
      next = current->next;
   }

   while (current->previus != NULL)
   {
      current = current->previus;
   }
   *list = current;
}

int NameCompare(DirLinkedList *current, DirLinkedList *next)
{
   return strcmp(current->name, next->name) > 0;
}
int SizeCompare(DirLinkedList *current, DirLinkedList *next)
{
   int stSizeCurr = current->state.st_size;
   int stSizeNext = next->state.st_size;
   if (S_ISDIR(current->state.st_mode))
      stSizeCurr = 0;
   if (S_ISDIR(next->state.st_mode))
      stSizeNext = 0;
   return stSizeCurr > stSizeNext;
}
int DateMCompare(DirLinkedList *current, DirLinkedList *next)
{
   return current->myear > next->myear || (current->myear == next->myear && current->month > next->month) || (current->myear == next->myear && current->month == next->month && current->mday > next->mday);
}
int NameRCompare(DirLinkedList *current, DirLinkedList *next)
{
   return NameCompare(next, current);
}
int SizeRCompare(DirLinkedList *current, DirLinkedList *next)
{
   return SizeCompare(next, current);
}
int DateMRCompare(DirLinkedList *current, DirLinkedList *next)
{
   return DateMCompare(next, current);
}
int TypeCompare(DirLinkedList *current, DirLinkedList *next)
{
   return S_ISDIR(current->state.st_mode) && !S_ISDIR(next->state.st_mode);
}
int TypeRCompare(DirLinkedList *current, DirLinkedList *next)
{
   return TypeCompare(next, current);
}