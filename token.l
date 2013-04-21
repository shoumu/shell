%{
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

//#define YY_INPUT(buff,yytext,50)

void yyerror(char *);
int yywrap();

//extern char * yytext;
%}
%%
[a-zA-Z0-9\-\.]+ {
	//yylval = yytext;
	yylval = strdup(yytext);
	//yylval = yytext;
	//printf("yylval: %s\nyytext: %s\n", yylval, yytext); 
	//printf("yytext: %s\n", yytext);
	return STRING;
	}
[<>&|]					{return yytext[0];}
[ \t]+					; 
[\n]					{ return yytext[0];}
.						{yyerror("Invalid character");}

%%

int yywrap()
{
	return 1;
}
