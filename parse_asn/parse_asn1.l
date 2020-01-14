
%{
#include <stdlib.h>
#include "IEItemDesp.h"
#include "parse_asn1.tab.h"

extern void HandleIEItems();
extern void DestroyIEItemNodes();
extern void DestroyContentItems();
extern void yyerror(char *);
void updatLocation(int textLen);

int comment_caller[2];
int stack_point = 0;
int column = 0;
int line = 1;


%}

/* 声明段 的注释可以顶格写。 */

%x comment messageSetType

MESSAGE_SET_TYPE NR-RRC
%%

	/* initial state */
{MESSAGE_SET_TYPE}-"Definitions DEFINITIONS AUTOMATIC TAGS ::="		{ 
																		comment_caller[stack_point] = YY_START;
																		stack_point++;
																		BEGIN(messageSetType); 
																		printf("%s\n\n", yytext);
																	}
"\n"					{ line++; column = 0; }
.						/* 省略掉所有非MESSAGE_SET_TYPE指定的数据 */;

<messageSetType>BEGIN				{ printf("%s\n\n", yytext); }
	/* 推出messageSetType状态 */
<messageSetType>END					{
										stack_point--;
										BEGIN(comment_caller[stack_point]); 
										printf("\n\n%s\n\n", yytext);
									}

<messageSetType>SetupRelease	{ yylval.type = BASE_TYPE_CHOICE; updatLocation(yyleng); return SETUP_RELEASE_TOKEN; }
<messageSetType>NULL			{ yylval.type = BASE_TYPE_NULL; updatLocation(yyleng); return NULL_TOKEN; }
<messageSetType>BOOLEAN			{ yylval.type = BASE_TYPE_BOOLEAN; updatLocation(yyleng); return BOOLEAN_TOKEN; }
<messageSetType>SEQUENCE		{ yylval.type = BASE_TYPE_SEQUENCE; updatLocation(yyleng); return SEQUENCE_TOKEN; }
<messageSetType>ENUMERATED		{ yylval.type = BASE_TYPE_ENUMERATED; updatLocation(yyleng); return ENUMERATED_TOKEN; }
<messageSetType>CHOICE			{ yylval.type = BASE_TYPE_CHOICE; updatLocation(yyleng); return CHOICE_TOKEN; }
<messageSetType>OPTIONAL		{ yylval.attri = IE_ATTRI_OPTION; updatLocation(yyleng); return OPTION_TOKEN; }
<messageSetType>CONTAINING		{ yylval.type = BASE_TYPE_CONTAINING; updatLocation(yyleng); return CONTAINING_TOKEN; }
<messageSetType>INTEGER			{ yylval.type = BASE_TYPE_INTEGER; updatLocation(yyleng); return INTEGER_TOKEN; }
<messageSetType>"OCTET STRING"	{ yylval.type = BASE_TYPE_OCTET_STRING; updatLocation(yyleng); return OCTET_STRING_TOKEN; }
<messageSetType>"BIT STRING"	{ yylval.type = BASE_TYPE_BIT_STRING; updatLocation(yyleng); return BIT_STRING_TOKEN; }
<messageSetType>"..."			{ yylval.type = BASE_TYPE_EXTENSION; updatLocation(yyleng); return EXTENSION_TAG_TOKEN; }
<messageSetType>::=				{ yylval.type = BASE_TYPE_HEADER; updatLocation(yyleng); return ASSIGN_TOKEN; }

<messageSetType>SIZE			{ updatLocation(yyleng); return SIZE_TOKEN; }
<messageSetType>DEFAULT			{ updatLocation(yyleng); return DEFAULT_TOKNE; }
<messageSetType>OF				{ updatLocation(yyleng); return OF_TOKEN; }
<messageSetType>"{"				{ updatLocation(yyleng); return LEFTBRACE_TOKEN; }
<messageSetType>"}"				{ updatLocation(yyleng); return RIGHTBRACE_TOKEN; }
<messageSetType>,				{ updatLocation(yyleng); return COMMA_TOKEN; }
<messageSetType>"("				{ updatLocation(yyleng); return LPARENTHESIS_TOKEN; }
<messageSetType>")"				{ updatLocation(yyleng); return RPARENTHESIS_TOKEN; }
<messageSetType>"[["			{ yylval.type = BASE_TYPE_DBRACKET; updatLocation(yyleng); return LDBRACKET_TOKEN; }  //the action like SEQUENCE.
<messageSetType>"]]"			{ updatLocation(yyleng); return RDBRACKET_TOKEN; }
<messageSetType>"["				{ updatLocation(yyleng); return LBRACKET_TOKEN; }
<messageSetType>"]"				{ updatLocation(yyleng); return RBRACKET_TOKEN; }
<messageSetType>".."			{ updatLocation(yyleng); return ELLIPSIS_TOKEN; }
<messageSetType>"--"			{ 
									comment_caller[stack_point] = messageSetType;
									stack_point++;
									BEGIN(comment); 
								}


<messageSetType>'[01]+'B			{
										yylval.chPtr = malloc(yyleng + 1);
										memcpy(yylval.chPtr, yytext, yyleng);
										yylval.chPtr[yyleng] = '\0';
										updatLocation(yyleng);
										return BITVALUE_TOKEN;
									}

<messageSetType>[a-z]+(-[a-zA-Z0-9]|[a-zA-Z0-9])*		{ 
															yylval.chPtr = malloc(yyleng + 1);
															memcpy(yylval.chPtr, yytext, yyleng);
															yylval.chPtr[yyleng] = '\0';
															updatLocation(yyleng);
															return LFIRSTCASETERM; 
														} /* 正则表达式不能有空格。而且在 规则模式段 的注释不能顶格写。 */

<messageSetType>[A-Z]+(-[a-zA-Z0-9]|[a-zA-Z0-9])*		{ 
															yylval.chPtr = malloc(yyleng + 1);
															memcpy(yylval.chPtr, yytext, yyleng);
															yylval.chPtr[yyleng] = '\0';
															updatLocation(yyleng);
															return UFIRSTCASETERM; 
														}

<messageSetType>(-[0-9]|[0-9])+							{
															yylval.chPtr = malloc(yyleng + 1);
															memcpy(yylval.chPtr, yytext, yyleng);
															yylval.chPtr[yyleng] = '\0';
															updatLocation(yyleng);
															return DIGIT;
														}


<messageSetType>"\n"			{ line++; column = 0; }

<messageSetType>[ \t]			column++;/* 去除空格 */
<messageSetType>.				yyerror("无效字符");

	/* comment state, used for comments. */
<comment>.						/* 省略掉所有comment数据 */;
<comment>"\n"					{	
									stack_point--;
									BEGIN(comment_caller[stack_point]); 
									line++; 
									column = 0;
								}/* 这里是换行符, 同时推出comment状态，进入messageSetType状态。*/


%%

int yywrap(void) {
	HandleIEItems();
	DestroyIEItemNodes();
	DestroyContentItems();
	return 1;
}

void updatLocation(int textLen)
{
	yylloc.first_line = line;
	yylloc.first_column = column;

	yylloc.last_line = line;
	yylloc.last_column = column + textLen;
}