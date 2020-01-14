#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IEItemDesp.h"
#include "utils_string.h"

ContentItem *gContentItem = NULL;
ContentItem *gCurrentContentItem = NULL;
IEItemNode	*gIEItemNodeHeader = NULL;
IEItemNode	*gCurrentIEItemNode = NULL;

extern void ParseCallback(IEItem *firstItem);
extern void yyerror(char *);

IEItem *CreateIEItem()
{
	IEItem *ieItem;
	char printBuff[100] = { 0 };

	//printf("CreateIEItem\n");

	ieItem = (IEItem *)malloc(sizeof(IEItem));

	if (ieItem == NULL)
	{
		sprintf(printBuff, "fail to malloc in file: %s, line: %d", __FILE__, __LINE__);
		yyerror(printBuff);
	}

	ieItem->IEVariableName = NULL;
	ieItem->IETypeName = NULL;
	ieItem->subIeItem = (void *)NULL;
	ieItem->nextIEItem = NULL;
	ieItem->sizeOf = NULL;
	ieItem->baseType = BASE_TYPE_max;
	ieItem->ieAttri = IE_ATTRI_MANDA;
	ieItem->needType = NEED_TYPE_max;
	ieItem->condTag = NULL;
	ieItem->defaultValue = NULL;
	ieItem->isPrinted = false;

	return ieItem;
}



void SetSubItem(IEItem *ie, IEItem *subIe)
{
	//printf("SetSubItem\n");
	ie->subIeItem = subIe;
}

void SetIEItemVariableName(IEItem *ie, char *IEVariableName)
{
	//printf("SetIEItemVariableName %s\n", IEVariableName);
	if (ie->IEVariableName == NULL)
	{
		ie->IEVariableName = IEVariableName;
	}
}

/* if want to set base type and type name, then set type name firstly, then 
   set type name. */
void SetIEItemTypeName(IEItem *ie, char *IETypeName)
{
	//printf("SetIEItemTypeName %s\n", IETypeName);
	if (ie->IETypeName == NULL)
	{
		ie->IETypeName = IETypeName;
	}
	
	if (ie->baseType == BASE_TYPE_max)
	{
		ie->baseType = BASE_TYPE_COMBINATION;
	}
	
}

void SetIEItemDefaultValue(IEItem *ie, char *defaultValue)
{
	ie->defaultValue = defaultValue;
}

void SetIEItemType(IEItem *ie, BASE_TYPE_E ieType)
{
	//printf("SetIEItemType %d\n", ieType);
	ie->baseType = ieType;
}

void SetIEItemCondTag(IEItem *ie, char *condTag)
{
	ie->condTag = condTag;
}

IEItem *CreateSetupRelease(char *IETypeName)
{
	IEItem *choiceIEItem;
	IEItem *setupIEItem;
	IEItem *releaseIEItem;
	char *setup, *release;
	char printBuff[100] = { 0 };

	choiceIEItem = CreateIEItem();
	setupIEItem = CreateIEItem();
	releaseIEItem = CreateIEItem();

	SetSubItem(choiceIEItem, releaseIEItem);
	LinkIEItem(releaseIEItem, setupIEItem);

	SetIEItemType(choiceIEItem, BASE_TYPE_CHOICE);

	setup = (char *)malloc(strlen("setup") + 1);
	if (setup == NULL)
	{
		sprintf(printBuff, "fail to malloc in file: %s, line: %d", __FILE__, __LINE__);
		yyerror(printBuff);
	}

	strcpy(setup, "setup");
	setup[strlen("setup")] = '\0';
	SetIEItemTypeName(setupIEItem, IETypeName);
	SetIEItemVariableName(setupIEItem, setup);

	release = (char *)malloc(strlen("release") + 1);
	if (release == NULL)
	{
		sprintf(printBuff, "fail to malloc in file: %s, line: %d", __FILE__, __LINE__);
		yyerror(printBuff);
	}

	strcpy(release, "release");
	release[strlen("release")] = '\0';
	SetIEItemType(releaseIEItem, BASE_TYPE_NULL);
	SetIEItemVariableName(releaseIEItem, release);

	return choiceIEItem;
}

IERange *CreateArray(char *startNum, char *endNum)
{
	IERange *ieRange;

	char printBuff[100] = { 0 };

	//printf("CreateArray\n");

	ieRange = (IERange *)malloc(sizeof(IERange));

	if (ieRange == NULL)
	{
		sprintf(printBuff, "fail to malloc in file: %s, line: %d", __FILE__, __LINE__);
		yyerror(printBuff);
	}

	ieRange->startNumber = startNum;
	ieRange->endNumber = endNum;

	return ieRange;
}

void SetIEItemArray(IEItem *ie, IERange *size)
{
	//printf("SetIEItemArray\n");
	ie->sizeOf = size;
}

void SetIEItemAttri(IEItem *ie, IE_ATTRI_E ieAttri)
{
	//printf("SetIEItemAttri\n");
	ie->ieAttri = ieAttri;
}

void SetIEItemNeedType(IEItem *ie, NEED_TYPE_E needType)
{
	ie->needType = needType;
}

//link the same type IEItems.
IEItem *LinkIEItem(IEItem *ie1, IEItem *ie2)
{
	IEItem *ieItem;

	ieItem = ie1;

	//printf("LinkIEItem\n");

	while (ie1->nextIEItem != NULL)
	{
		ie1 = ie1->nextIEItem;
	}
	
	ie1->nextIEItem = (void *)ie2;
	
	return ieItem;
}

//如果存在子item，那么将当前item分成前后两部分；
//如果不存在子item，则显示整个item内容。
void printIEItemAttriPre(IEItem *ie)
{
	IEItem *subIe;

	if (ie->IEVariableName != NULL)
	{
		printf("%s ", ie->IEVariableName);
	}

	switch (ie->baseType)
	{
	case BASE_TYPE_CONTAINING:
		printf(" CONTAINING ");
		break;
	}
		
	if (ie->IETypeName != NULL)
	{
		if (ie->IEVariableName != NULL)
		{
			printf("\t\t%s", ie->IETypeName);
		}
		else
		{
			printf("%s", ie->IETypeName);
		}
		
	}

	switch (ie->baseType)
	{
	case BASE_TYPE_SEQUENCE:
		if (ie->sizeOf != NULL)
		{
			printf(" SEQUENCE ");

			if (ie->sizeOf->endNumber == NULL)
			{
				printf("(SIZE (%s))", ie->sizeOf->startNumber);
			}
			else
			{
				printf("(SIZE (%s..%s))", ie->sizeOf->startNumber, ie->sizeOf->endNumber);
			}
		}
		else
		{
			printf(" SEQUENCE {");
		}
		
		break;
	case BASE_TYPE_DBRACKET:
		printf("[[");
		break;
	case BASE_TYPE_ENUMERATED:
		printf(" ENUMERATED {");
		break;
	case BASE_TYPE_CHOICE:
		printf(" CHOICE {");
		break;
	case BASE_TYPE_OCTET_STRING:
		subIe = ie->subIeItem;
		if (subIe == NULL)
		{
			printf(" OCTET STRING");
		}
		else
		{
			if (subIe->sizeOf != NULL ||
				subIe->baseType == BASE_TYPE_CONTAINING)
			{
				printf(" OCTET STRING (");
			}
			else
			{
				printf(" OCTET STRING");
			}
		}
		
		break;
	case BASE_TYPE_BIT_STRING:
		printf(" BIT STRING (");
		break;
	case BASE_TYPE_EXTENSION:
		printf("...");
		break;
	case BASE_TYPE_INTEGER:
		printf("INTEGER ");
		if (ie->sizeOf != NULL)
		{
			if (ie->sizeOf->endNumber == NULL)
			{
				printf("(%s)", ie->sizeOf->startNumber);
			}
			else
			{
				printf("(%s..%s)", ie->sizeOf->startNumber, ie->sizeOf->endNumber);
			}	
		}
		break;
	case BASE_TYPE_HEADER:
		if (ie->subIeItem != NULL)
		{
			printf(" ::= ");
		}
		
		break;
	}

	if (ie->subIeItem != NULL)
	{
		subIe = (IEItem *)(ie->subIeItem);

		if (ie->sizeOf != NULL)
		{
			printf(" OF ");
		}
		
		if (ie->baseType == BASE_TYPE_EXTENSION)
		{
			printf(",");
		}
	}

}

void printIEItemAttriAfter(IEItem *ie)
{
	IEItem *subIe;

	if (ie->defaultValue != NULL)
	{
		printf("\t\tDEFAULT   %s", ie->defaultValue);
	}

	if (ie->subIeItem != NULL)
	{
		subIe = (IEItem *)(ie->subIeItem);

		if (subIe->baseType == BASE_TYPE_CONTAINING)
		{
			printf(")");
		}

		if (ie->baseType != BASE_TYPE_INTEGER &&
			ie->baseType != BASE_TYPE_max &&
			ie->sizeOf == NULL &&
			subIe->baseType != BASE_TYPE_CONTAINING)
		{
			if (ie->baseType != BASE_TYPE_EXTENSION)
			{
				if (ie->baseType == BASE_TYPE_DBRACKET)
				{
					printf("]]");
				}
				else
				{
					if (ie->baseType == BASE_TYPE_HEADER)
					{
						//do nothing.
						//the ieItem is left of ::=
					}
					else
					{
						printf("}");
					}
				}
				
			}
		}
	}
	else
	{
		
		if (ie->nextIEItem == NULL)
		{
			if (ie->baseType != BASE_TYPE_INTEGER &&
				ie->baseType != BASE_TYPE_OCTET_STRING &&
				ie->baseType != BASE_TYPE_BIT_STRING &&
				ie->baseType != BASE_TYPE_CONTAINING &&
				ie->baseType != BASE_TYPE_EXTENSION &&
				ie->baseType != BASE_TYPE_COMBINATION &&
				ie->baseType != BASE_TYPE_HEADER &&
				ie->baseType != BASE_TYPE_max)
			{
				printf("}");
			}
		}

	}

	if (ie->ieAttri == IE_ATTRI_OPTION)
	{
		printf(" OPTIONAL");
	}

	if (ie->nextIEItem != NULL)
	{
		printf(",");
	}


	switch (ie->needType)
	{
	case NEED_TYPE_OP:
		printf(" -- Need OP");
		break;
	case NEED_TYPE_ON:
		printf(" -- Need ON");
		break;
	case NEED_TYPE_OR:
		printf(" -- Need OR");
		break;
	}

	if (ie->condTag != NULL)
	{
		printf(" -- Cond %s", ie->condTag);
	}
}

void HandleIndentSameLevelIEItem(IEItem *ie, int indentCount)
{
	int i;

	if (ie->baseType != BASE_TYPE_max)
	{
		printf("\n");
		for (i = 0; i < indentCount; i++)
		{
			printf("\t");
		}
	}
}

int HandleIndentEnterSubIEItem(IEItem *ie, int indentCount)
{
	IEItem *subIe;
	int i;

	subIe = ie->subIeItem;
	if(ie->sizeOf == NULL)
	{
		if (ie->baseType == BASE_TYPE_EXTENSION)
		{
			printf("\n");
			for (i = 0; i < indentCount; i++)
			{
				printf("\t");
			}
		}
		else
		{
			if (ie->baseType == BASE_TYPE_HEADER)
			{
				//do nothing.
				//the ieItem is left of ::=
			}
			else
			{
				if (subIe->baseType != BASE_TYPE_CONTAINING &&
					subIe->baseType != BASE_TYPE_max)
				{
					indentCount++;
					printf("\n");
					for (i = 0; i < indentCount; i++)
					{
						printf("\t");
					}
				}
			}
			
		}
	}

	return indentCount;
}

int HandleIndentLeaveSubIEItem(IEItem *ie, int indentCount)
{
	IEItem *subIe;
	int i;
	
	subIe = ie->subIeItem;
	if (ie->baseType != BASE_TYPE_max &&
		ie->sizeOf == NULL &&
		ie->baseType != BASE_TYPE_EXTENSION)
	{
		if (ie->baseType == BASE_TYPE_HEADER)
		{
			//do nothing.
			//the ieItem is left of ::=
		}
		else
		{
			if (subIe->baseType != BASE_TYPE_CONTAINING &&
				subIe->baseType != BASE_TYPE_max)
			{
				indentCount--;
				printf("\n");
				for (i = 0; i < indentCount; i++)
				{
					printf("\t");
				}
			}
		}

	}

	return indentCount;
}

void printIEItem(IEItem *ie)
{
	static int indentCount = 0;

	while(1)
	{
		printIEItemAttriPre(ie);
		
		if (ie->subIeItem != NULL)
		{
			indentCount = HandleIndentEnterSubIEItem(ie, indentCount);
			printIEItem(ie->subIeItem);
			indentCount = HandleIndentLeaveSubIEItem(ie, indentCount);
		}

		printIEItemAttriAfter(ie);

		if (ie->nextIEItem != NULL)
		{
			HandleIndentSameLevelIEItem(ie, indentCount);
			ie = ie->nextIEItem;
		}
		else
		{
			break;
		}
	}
}

void FreeIEItem(IEItem *ie)
{
	IEItem *ieItem;

	while (1)
	{
		if (ie->subIeItem != NULL)
		{
			ieItem = ie->subIeItem;
			FreeIEItem(ieItem);
		}

		if (ie->defaultValue != NULL)
		{
			free(ie->defaultValue);
			ie->defaultValue = NULL;
		}

		if(ie->sizeOf != NULL)
		{
			free(ie->sizeOf);
			ie->sizeOf = NULL;
		}

		if (ie->IETypeName != NULL)
		{
			free(ie->IETypeName);
			ie->IETypeName = NULL;
		}

		if (ie->IEVariableName != NULL)
		{
			free(ie->IEVariableName);
			ie->IEVariableName = NULL;
		}

		if (ie->condTag != NULL)
		{
			free(ie->condTag);
			ie->condTag = NULL;
		}

		if (ie->nextIEItem != NULL)
		{
			ieItem = ie;
			ie = ie->nextIEItem;
			
			free(ieItem);
			ieItem = NULL;
		}
		else
		{
			free(ie);
			ie = NULL;
			break;
		}
	}
}


void CreateContentItem(char *content, char *value)
{
	ContentItem *lContentItem;

	if (gContentItem == NULL)
	{
		gContentItem = malloc(sizeof(ContentItem));
		if (gContentItem == NULL)
		{
			printf("fail to malloc.\n");
			while(1){}
		}

		gCurrentContentItem = gContentItem;
		gCurrentContentItem->value = _atoi64(value);
		gCurrentContentItem->variable = content;
		gCurrentContentItem->nextItem = NULL;

		free(value);
	}
	else
	{
		lContentItem = malloc(sizeof(ContentItem));
		if (lContentItem == NULL)
		{
			printf("fail to malloc.\n");
			while (1) {}
		}

		lContentItem->variable = content;
		lContentItem->value = _atoi64(value);
		lContentItem->nextItem = NULL;

		gCurrentContentItem->nextItem = (void *)lContentItem;
		gCurrentContentItem = lContentItem;
	}
}

void DestroyContentItems()
{
	ContentItem *lContentItem;

	while (gContentItem != NULL)
	{
		lContentItem = gContentItem;
		gContentItem = gContentItem->nextItem;
		
		free(lContentItem->variable);
		lContentItem->variable = NULL;

		free(lContentItem);
	}
}

void AddIEItemNode(IEItem *ie)
{
	IEItemNode *ieNode = NULL;

	if (ie == NULL)
	{
		return;
	}

	if (gIEItemNodeHeader == NULL)
	{
		gIEItemNodeHeader = malloc(sizeof(IEItemNode));
		if (gIEItemNodeHeader == NULL)
		{
			printf("fail to malloc.\n");
			while (1) {}
		}

		gIEItemNodeHeader->item = ie;
		gIEItemNodeHeader->nextNode = NULL;

		gCurrentIEItemNode = gIEItemNodeHeader;
	}
	else
	{
		ieNode = malloc(sizeof(IEItemNode));
		if (ieNode == NULL)
		{
			printf("fail to malloc.\n");
			while (1) {}
		}

		ieNode->item = ie;
		ieNode->nextNode = NULL;

		gCurrentIEItemNode->nextNode = ieNode;
		gCurrentIEItemNode = ieNode;
	}
}

IEItem *FindIEItemHeader(char *typeName)
{
	IEItemNode *ieNode = gIEItemNodeHeader;
	IEItem *ie = NULL;
	char buff[1000] = { 0 };
	char varBuff[1000] = { 0 };

	while (ieNode != NULL)
	{
		strcpy(buff, ieNode->item->IETypeName);
		buff[strlen(ieNode->item->IETypeName)] = '\0';
		StrDash2Underline(buff);

		strcpy(varBuff, typeName);
		StrDash2Underline(varBuff);

		if (strcmp(buff, varBuff) == 0)
		{
			ie = ieNode->item;
			break;
		}
		ieNode = ieNode->nextNode;
	}

	return ie;
}

void HandleIEItems()
{
	IEItemNode *ieNode = gIEItemNodeHeader;

	while (ieNode != NULL)
	{
		printf("%s\n", ieNode->item->IETypeName);
		ParseCallback(ieNode->item);
		//printIEItem(ieNode->item);

		ieNode = ieNode->nextNode;
	}
}

void DestroyIEItemNodes()
{
	IEItemNode *ieNode;

	gCurrentIEItemNode = gIEItemNodeHeader;

	printf("free gIEItemNodeHeader......\n");

	while (gCurrentIEItemNode != NULL)
	{
		
		ieNode = gCurrentIEItemNode;

		if(ieNode->item != NULL)
		{
			FreeIEItem(ieNode->item);
		}
		else
		{
			printf("");
		}
		
		gCurrentIEItemNode = gCurrentIEItemNode->nextNode;

		if (ieNode != NULL)
		{
			free(ieNode);
		}
		
	}

	gCurrentIEItemNode = NULL;
	gIEItemNodeHeader = NULL;
}