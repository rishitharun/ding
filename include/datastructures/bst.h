#define BST_H

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef NODE_H
#define NODE_H
#include "datastructures/node.h"
#endif

char insertNode(BinarySearchTree** root, Node* node);
void freeTree(BinarySearchTree* root);
BinarySearchTree* getNode(BinarySearchTree* root, char letter, short* index);

