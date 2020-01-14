#include <stdio.h>
#include "handle_bits.h"

extern FILE *yyin;

extern int yyparse(void);

int main(int argc, char **argv) {

	unsigned int value = 0;
	unsigned char nas[6] = { 0 };
	unsigned char buff[] = { 0x24, 0x01, 0x20, 0x1D, 0xE0, 0x00, 0x00, 0x00, 0x18, 0x20 };

	BitsIter bits_iter = { 0 };

	bits_iter.bit_iter = 3;
	bits_iter.octet_iter = 3;

	value = Get_bits(buff, 2, &bits_iter);
	Get_octets(nas, buff, 6, &bits_iter);
	if (argc < 2)
	{
		printf("argc error!\n");
		return 0;
	}

	yyin = fopen(argv[1], "r");
	yyparse();
	return 0;
}
