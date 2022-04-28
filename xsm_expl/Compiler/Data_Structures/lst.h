#ifndef LST_TREE_H
#define LST_TREE_H

struct Lsymbol{
  char *name;               //name of the variable
  // struct Typetable *type;   //pointer to the Typetable entry of variable type
  int binding;              //stores memory address allocated to the variable 
  struct Lsymbol *next;     //points to the next Local Symbol Table entry
} *HEAD;

struct Lsymbol* LInstall(char *name); 

struct Lsymbol* LLookup(char *name); 

void PrintLsymbol();

int LStat();

#endif