#include "codes/node.h"
#include "datastructures/ntree.h"

#include "logger.h"

void insertNodeNTree(Node* root, Node* child)
{
  if (root->left == NULL) { root->left = child; return; }
  else {;}

  Node* curr = root->left;
  while(curr->right!=NULL) curr = curr->right;
  curr->right = child;
}

void freeNTree(Node* root)
{
  if (root==NULL) return;
  else {;}

  freeNTree(root->left);
  freeNTree(root->right);

  Node* inner_node = (Node*)(root->data);

  if (inner_node->type == RAW_VARIABLE_NODE)
  {
    #if LOGGING_ENABLED(LOG_MEMORY) > 0
      MEMORY_LOGGING_UTIL("F\0", (long int) 0, inner_node->data, "data of raw variable inner node in NTree");
    #endif
    free(inner_node->data);
    inner_node->data = NULL;
  }
  else {;}
  if (inner_node->type == MEMBER_NODE)
  {
    #if LOGGING_ENABLED(LOG_MEMORY) > 0
      MEMORY_LOGGING_UTIL("F\0", (long int) 0, inner_node->data,"data of member variable inner node in NTree");
      MEMORY_LOGGING_UTIL("F\0", sizeof(Node), inner_node->data, "member variable inner node in NTree");
    #endif

    free(inner_node->data); inner_node->data = NULL;
    free(inner_node); inner_node = NULL;
  }
  else {;}

  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("F\0", sizeof(Node), root, "Node in NTree");
  #endif
  free(root);
  root = NULL;
}

/*
======================================================================
|----------------- DEBUG FUNCTION USED BY THE AUTHOR ----------------|
|--------------------------------------------------------------------|
|-USE THESE OR DEFINE YOUR OWN FUNCTIONS TO DEBUG DURING DEVELOPMENT-|
======================================================================

void printTree(Node* root)
{
  if(root == NULL) return;
  else {printf("\n");}

  printf("<>\n");
  printf("LEFT ");
  printTree(root->left);
  printf("RIGHT ");
  printTree(root->right);
}
*/

