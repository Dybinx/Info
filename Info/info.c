#include "info_type.h"
#include "Info.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



Item* CreateItem()
{
	Item* item = NULL;


	item = (Item*)malloc(sizeof(Item));

	if (item == NULL)
	{
		printf("fail to malloc in file: %s, line: %d\n", __FILE__, __LINE__);
	}

	item->ieVariableName = NULL;
	item->startNumber = NULL;
	item->endNumber = NULL;
	item->nextItem = NULL;


	return item;
}



void SetItemVariableName(Item* ie, char* variableName)
{
	if (ie->ieVariableName == NULL)
	{
		ie->ieVariableName = variableName;
	}
}

void SetItemRange(Item* ie, char* startNumber, char* endNumber)
{
	ie->startNumber = startNumber;
	ie->endNumber = endNumber;
}


Item* LinkItem(Item* ie1, Item* ie2)
{
	Item* item = NULL;

	item = ie1;


	while (ie1->nextItem != NULL)
	{
		ie1 = ie1->nextItem;
	}

	ie1->nextItem = (void*)ie2;

	return item;
}


void WriteLine(FILE* file, char* line)
{
	fprintf(file, "%s\n", line);
}



void ParseItem(Item* item)
{
	FILE* gen_c_file;
	FILE* gen_h_file;
	Item* tmpItem;
	int diff = 0;
	int width = 0;
	char buff[1000] = { 0 };

	gen_c_file = fopen("parse_asn1.c", "w");
	gen_h_file = fopen("parse_asn1.h", "w");

	if (gen_c_file == NULL)
	{
		return;
	}

	if (gen_h_file == NULL)
	{
		fclose(gen_c_file);
		return;
	}

	
	WriteLine(gen_h_file, "#ifndef _PARSE_ASN1_H_");
	WriteLine(gen_h_file, "#define _PARSE_ASN1_H_");
	WriteLine(gen_h_file, "");

	WriteLine(gen_h_file, "typedef struct {");

	tmpItem = item;

	while (tmpItem != NULL)
	{
		sprintf(buff, "\tint\t%s;", tmpItem->ieVariableName);
		WriteLine(gen_h_file, buff);
		tmpItem = tmpItem->nextItem;
	}

	WriteLine(gen_h_file, "}Info;");

	WriteLine(gen_h_file, "");
	WriteLine(gen_h_file, "extern void parase_asn1(Info *info, unsigned char *bitstream);");

	WriteLine(gen_h_file, "");
	WriteLine(gen_h_file, "#endif // !_PARSE_ASN1_H_");


	WriteLine(gen_c_file, "#include \"parse_asn1.h\"");
	WriteLine(gen_c_file, "#include \"handle_bits.h\"");
	WriteLine(gen_c_file, "");

	WriteLine(gen_c_file, "void parase_asn1(Info *info, unsigned char *bitstream)");
	WriteLine(gen_c_file, "{");

	WriteLine(gen_c_file, "\tBitsIter bits_iter = { 0 };");
	WriteLine(gen_c_file, "\tunsigned char bits_width = 0;");
	WriteLine(gen_c_file, "");

	tmpItem = item;

	while (tmpItem != NULL)
	{
		diff = atoi(tmpItem->endNumber) - atoi(tmpItem->startNumber);
		while ((__int64)pow(2, (double)width) < diff)
		{
			width++;
		}

		if (atoi(tmpItem->startNumber) > 0)
		{
			sprintf(buff, "\tinfo->%s = Get_bits(bitstream, %d, &bits_iter) + %d;", 
				tmpItem->ieVariableName, width, atoi(tmpItem->startNumber));
		}
		else
		{
			sprintf(buff, "\tinfo->%s = Get_bits(bitstream, %d, &bits_iter);", tmpItem->ieVariableName, width);
		}
		
		WriteLine(gen_c_file, buff);
		
		tmpItem = tmpItem->nextItem;
		width = 0;
	}

	WriteLine(gen_c_file, "}");

	fclose(gen_c_file);
	fclose(gen_h_file);
}