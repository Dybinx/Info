%{
#include <stdlib.h>
#include "info_type.h"
#include "info.tab.h"

extern void yyerror(char *);
%}

%option noyywrap

/* 声明段 的注释可以顶格写。 */

%%


SEQUENCE		{ return SEQUENCE_TOKEN; }
INTEGER			{ return INTEGER_TOKEN; }
::=				{ return ASSIGN_TOKEN; }

"{"				{ return LEFTBRACE_TOKEN; }
"}"				{ return RIGHTBRACE_TOKEN; }
,				{ return COMMA_TOKEN; }
"("				{ return LPARENTHESIS_TOKEN; }
")"				{ return RPARENTHESIS_TOKEN; }

".."			{ return ELLIPSIS_TOKEN; }





[a-z]+(-[a-zA-Z0-9]|[a-zA-Z0-9])*		{ 
											yylval.chPtr = malloc(yyleng + 1);
											memcpy(yylval.chPtr, yytext, yyleng);
											yylval.chPtr[yyleng] = '\0';
											return LFIRSTCASETERM; 
										} /* 正则表达式不能有空格。而且在 规则模式段 的注释不能顶格写。 */

[A-Z]+(-[a-zA-Z0-9]|[a-zA-Z0-9])*		{ 
											yylval.chPtr = malloc(yyleng + 1);
											memcpy(yylval.chPtr, yytext, yyleng);
											yylval.chPtr[yyleng] = '\0';
											return UFIRSTCASETERM; 
										}

(-[0-9]|[0-9])+							{
											yylval.chPtr = malloc(yyleng + 1);
											memcpy(yylval.chPtr, yytext, yyleng);
											yylval.chPtr[yyleng] = '\0';
											return DIGIT;
										}


[ \t]			{}/* 去除空格 */
.				yyerror("无效字符");


%%

