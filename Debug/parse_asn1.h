#ifndef _PARSE_ASN1_H_
#define _PARSE_ASN1_H_

typedef struct {
	int	a;
	int	b;
}Info;

extern void parase_asn1(Info *info, unsigned char *bitstream);

#endif // !_PARSE_ASN1_H_
