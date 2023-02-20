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
void newLinkedList(linkedList *list);
void newNode(node *node);
void *getDataById(void *id, linkedList *list);
void add(node *nodeToAdd, linkedList *list);
int isIdExist(char *id, linkedList *list);
int isNotEmpty(linkedList *list);
#endif //ASSEMBLERC_LIST_H
