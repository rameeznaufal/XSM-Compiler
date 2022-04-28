#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

struct ASTNode* TreeCreate( int type, int nodetype, int value, char *name,  struct ASTNode *arglist, struct ASTNode *ptr1, struct ASTNode *ptr2, struct ASTNode *ptr3 ) 
{
struct ASTNode *node = (struct ASTNode*) malloc(sizeof(struct ASTNode));

node->type  = type;
node->nodetype = nodetype;
if(name){
  node->name = (char*) malloc(sizeof(char));  
 strcpy(node->name,name);
}
else
 node->name = NULL; 
node->value = value;
node->arglist = arglist;
node->ptr1 = ptr1;
node->ptr2 = ptr2;
node->ptr3 = ptr3;

return node;
}

void PrintPreorder(struct ASTNode* node) {
    if(node) {
        printf("%d ", node->nodetype);
        PrintPreorder(node->arglist);
        PrintPreorder(node->ptr1);
        PrintPreorder(node->ptr2);
        PrintPreorder(node->ptr3);
    }
}


