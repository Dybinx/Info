%{

#include <stdio.h>
#include <stdlib.h>

#include "info_type.h"
#include "info.h"
#include "info.tab.h"



extern FILE *yyin;


extern void yyerror(char *);
extern int yyparse(void);

%}


%union{
	char *chPtr;			
	Item *structPtr;		
}

%token <chPtr> LFIRSTCASETERM UFIRSTCASETERM DIGIT

%token SEQUENCE_TOKEN INTEGER_TOKEN ASSIGN_TOKEN

%type <chPtr> typeName  variableName
%type <structPtr> IEItem typeIE program

%right COMMA_TOKEN
%right LPARENTHESIS_TOKEN
%left RPARENTHESIS_TOKEN
%left RIGHTBRACE_TOKEN
%right LEFTBRACE_TOKEN

%left ELLIPSIS_TOKEN

%start program 

%%


program :	
			| program typeIE
			 {
				ParseItem($2);
			 }
;


typeIE:			typeName ASSIGN_TOKEN SEQUENCE_TOKEN LEFTBRACE_TOKEN IEItem RIGHTBRACE_TOKEN
				{
					$$ = $5;
				}
;



IEItem:		  variableName INTEGER_TOKEN LPARENTHESIS_TOKEN DIGIT ELLIPSIS_TOKEN DIGIT RPARENTHESIS_TOKEN
			  {
				$$ = CreateItem();
				SetItemVariableName($$, $1);
				SetItemRange($$, $4, $6);

			  }
			| IEItem COMMA_TOKEN IEItem
			  {
				/* 形成数组，*/
				$$ = LinkItem($1, $3);
			  }
;


typeName: UFIRSTCASETERM { $$ = $1; }
;

variableName: LFIRSTCASETERM { $$ = $1; }
;


%%

void yyerror(char *s) {
	printf("error: %s\n", s);
}


int main(int argc, char* argv[])
{

	yyin = fopen(argv[1], "r");

    yyparse();

    return 0;

}
