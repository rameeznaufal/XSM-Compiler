#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "lst.h"
#include "code_gen.h"

int REG_NO = 0, LABEL_NO = 0;

int getLabel(){
    return LABEL_NO++;
}

int getReg() {
   if((REG_NO) == 20) {
       perror("ERROR: All registers are in use! \n");
       exit(1);
   } else {
       return REG_NO++;
   }
}

void freeReg() {
    if(REG_NO == 0) {
        return;
    } else {
        --REG_NO;
    }
}

int codeGen(struct ASTNode *t, FILE *targetfile) {
    if(targetfile == NULL || ferror(targetfile)) {
        perror("Error! \n");
        exit(1);
    }

    if(t == NULL) {
        return -1;
    }

    int i, j;
    int label_1, label_2;

    if(t->nodetype == READ_NODE) {
        fprintf(targetfile, "MOV R0, 7\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "MOV R0, -1\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "PUSH BP\n");
        fprintf(targetfile, "MOV R0, 0\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "INT 6\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R1\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R0\n");
        return -1;
    }

    if(t->nodetype == WRITE_NODE) {
        fprintf(targetfile, "MOV R0, 5\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "MOV R0, -2\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "MOV R0, [BP]\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "MOV R0, 0\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "PUSH R0\n");
        fprintf(targetfile, "INT 7\n");
        fprintf(targetfile, "POP R1\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R0\n");
        fprintf(targetfile, "POP R0\n");
        return -1;
    }

    if(t->nodetype == VAR_NODE) {
        i = getReg();
        fprintf(targetfile, "MOV R%d, BP\n", i);
        fprintf(targetfile, "ADD R%d, %d\n", i, t->Lentry->binding);
       // printf("MOV R%d, %d\n", i, t->value);
        return i;
    }

    if(t->nodetype == CONST_NODE) {
        i = getReg();
        fprintf(targetfile, "MOV R%d, %d\n", i, t->value);
       // printf("MOV R%d, %d\n", i, t->value);
        return i;
    }

    if(t->nodetype == ARGC_NODE) {
        i = getReg();
        fprintf(targetfile, "MOV R%d, BP\n", i);
        fprintf(targetfile, "SUB R%d, 3\n", i);
       // printf("MOV R%d, %d\n", i, t->value);
        return i;
    }
    
    
    if(t->nodetype == MAIN_NODE) {
        fprintf(targetfile, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4096\nMOV BP, 4096\n");
        codeGen(t->ptr1, targetfile);
        fprintf(targetfile, "MOV R1, [R1]\n");
        
        int reg_count = REG_NO;
        
        while(reg_count > 0) {
            fprintf(targetfile, "PUSH R%d\n",reg_count-1);
            reg_count--;
        }

        fprintf(targetfile, "PUSH R1\nPUSH R0\nCALL FUN \n");
        int k = getReg();
        fprintf(targetfile, "POP R%d\nPOP R%d\n", k, (k+1)%20);

         while(reg_count<(REG_NO - 1)) {
            fprintf(targetfile, "POP R%d\n",reg_count);
            reg_count++;
        }

        fprintf(targetfile, "MOV R1, BP\nMOV [R1], R%d\n", k);
        freeReg();
        codeGen(t->ptr3, targetfile);
        fprintf(targetfile, "INT 10\nFUN:\nPUSH BP\nMOV BP, SP\n");
        int num_of_loc_v = LStat();
         while(num_of_loc_v) {
              fprintf(targetfile, "PUSH R0\n");
              --num_of_loc_v;
         }  
    }

   

    if(t->nodetype == FUNC_NODE || t->nodetype == MAIN_NODE ) {
        
        j = codeGen(t->ptr2, targetfile);
    }

    if((t->nodetype == FUNC_NODE) && (t->ptr2 == NULL)) {
        j = codeGen(t->arglist, targetfile);
        int reg_count = REG_NO - 1;
        
        while(reg_count > 0) {
            fprintf(targetfile, "PUSH R%d\n",reg_count-1);
            reg_count--;
        }

        fprintf(targetfile, "PUSH R%d\nPUSH R0\nCALL FUN \n",j);
        freeReg();
        int k = getReg();
        fprintf(targetfile, "POP R%d\nPOP R%d\n",k, (k+1)%20);

        while(reg_count<(REG_NO - 1)) {
            fprintf(targetfile, "POP R%d\n",reg_count);
            reg_count++;
        }
        return k;
    }
    
    if(t->nodetype != MAIN_NODE)
         i = codeGen(t->ptr1, targetfile);

    if(t->nodetype == RET_NODE) {
         j = getReg();
         fprintf(targetfile, "MOV R%d, BP\n",j);
         fprintf(targetfile, "SUB R%d, 2\n",j);

         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
      
         fprintf(targetfile, "MOV [R%d], R%d\n",j, i);  //assigning ret value
       
         int num_of_loc_v = LStat();
         while(num_of_loc_v) {
              fprintf(targetfile, "POP R0\n");
              --num_of_loc_v;
         } 
         freeReg();
         freeReg();
         fprintf(targetfile, "MOV BP, [BP]\n");
         fprintf(targetfile, "POP R0\n");
         fprintf(targetfile, "RET\n");
         return -1;
    }

    if(t->nodetype == IFELSE_NODE ) {
        label_1 = getLabel();
        label_2 = getLabel();
        fprintf(targetfile, "JZ R%d, L%d \n", i, label_1);
        j = codeGen(t->ptr2, targetfile);
        fprintf(targetfile, "JMP L%d \n", label_2);
        fprintf(targetfile, "L%d:\n", label_1);
    }

    if(t->nodetype != MAIN_NODE)
        j = codeGen(t->ptr3, targetfile);
    
    if(t->nodetype == IFELSE_NODE )
        fprintf(targetfile, "L%d:\n", label_2);

    if(t->nodetype == IF_NODE ) {
        int label_1 = getLabel();
        fprintf(targetfile, "JZ R%d, L%d \n", i, label_1);
        j = codeGen(t->ptr2, targetfile);
        fprintf(targetfile, "L%d:\n", label_1);
    }

    if(t->nodetype == ASGN_NODE) {
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "MOV [R%d], R%d\n", i, j);
        freeReg();
        freeReg();
    }

    
    if(t->nodetype == ADD_NODE) {
       if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
       
        fprintf(targetfile, "ADD R%d, R%d\n", i, j);
       // printf("ADD R%d, %d\n", i, t->value);
        
        freeReg();
    }
    if(t->nodetype == SUB_NODE) {
        if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "SUB R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == MUL_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "MUL R%d, R%d\n", i, j);
       // printf("SUB R%d, %d\n", i, t->value);

        freeReg();
    }
    if(t->nodetype == DIV_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "DIV R%d, R%d\n", i, j);
        freeReg();
    }

    if(t->nodetype == GT_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "GT R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == LT_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "LT R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == GTE_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "GE R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == LTE_NODE) {
         if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "LE R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == EQ_NODE) {
        if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "EQ R%d, R%d\n", i, j);
        freeReg();
    }
    if(t->nodetype == NTE_NODE) {
        if(t->ptr1->nodetype == VAR_NODE || t->ptr1->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", i, i);
        if(t->ptr3->nodetype == VAR_NODE || t->ptr3->nodetype == ARGC_NODE)
            fprintf(targetfile, "MOV R%d, [R%d]\n", j, j);
        fprintf(targetfile, "NE R%d, R%d\n", i, j);
        freeReg();
    }

   

    if(t->nodetype == ASGN_NODE || t->nodetype == FUNC_NODE) {
        return -1;
    }

   
    return i;

}
