#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LAT.h"

struct LAT *HEAD = NULL;

struct LAT* LInsert(char *lab, int address) {

if(HEAD) {
    struct LAT *ptr = HEAD, *temp;
    
    while(ptr->next) {    
        ptr = ptr->next;
    }

    temp = (struct LAT*) malloc(sizeof(struct LAT));
    
    temp->lab = (char*) malloc(sizeof(char));
    strcpy(temp->lab,lab);

    temp->address = address;

    temp->next = NULL;
    ptr->next = temp;
    return temp;
} else {

    HEAD = (struct LAT*) malloc(sizeof(struct LAT));
    HEAD->lab = (char*) malloc(sizeof(char));
    strcpy(HEAD->lab,lab);
    HEAD->next = NULL;
    HEAD->address = address;
    return HEAD;
}
}

void PrintLsymbol() {
    struct LAT *temp = HEAD;

    while(temp) {
        printf("%s-%d ", temp->lab, temp->address);
        temp = temp->next;
    }
}

struct LAT* LLookup(char *lab) {
    struct LAT *temp = HEAD;

    while(temp) {
        if(strcmp(temp->lab, lab) == 0) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}


