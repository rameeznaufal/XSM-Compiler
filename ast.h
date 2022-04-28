#ifndef AST_TREE_H
#define AST_TREE_H

#define CONST_NODE 0
#define VAR_NODE 1

#define ADD_NODE 2
#define SUB_NODE 3
#define MUL_NODE 4
#define DIV_NODE 5

#define GT_NODE 6
#define LT_NODE 7
#define LTE_NODE 8
#define GTE_NODE 9

#define EQ_NODE 10
#define NTE_NODE 11

#define IF_NODE 12

#define WHILE_NODE 13

#define READ_NODE 14
#define WRITE_NODE 15

#define ASGN_NODE 16
#define RET_NODE 17

#define FUNC_NODE 18

#define MAIN_NODE 19

#define FIELD_NODE 20

#define IFELSE_NODE 21

#define STMTS_NODE 22

#define ARGC_NODE 23

#define inttype 0
#define booltype 1
#define chartype 2
#define floattype 3
#define voidtype 4
#define notype -1


union Constant{
    int intval;
    char* strval;
};

struct ASTNode{
 // struct Typetable *type;           //pointer to the type table entry
  int type;                         
  int nodetype;                     //node type information,eg : NODETYPE_WHILE,NODETYPE_PLUS,NODETYPE_STMT etc 
  char *name;                       //stores the variable/function name in case of variable/function nodes
  int value;             //stores the value of the constant if the node corresponds to a constant
  struct ASTNode *arglist;          //pointer to the expression list given as arguments to a function call
  struct ASTNode *ptr1,*ptr2,*ptr3; //Subtrees of the node. (Maximum Subtrees for IF THEN ELSE)
 // struct Gsymbol *Gentry;           //pointer to GST entry for global variables and functions
  struct Lsymbol *Lentry;           //pointer to the function's LST for local variables and arguments
};

struct ASTNode *TreeCreate( int type, 
                            int nodetype, 
                            int value, 
                            char *name, 
                            struct ASTNode *arglist, 
                            struct ASTNode *ptr1, 
                            struct ASTNode *ptr2, 
                            struct ASTNode *ptr3 );

void PrintPreorder(struct ASTNode* node);                           

#endif