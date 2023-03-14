/*
 Created by Iluz, Lisaf(AWF) on 21/02/2023.
*/
#include <stdlib.h>
#include "list.h"
#include "string.h"
#include "../util/readerUtils.h"

#define TRUE 1
#define FALSE 0

struct node {
    void *id;
    void *data;
    node next;
};

struct linkedList {
    node head;
    node tail;
};

void add(node nodeToAdd, linkedList list) {
    if (list->head == NULL) {
        list->head = nodeToAdd;
        list->tail = nodeToAdd;
    } else {
        list->tail->next = nodeToAdd;
        list->tail = nodeToAdd;
    }
}

node createNewNode(void *id, void *data) {
    node n;
    n = malloc(sizeof(*n));
    initNode(n);
    n->id = (void *) id;
    n->data = (void *) data;
    return n;
}

linkedList createNewLinkedList() {
    linkedList list;
    list = malloc(sizeof(*list));
    initLinkedList(list);
    return list;
}

void initLinkedList(linkedList list) {
    list->head = NULL;
    list->tail = NULL;
}

void initNode(node node) {
    node->id = NULL;
    node->data = NULL;
    node->next = NULL;
}

int isIdExist(char *id, linkedList list) {
    if (isListNotEmpty(list)) {
        node currNode;
        currNode = list->head;
        while (currNode != NULL) {
            if (isEqual((char *) (currNode->id), id)) {
                return TRUE;
            }
            currNode = currNode->next;
        }
    }
    return FALSE;
}

void *getDataById(void *id, linkedList list) {
    node curr = list->head;

    while (curr != NULL) {
        if (strcmp((const char *) (curr->id), (const char *) id) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

int isListNotEmpty(linkedList list) {
    return list != NULL && list->head != NULL;
}

void freeLinkedList(linkedList list) {
    if (isListNotEmpty(list)) {
        node currNode = list->head;
        node nextNode;
        while (currNode != NULL) {
            nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
    }
}

int getLength(linkedList list) {
    node curr = list->head;
    int i;
    i = 0;
    while (curr != NULL) {
        i++;
        curr = curr->next;
    }
    return i;
}

node getHead(linkedList list) {
    return list->head;
}

node getTail(linkedList list) {
    return list->tail;
}

void *getId(node node) {
    return node->id;
}

void *getData(node node) {
    return node->data;
}

node getNextNode(node node) {
    return node->next;
}

void setData(node node, void *data) {
    node->data = data;
}
