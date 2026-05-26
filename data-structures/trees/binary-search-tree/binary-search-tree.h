#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#pragma once
#include <stdlib.h>

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int data;

} Node;

Node *newNode(int data);

Node *insert(Node *root, int data);

Node *remove(Node *root, int data);

int height(Node *root);

Node *getMax(Node *root);



#endif