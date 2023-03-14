/*

 Created by Iluz, Lisaf(AWF) on 03/02/2023.
*/


#ifndef ASSEMBLERC_LIST_H
#define ASSEMBLERC_LIST_H

typedef struct node *node;

typedef struct linkedList *linkedList;

void initLinkedList(linkedList list);

void initNode(node node);

void *getDataById(void *id, linkedList list);

void add(node nodeToAdd, linkedList list);

linkedList createNewLinkedList();

node createNewNode(void *id, void *data);

int isIdExist(char *id, linkedList list);

int isListNotEmpty(linkedList list);

void freeLinkedList(linkedList list);

int getLength(linkedList list);

node getHead(linkedList list);

node getTail(linkedList list);

void *getId(node node);

void *getData(node node);

node getNextNode(node node);

void setData(node node, void *data);

#endif
