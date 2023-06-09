#ifndef DIRECTORY_MANAGEMENT_H
#define DIRECTORY_MANAGEMENT_H
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
typedef struct DirLinkedList
{
  char *name;
  int mday;
  struct stat state;
  int month;
  int myear;
  struct DirLinkedList *previus;
  struct DirLinkedList *next;

} DirLinkedList;

DirLinkedList *Initialize(char *directory, int *n);
void SetDate(DirLinkedList *list);
#endif