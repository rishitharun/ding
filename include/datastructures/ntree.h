
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

void insertNodeNTree(Node* root, Node* child);
void freeNTree(Node* root);

