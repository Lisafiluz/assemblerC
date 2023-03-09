//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//
#include <stdlib.h>
#include "list.h"
#include "string.h"
#include "../util/readerUtils.h"

#define TRUE 1
#define FALSE 0

void add(node *nodeToAdd, linkedList *list) {
    if (list->head == NULL) {
        list->head = nodeToAdd;
        list->tail = nodeToAdd;
    } else {
        list->tail->next = nodeToAdd;
        list->tail = nodeToAdd;
    }
}

node *createNewNode(void *id, void *data) {
    node* n = (node *) malloc(sizeof(node));
    initNode(n);
    n->id = (void *) id;
    n->data = (void *) data;
    return n;
}

linkedList *createNewLinkedList() {
    linkedList *list;
    list = (linkedList *) malloc(sizeof(linkedList));
    initLinkedList(list);
    return list;
}

void initLinkedList(linkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}

void initNode(node *node) {
    node->id = NULL;
    node->data = NULL;
    node->next = NULL;
}

int isIdExist(char *id, linkedList *list) {
    node *currNode;
    currNode = list->head;
    if (isListNotEmpty(list)) {
        while (currNode != NULL) {
            if (isEqual((char *) (currNode->id), id)) {
                return TRUE;
            }
            currNode = currNode->next;
        }
    }
    return FALSE;
}

void *getDataById(void *id, linkedList *list) {
    node *curr = list->head;

    while (curr != NULL) {
        if (strcmp((const char *) (curr->id), (const char *) id) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

int isListNotEmpty(linkedList *list) {
    return list != NULL && list->head != NULL;
}

void freeLinkedList(linkedList* list) {
    if (isListNotEmpty(list)) {
        node* currNode = list->head;
        node* nextNode;
        while (currNode != NULL) {
            nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
    }
}

int getLength(const linkedList *list) {
    node *curr = list->head;
    int i;
    i = 0;
    while (curr != NULL) {
        i++;
        curr = curr->next;
    }
    return i;
}


