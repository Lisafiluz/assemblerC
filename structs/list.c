//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//
#include "list.h"
#include "macro.h"
#include "string.h"

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

void newLinkedList(linkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}

void newNode(node *node) {
    node->id = NULL;
    node->data = NULL;
    node->next = NULL;
}

int isIdExist(char *id, linkedList *list) {
    node *currNode;
    currNode = list->head;
    if (isNotEmpty(list)) {
        while (currNode != NULL) {
            if (strcmp((char *) currNode->id, id) == 0) {
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

int isNotEmpty(linkedList *list) {
    return list->head != NULL;
}


