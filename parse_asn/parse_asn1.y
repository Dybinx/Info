%{
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "IEItemDesp.h"
#include "parse_asn1.tab.h"

/*
//get all bison DEBUG info.
#define YYDEBUG 1
int yydebug = 1;
*/

//get detail error info
#define YYERROR_VERBOSE 1

extern int line;
extern int column;

extern void CreateContentItem(char *content, char *value);
extern void AddIEItemNode(IEItem *ie);
extern int yylex(void);
void yyerror(char *);

%}

/* ͨ��union��ʾ��������ֵ����֮�󣬲���ʹ��Ĭ�ϵ�int���ͣ����ҵ���ʹ��%unionָ������ֵ���͵�ʱ��, �����Ϊÿ��Ҫʹ��������ֵ�ķ��ս��ָ��һ��ֵ���͡�
   ����ս��������͹����еĵ�һ�����ŵ�����ֵ������ͬ�Ļ������û��ָ��action����ôĬ��$$ = $1��*/
%union{
	char *chPtr;			/* �������Ԫ�ص����� */

	IEItem *structPtr;	/* ����ṹ��ָ�� */
	BASE_TYPE_E type;		/* ����baseType */
	IE_ATTRI_E  attri;		/* ����ie������ */
	IERange *ieRangPtr;
	NEED_TYPE_E needType;
}

%token NEED_TOKEN COND_TOKEN DEFAULT_TOKNE SIZE_TOKEN
%token <needType> OP_TOKEN OR_TOKEN ON_TOKEN
%token <attri> OPTION_TOKEN
%token <type> SEQUENCE_TOKEN ENUMERATED_TOKEN CHOICE_TOKEN INTEGER_TOKEN OCTET_STRING_TOKEN BIT_STRING_TOKEN 
				CONTAINING_TOKEN EXTENSION_TAG_TOKEN BOOLEAN_TOKEN NULL_TOKEN SETUP_RELEASE_TOKEN

/* ���еķ��ս�����ս��������ֵ�������ַ�ָ�롣 */
%token <chPtr> LFIRSTCASETERM UFIRSTCASETERM DIGIT BITVALUE_TOKEN
%type <type> string
%type <needType> needTypeTag needTypeWord
%type <chPtr> typeName  variableName condTag defaultValue
%type <structPtr> specialTpyeValue IEItem  IEItemForString contrain program typeIE
%type <type> complexType singleType
%type <ieRangPtr> sizeValue size
%right <type> ASSIGN_TOKEN
%right COMMA_TOKEN
%right DASH_TOKEN
%right OF_TOKEN
%right LPARENTHESIS_TOKEN
%left RPARENTHESIS_TOKEN
%right LBRACKET_TOKEN
%left  RBRACKET_TOKEN
%right <type> LDBRACKET_TOKEN
%left  RDBRACKET_TOKEN
%left ELLIPSIS_TOKEN
%right LEFTBRACE_TOKEN
%left RIGHTBRACE_TOKEN

%start program 


%%

/* ��ʾ����������ͬʱ����0������typeName������PHICH-Config��*/
program :	
			| program typeIE
			 {
				AddIEItemNode($2);
			 }
;

/* ÿ��typeName����ʽ�������ʾ�� */
typeIE:		   typeName ASSIGN_TOKEN specialTpyeValue
			   {
					IEItem *ie;
					char *name;

					ie = CreateIEItem();
					SetIEItemTypeName(ie, $1);
					SetIEItemType(ie, $2);
					SetSubItem(ie, $3);

					name = malloc(strlen($1) + 1);
					if(name == NULL)
					{
						printf("fail to malloc for name of parent of typeName: %s", $1);
						while(1){}
					}
					strcpy(name, $1);
					SetIEItemTypeName($3, name);

					$$ = ie;
			   }
			 | typeName ASSIGN_TOKEN typeName
			   {
					IEItem *ie1, *ie2;

					ie1 = CreateIEItem();
					SetIEItemTypeName(ie1, $1);
					SetIEItemType(ie1, $2);

					ie2 = CreateIEItem();
					SetIEItemTypeName(ie2, $3);
					SetIEItemType(ie2, $2);

					SetSubItem(ie1, ie2);
					
					$$ = ie1;
			   }
			 | variableName INTEGER_TOKEN ASSIGN_TOKEN DIGIT
			   {
					printf("%s\t\tINTEGER ::= %s\n", $1, $4);
					CreateContentItem($1, $4);
					$$ = NULL;
			   }
			 | SETUP_RELEASE_TOKEN LEFTBRACE_TOKEN  typeName RIGHTBRACE_TOKEN ASSIGN_TOKEN CHOICE_TOKEN LEFTBRACE_TOKEN IEItem RIGHTBRACE_TOKEN
			   {
					//���Ե�����ṹ������
					$$ = NULL;
			   }
;

specialTpyeValue:	  complexType LEFTBRACE_TOKEN RIGHTBRACE_TOKEN
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, BASE_TYPE_NULL); //considered as NULL
 					  }
					| complexType LEFTBRACE_TOKEN IEItem RIGHTBRACE_TOKEN
					 {
						$$ = CreateIEItem();
						SetSubItem($$, $3);
						SetIEItemType($$, $1);
					 }
					| singleType
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
					  }
					| INTEGER_TOKEN
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
					  }
					| INTEGER_TOKEN LPARENTHESIS_TOKEN sizeValue RPARENTHESIS_TOKEN
					 {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetIEItemArray($$, $3);
					 }
					| complexType LPARENTHESIS_TOKEN size RPARENTHESIS_TOKEN OF_TOKEN specialTpyeValue
					 {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetIEItemArray($$, $3);
						SetSubItem($$, $6);
					 }
					| complexType LPARENTHESIS_TOKEN size RPARENTHESIS_TOKEN OF_TOKEN typeName
					  {
						IEItem *ie;
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetIEItemArray($$, $3);

						ie = CreateIEItem();
						SetIEItemTypeName(ie, $6);

						SetSubItem($$, ie);
					  }
					| string
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
					  }
					| string LPARENTHESIS_TOKEN contrain RPARENTHESIS_TOKEN
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetSubItem($$, $3);
					  }
					| string LPARENTHESIS_TOKEN size RPARENTHESIS_TOKEN
					  {
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetIEItemArray($$, $3);
					  }
					| SETUP_RELEASE_TOKEN LEFTBRACE_TOKEN typeName RIGHTBRACE_TOKEN 
					  {
						$$ = CreateSetupRelease($3);
					  }
;

contrain:			CONTAINING_TOKEN typeName
					{
						$$ = CreateIEItem();
						SetIEItemType($$, $1);
						SetIEItemTypeName($$, $2);
					}
;

size:				 SIZE_TOKEN LPARENTHESIS_TOKEN sizeValue RPARENTHESIS_TOKEN
					 {
						$$ = $3;
					 }
;

/* ÿ��IEItem��Ӧһ��baseType������Ӧһ��specialTpyeValue�� 
   ÿ��IEItem���������ݿ��������¼������֮һ��
   1. ���� ����
   2. ���� �������� {����}
   3. ���� ���� OPTIONAL 
   4. ���� �������� {����} OPTIONAL 
   5. ���� 
   6. ����..����
   7. ����..����
   */

IEItemForString:	  variableName string
					  {
						$$ = CreateIEItem();
						SetIEItemVariableName($$, $1);
						SetIEItemType($$, $2);
					  }
					| variableName string LPARENTHESIS_TOKEN size RPARENTHESIS_TOKEN
					  {
						$$ = CreateIEItem();
						SetIEItemVariableName($$, $1);
						SetIEItemType($$, $2);
						SetIEItemArray($$, $4);
					  }
;

IEItem:		  LDBRACKET_TOKEN IEItem RDBRACKET_TOKEN
			  {
				$$ = CreateIEItem();
				SetIEItemType($$, $1);
				SetSubItem($$, $2);
			  }
			| IEItemForString
			  {
				$$ = $1;
			  }
			| IEItemForString OPTION_TOKEN
			  {
				$$ = $1;
				SetIEItemAttri($$, $2);
			  }
			| variableName
			  {
				$$ = CreateIEItem();
				SetIEItemVariableName($$, $1);
			  }
			| variableName typeName
			  {
			  @$ = @1;
				$$ = CreateIEItem();
				SetIEItemVariableName($$, $1);
				SetIEItemTypeName($$, $2);
			  }
			| variableName typeName OPTION_TOKEN 
			  {
				$$ = CreateIEItem();
				SetIEItemVariableName($$, $1);
				SetIEItemTypeName($$, $2);
				SetIEItemAttri($$, $3);
			  }
			| variableName singleType 
			  {
				$$ = CreateIEItem();
				SetIEItemVariableName($$, $1);
				SetIEItemType($$, $2);
			  }
			| variableName singleType OPTION_TOKEN
			  {
				$$ = CreateIEItem();
				SetIEItemVariableName($$, $1);
				SetIEItemType($$, $2);
				SetIEItemAttri($$, $3);
			  }
			| variableName specialTpyeValue
			  {
				SetIEItemVariableName($2, $1);
				$$ = $2;
			  }
			| variableName specialTpyeValue OPTION_TOKEN
			 {
				SetIEItemVariableName($2, $1);
				SetIEItemAttri($2, $3);
				$$ = $2;
			 }
			| IEItem COMMA_TOKEN IEItem
			  {
				/* �γ����飬*/
				$$ = LinkIEItem($1, $3);
			  }
			| IEItem COMMA_TOKEN needTypeTag IEItem
			  {
				SetIEItemNeedType($1, $3);
				$$ = LinkIEItem($1, $4);
			  }
			| IEItem needTypeTag		/* last item. */
			  {
				SetIEItemNeedType($1, $2);
				$$ = $1;
			  }
			| IEItem condTag			/* last item. */
			  {
				SetIEItemCondTag($1, $2);
				$$ = $1;
			  }
			| IEItem COMMA_TOKEN condTag IEItem
			  {
				SetIEItemCondTag($1, $3);
				$$ = LinkIEItem($1, $4);
			  }
			| EXTENSION_TAG_TOKEN
			  {
				$$ = CreateIEItem();
				SetIEItemType($$, $1);
			  }
			| IEItem COMMA_TOKEN condTag EXTENSION_TAG_TOKEN COMMA_TOKEN IEItem
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $4);
				SetSubItem(ie, $6);

				SetIEItemCondTag($1, $3);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem COMMA_TOKEN needTypeTag EXTENSION_TAG_TOKEN COMMA_TOKEN IEItem
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $4);
				SetSubItem(ie, $6);

				SetIEItemNeedType($1, $3);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem COMMA_TOKEN EXTENSION_TAG_TOKEN COMMA_TOKEN IEItem
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $3);
				SetSubItem(ie, $5);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem COMMA_TOKEN EXTENSION_TAG_TOKEN
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $3);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem COMMA_TOKEN needTypeTag EXTENSION_TAG_TOKEN
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $4);

				SetIEItemNeedType($1, $3);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem COMMA_TOKEN condTag EXTENSION_TAG_TOKEN
			  {
				IEItem *ie;

				ie = CreateIEItem();
				SetIEItemType(ie, $4);

				SetIEItemCondTag($1, $3);

				$$ = LinkIEItem($1, ie);
			  }
			| IEItem DEFAULT_TOKNE defaultValue
			  {
				SetIEItemDefaultValue($1, $3);
				$$ = $1;
			  }
;

defaultValue:     LFIRSTCASETERM
				| DIGIT
				| BITVALUE_TOKEN

condTag:	  DASH_TOKEN COND_TOKEN UFIRSTCASETERM
			  {
				$$ = $3;
			  }
			| DASH_TOKEN COND_TOKEN LFIRSTCASETERM
			  {
				$$ = $3;
			  }

needTypeTag: DASH_TOKEN NEED_TOKEN needTypeWord
			 {
				$$ = $3;
			 }
;

needTypeWord:	  OR_TOKEN
				| ON_TOKEN
				| OP_TOKEN
;

/* eg: 1..100; 1..maxSIB */
sizeValue:    DIGIT
			  {
				$$ = CreateArray($1, NULL);
			  }
			| LFIRSTCASETERM
			  {
				$$ = CreateArray($1, NULL);
			  }
			| LFIRSTCASETERM ELLIPSIS_TOKEN LFIRSTCASETERM
			  {
				$$ = CreateArray($1, $3);
			  }
			| DIGIT ELLIPSIS_TOKEN DIGIT
			  {
				$$ = CreateArray($1, $3);
			  }
			| DIGIT ELLIPSIS_TOKEN LFIRSTCASETERM
			  {
				$$ = CreateArray($1, $3);
			  }
;

typeName: UFIRSTCASETERM { $$ = $1; }
;

variableName: LFIRSTCASETERM { $$ = $1; }
;

complexType:	  SEQUENCE_TOKEN
				| ENUMERATED_TOKEN
				| CHOICE_TOKEN
;


string:		  OCTET_STRING_TOKEN 
			| BIT_STRING_TOKEN
;

singleType:	  BOOLEAN_TOKEN
			| NULL_TOKEN


%%

void yyerror(char *s) {
	char buf[1000] = {0};
	sprintf(buf, "%s,\nline: %d,  or around.", s, yylloc.first_line);
	printf("%s\n", buf);
}



