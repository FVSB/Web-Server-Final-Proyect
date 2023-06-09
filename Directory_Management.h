#ifndef DIRECTORY_MANAGEMENT_H
#define DIRECTORY_MANAGEMENT_H
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
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
void SortBy(DirLinkedList **list, int n, int (*comparer)(DirLinkedList *, DirLinkedList *));
int NameCompare(DirLinkedList *current, DirLinkedList *next);
int SizeCompare(DirLinkedList *current, DirLinkedList *next);
int DateMCompare(DirLinkedList *current, DirLinkedList *next);
void SetDate(DirLinkedList *list);
void SwapDirLinkedList(DirLinkedList **current, DirLinkedList **next);
int NameRCompare(DirLinkedList *current, DirLinkedList *next);
int SizeRCompare(DirLinkedList *current, DirLinkedList *next);
int DateMRCompare(DirLinkedList *current, DirLinkedList *next);
int TypeCompare(DirLinkedList *current, DirLinkedList *next);
int TypeRCompare(DirLinkedList *current, DirLinkedList *next);
#endif