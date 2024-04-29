#pragma once

#ifndef _FILE_LIST
#define _FILE_LIST

#ifndef _BOOL
#define _BOOL

typedef enum
{
	FALSE,
	TRUE

} boolean;

#endif // !_BOOL

#ifndef _STRING
#define _STRING

#define MAX_STR_LEN  100

typedef char string[MAX_STR_LEN];
#endif // !_STRING

// Node
typedef struct fileUnit
{
  unsigned int  fileId;
  unsigned int  fileVarables[3];

} FileUnit, *pFileUnit;

typedef struct node
{
  FileUnit     info;
	struct node *next;

} FileList, *ptrFileList;

void    initFileList(ptrFileList list);
boolean isEmptyFileList(ptrFileList list);
void    pushFileListToEnd(ptrFileList* list, unsigned int fileId, unsigned int var1, unsigned int var2, unsigned int var3);
void    combineList(ptrFileList *list, ptrFileList toAdd);
void   *removeAfterFileList(ptrFileList node);
#endif
