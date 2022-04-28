#ifndef CODE_GEN_H
#define CODE_GEN_H


int REG_NO,LABEL_NO;

int getReg();
void freeReg();

int codeGen(struct ASTNode *t, FILE *targetfile);

#endif