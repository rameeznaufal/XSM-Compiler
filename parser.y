%{

   #include <stdio.h>
   #include <limits.h>
   #include <stdlib.h>
   

   #include "ast.h"
   #include "lst.h"
   #include "code_gen.h"

   int yylex();
   void yyerror(char const *s);
%}

%union
{
    struct ASTNode *treenode;
}

%start Program

%token FUN IF ELSE DO WHILE READ WRITE RETURN ARGC VAR NUM
%token ADD SUB MUL DIV
%token LT GT LTE GTE EQ NTE

%type <treenode> Program WriteStmt ReadStmt MainFunc Stmts Stmt AssgnStmt IfStmt IfElseStmt RetStmt E ARGC VAR NUM 

%left EQ NTE
%left GTE LTE GT LT
%left ADD SUB
%left MUL DIV

%%

Program : ReadStmt  
          MainFunc
          WriteStmt {$$ = TreeCreate(notype, MAIN_NODE, -1, NULL, NULL, $1, $2, $3);
                     struct ASTNode *temp;
                     temp = $$;
                    // printf("\n");
                    // PrintPreorder(temp);
                    // printf("\n-- AST Generated successfully --\n");
                    // PrintLsymbol();
                    // printf("\n -- LST Generated successfully --\n");
                     FILE *fptr;
                     fptr = fopen("target_code.txt","w");
                     codeGen(temp, fptr);
                     fclose(fptr);
                    // printf("\n -- Target Code Generated successfully at Target_Files/ --\n");
                     }
          ; 

WriteStmt  : WRITE '(' ARGC ')' ';' {$$ = TreeCreate(notype, WRITE_NODE, -1, NULL, NULL, $3, NULL, NULL);}
           ;

ReadStmt : READ '(' ARGC ')' ';' {$$ = TreeCreate(notype, READ_NODE, -1, NULL, NULL, $3, NULL, NULL);}
        ;
MainFunc  : ARGC '=' FUN '(' ARGC ')' '{' Stmts '}' {$$ = TreeCreate(notype, FUNC_NODE, -1, NULL, $5, $1, $8, NULL);}
          ;
        
Stmts   : Stmts Stmt {$$ = TreeCreate(notype, STMTS_NODE, -1, NULL, NULL, $1, NULL, $2);}
          | Stmt {$$ = $1;}
        ;

Stmt    : AssgnStmt {$$ = $1;}
          | IfStmt {$$ = $1;}
          | IfElseStmt {$$ = $1;}
          | RetStmt {$$ = $1;}
        ;
AssgnStmt: VAR '=' E ';' {
                          $$ = TreeCreate($3->type, ASGN_NODE, -1, NULL, NULL, $1, NULL, $3);
                          /* Variables have local scope in FUN*/    
                          if ($3->type != inttype)
                           {
                              perror("\nERROR:type mismatch!\n");
                              exit(1);
                           } 

                           if(LLookup($1->name) == NULL)
                            LInstall($1->name);
                           $1->Lentry = LLookup($1->name);
                           
                           $$->ptr1->type = $3->type;
                         }
                             
         ;
IfStmt  : IF '(' E ')' '{' Stmts '}' {$$ = TreeCreate(notype, IF_NODE, -1, NULL, NULL, $3, $6, NULL);}
        ;
IfElseStmt  : IF '(' E ')' '{' Stmts '}' ELSE '{' Stmts '}' {$$ = TreeCreate(notype, IFELSE_NODE, -1, NULL, NULL, $3, $6, $10);}
            ;
RetStmt    : RETURN E ';' {$$ = TreeCreate(notype, RET_NODE, -1, NULL, NULL, $2, NULL, NULL);}
            ;
E   : E ADD E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(inttype, ADD_NODE, -1, NULL, NULL, $1, NULL, $3);} 
    | E MUL E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(inttype, MUL_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E SUB E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(inttype, SUB_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E DIV E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(inttype, DIV_NODE, -1, NULL, NULL, $1, NULL, $3);}           //assuming division is floor division
    | E LTE E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(booltype, LTE_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E GT E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
             $$ = TreeCreate(booltype, GT_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E LT E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
              exit(1);
                 } 
              $$ = TreeCreate(booltype, LT_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E GTE E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(booltype, GTE_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E EQ E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
              $$ = TreeCreate(booltype, EQ_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | E NTE E {if (($1->type != inttype) || ($3->type != inttype))
                 {
                    perror("\nERROR:type mismatch!\n");
                    exit(1);
                 } 
               $$ = TreeCreate(booltype, NTE_NODE, -1, NULL, NULL, $1, NULL, $3);}
    | '(' E ')' {$$ = $2;}
    | NUM {$$ = $1;}
    | VAR  {$$ = $1;
            if ($1->nodetype == VAR_NODE) {
                    if(LLookup($1->name) == NULL)
                    { printf("\nERROR: Variable %s not Defined\n",$1->name);
                      exit(1);}
                    $1->Lentry = LLookup($1->name);
               } 
            }
    | ARGC {$$ = TreeCreate(inttype , ARGC_NODE, -1, NULL, NULL, NULL, NULL, NULL);}
    | FUN '(' E ')' {$$ = TreeCreate(inttype, FUNC_NODE, -1, NULL, $3, NULL, NULL, NULL);}
    ;
 

%%

void yyerror(char const *s)
{
    printf("yyerror  %s\n",s);
    return ;
}

int main() {
    yyparse();
    return 0;
}