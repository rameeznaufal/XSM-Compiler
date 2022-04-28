%{
    #include <stdio.h>
    #include <limits.h>
    #include <ctype.h>
    #include "LAT.h"
    
    int address = 2056, PASS = 1, Lcount = 0;
    char line[100];
    FILE *temp;
    int yywrap();
%}

%%

L[0-9]+:  {
           LInsert(yytext, address - Lcount);
           Lcount += 2;
          }

"FUN:"  {
           LInsert(yytext, address - Lcount);
           Lcount += 2;
          }

"FUN"   {
           if(PASS == 2) {
              struct LAT *add = LLookup(strcat(yytext,":"));
              fprintf(temp, " %d\n", add->address);
           }
          }

L[0-9]+   {
           if(PASS == 2) {
              struct LAT *add = LLookup(strcat(yytext,":"));
              fprintf(temp, " %d\n", add->address);
           }
          }

0\n2056\n0\n0\n0\n0\n0\n0\n   {
  if(PASS == 2) {
              fprintf(temp, "0\n2056\n0\n0\n0\n0\n0\n0\n");
           }
}

R[0-9]+,*  {
                    if(PASS == 2) {fprintf(temp, " %s", yytext);}
                   }

\[R[0-9]+\],*  {
                    if(PASS == 2) {fprintf(temp, " %s", yytext);}
                   }

\[BP\],*        {
             if(PASS == 2) {fprintf(temp, " %s", yytext);}
            }

BP,*        {
             if(PASS == 2) {fprintf(temp, " %s", yytext);}
            }

SP,*        {
             if(PASS == 2) {fprintf(temp, " %s", yytext);}
            }

-?[0-9]+    {
                if(PASS == 2) {fprintf(temp, " %s", yytext);}
              }

"MOV"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"ADD"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"SUB"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"DIV"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"MUL"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

"LT"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"GT"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"EQ"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"NE"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"GE"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"LE"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

"JZ"        { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"JNZ"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"JMP"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

"PUSH"      { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"POP"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

"CALL"      { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"RET"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}
"INT"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

"BRKP"       { if(PASS == 2) {fprintf(temp, "%s", yytext);}}

[\n]       {if(PASS == 2) {fprintf(temp, "%s", yytext);}
            address+=2;
           }

%%

int yywrap() 
{   if(PASS == 1) {
       fseek(yyin, 0, SEEK_SET);

       while(yyin && fscanf(yyin, "%[^\n]%*c", line) != EOF)
        {if((line[0] == 'L' && isdigit(line[1])) || (strcmp(line, "FUN:") == 0 )) 
            continue;
         fprintf(temp, "%s\n", line);}
       
       fclose(temp);
       fclose(yyin);

       remove("target_code.txt");
       rename("temp.txt", "target_code.txt" );
       
       yyin = fopen("target_code.txt","r");
       temp = fopen("temp.txt","w");
       PASS = 2;
       return 0; 
      }

    fclose(temp);
    fclose(yyin);

    remove("target_code.txt");
    rename("temp.txt", "B190785CS.xsm");
       
    return 1;
    }

 int main() {
    yyin = fopen("target_code.txt","r");
    temp = fopen("temp.txt","w");

    if(yyin == NULL) {
        perror("Error: No target file generated!\n");
        exit(1);
    }
    yylex();
    return 0;

}

