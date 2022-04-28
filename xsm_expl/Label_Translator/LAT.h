#ifndef L_ADDRESS_T_H
#define L_ADDRESS_T_H

struct LAT{
  char *lab;               //label value
  int address;             //stores the mapped address location 
  struct LAT *next;     //points to the next Label-Address table entry
} *HEAD;

struct LAT* LInsert(char *lab, int address); 

struct LAT* LLookup(char *lab); 

void PrintLAT();

#endif