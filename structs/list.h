//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#ifndef ASSEMBLERC_LIST_H
#define ASSEMBLERC_LIST_H
typedef struct node {
    void *id;
    void *data;
    struct node *next;
} node;
typedef struct linkedList {
    struct node *head;
    struct node *tail;
} linkedList;

void initLinkedList(linkedList *list);

void initNode(node *node);

void *getDataById(void *id, linkedList *list);

void add(node *nodeToAdd, linkedList *list);

linkedList *createNewLinkedList();

node *createNewNode(void *id, void *data);

int isIdExist(char *id, linkedList *list);

int isListNotEmpty(linkedList *list);

void freeLinkedList(linkedList *list);

int getLength(const linkedList *list);

#endif //ASSEMBLERC_LIST_H
