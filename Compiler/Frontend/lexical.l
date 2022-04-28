%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <limits.h>
    #include "../Data_Structures/ast.h"
    #include "../Data_Structures/lst.h"
    
    
    int yywrap();
    int number;
%}

%%

"FUN"       { return FUN; }
"If"        { return IF; }
"if"        { return IF; }
"else"      { return ELSE;}
"do"        { return DO;}
"while"     { return WHILE;}
"read"      { return READ;}
"write"     { return WRITE;}
"return"    { return RETURN;}
"argc"      { return ARGC;}

"<"         {return LT;}        
">"         {return GT;}
"<="        {return LTE;}
">="        {return GTE;}
"=="        {return EQ;}
"!="        {return NTE;}

"+"         {return ADD;}         
"-"         {return SUB;}
"*"         {return MUL;}
"/"         {return DIV;}

"{"         {return *yytext;}
"}"         {return *yytext;}
"["         {return *yytext;}
"]"         {return *yytext;}
"("         {return *yytext;}
")"         {return *yytext;}

"="         {return *yytext;}
";"         {return *yytext;}

[a-zA-Z_][a-zA-Z0-9_]*  {

                        yylval.treenode = TreeCreate(inttype, VAR_NODE, -111110, yytext, NULL, NULL, NULL, NULL);
                       
                        return VAR;
                        }

[0-9]*                  { number = atoi(yytext);
                        yylval.treenode = TreeCreate(inttype, CONST_NODE, number, NULL, NULL, NULL, NULL, NULL);
                        return NUM;
                        }
[\r\n]                  {}
[\t]                    {}


%%

int yywrap() 
{return 1;}

