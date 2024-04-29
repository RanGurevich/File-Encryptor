#include "FileList.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void initFileList(ptrFileList list)
{
	list = NULL;
}



boolean isEmptyFileList(ptrFileList list)
{
	return (!list) ? TRUE : FALSE;
}



void pushFileListToEnd(ptrFileList *list, unsigned int fileId, unsigned int var1, unsigned int var2, unsigned int var3)
{
	pFileUnit tempNode = (pFileUnit)malloc(sizeof(pFileUnit));
	ptrFileList head = (ptrFileList)malloc(sizeof(ptrFileList));

	// Code Section
	head = *list;
	tempNode->fileId = fileId;
	tempNode->fileVarables[0] = var1;
	tempNode->fileVarables[1] = var2;
	tempNode->fileVarables[2] = var3;
	if ((*list) == NULL)
	{

		(*list)->info = *tempNode;
	}
	else
	{
		while ((*list)->next != NULL)
			(*list) = (*list)->next;

		(*list)->next = tempNode;
		*list = head;
	}
}



void *removeAfterFileList(ptrFileList node)
{
	void *tempData;
	ptrFileList tempNode = node->next;
	tempData = node;
	node->next = tempNode->next;
	free(tempNode);
	return tempData;
}



void combineList(ptrFileList *list, ptrFileList toAdd)
{
	ptrFileList tempNode = list;
	while (tempNode->next)
		tempNode = tempNode->next;
	tempNode->next = toAdd;
}
