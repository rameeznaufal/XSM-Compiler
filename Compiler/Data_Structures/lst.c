#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lst.h"

struct Lsymbol *HEAD = NULL;

struct Lsymbol* LInstall(char *name) {

if(HEAD) {
    struct Lsymbol *ptr = HEAD, *temp;
    
    while(ptr->next) {    
        ptr = ptr->next;
    }

    temp = (struct Lsymbol*) malloc(sizeof(struct Lsymbol));
    
    temp->name = (char*) malloc(sizeof(char));
    strcpy(temp->name,name);
    
    temp->binding = ptr->binding + 1;

    temp->next = NULL;
    ptr->next = temp;
    return temp;
} else {

    HEAD = (struct Lsymbol*) malloc(sizeof(struct Lsymbol));
    HEAD->name = (char*) malloc(sizeof(char));
    strcpy(HEAD->name,name);
    HEAD->next = NULL;
    HEAD->binding = 1;
    return HEAD;
}
}

void PrintLsymbol() {
    struct Lsymbol *temp = HEAD;

    while(temp) {
        printf("%d-%s ", temp->binding, temp->name);
        temp = temp->next;
    }
}

struct Lsymbol* LLookup(char *name) {
    struct Lsymbol *temp = HEAD;

    while(temp) {
        if(strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

int LStat() {
    struct Lsymbol *temp = HEAD,*prev = NULL;

    while(temp) {
        prev = temp;
        temp = temp->next;
    }

    if(prev) {
        return prev->binding;
    }

    return 0;
}



