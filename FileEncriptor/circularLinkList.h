#pragma once
#pragma once

#ifndef _CIRCULAR_LIST
#define _CIRCULAR_LIST

#ifndef N
  #define N  50
#endif // !N

#ifndef _STRING
#define _STRING
typedef char  STRING[N];
#endif // !_STRING

#ifndef _BOOL
#define _BOOL
typedef enum { FALSE, TRUE }boolean;
#endif

typedef struct linkCircularNode
{
  void                    *info;
  struct linkCircularNode *next;

} linkCircularNode, *linkCircularNodePtr;

// function type
void  initCircularLinkList(linkCircularNodePtr *list);
void  insertEndCircularLinkList(linkCircularNodePtr *list, void *info);
void *deleteEndCircularLinkList(linkCircularNodePtr *list);
void  insertAfterCircularLinkList(linkCircularNodePtr node, void *info);
void *deleteAfterCircularLinkList(linkCircularNodePtr node);
void  emptyList(linkCircularNodePtr *list, void(*freeInt)(void *));
void  printCircularLinkList(linkCircularNodePtr list, void(*prn)(void *));

#endif // !_CIRCULAR_LIST
