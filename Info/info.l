%{
#include <stdlib.h>
#include "info_type.h"
#include "info.tab.h"

extern void yyerror(char *);
%}

%option noyywrap

/* ������ ��ע�Ϳ��Զ���д�� */

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
										} /* �������ʽ�����пո񡣶����� ����ģʽ�� ��ע�Ͳ��ܶ���д�� */

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


[ \t]			{}/* ȥ���ո� */
.				yyerror("��Ч�ַ�");


%%
