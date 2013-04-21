%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"


#define YYSTYPE char*
extern YYSTYPE yylval;

int yylex();
void yyerror(char *);


extern char* yytext;
%}
%token STRING
%%
line		: {return 0;}
			| command '\n' 
			{
			after_parse();
			//print_test();
			return 0;
			}
			| '\n' {return 0;}
			;
command		:fgcommand 
			{ 
			}
			|fgcommand '&'
			{
				make_command(0,NULL);
			}
			;
fgcommand	:simple_cmd next_cmd
			;
next_cmd	:
			| '|' fgcommand
			;

simple_cmd	:prog_invocation input_redirect output_redirect
		   {
		   push_command();
		   }
		   ;
prog_invocation:STRING args
			   {
			   make_command(4,$1);
			   }
			   ;
input_redirect:
			  |'<' STRING
			  {
			  make_command(2,$2);
			  }
			  ;
output_redirect:
			   |'>' STRING
			   {
			   make_command(3,$2);
			   }
			   ;
args:{ /*printf("empty args\n");*/}
	| args STRING
	{
	make_command(1,$2);
	}
	;
%%
void yyerror(char *s)
{
	if(s)
	{
		printf("%s\n",s);
	}
	else
	{
		printf("输入的命令不正确，请重新输入");
	}
}

int main()
{
	int i;
	
	// initialize the environment
	init();

	while(1) 
	{
		printf("shoumu-sh@localhost:%s> ", get_current_dir_name());
		before_parse();
		yyparse();
		execute();
	}
	
	return 0;
}
