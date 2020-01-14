#pragma once

#include <stdbool.h>

#define INVALID 0xFFFFFFFF
#define INVALID_64 0x7FFFFFFFFFFFFFFF

typedef enum {
	BASE_TYPE_SEQUENCE,
	BASE_TYPE_ENUMERATED,
	BASE_TYPE_CHOICE,
	BASE_TYPE_INTEGER,
	BASE_TYPE_OCTET_STRING,
	BASE_TYPE_BIT_STRING,
	BASE_TYPE_CONTAINING,
	BASE_TYPE_EXTENSION,
	BASE_TYPE_DBRACKET,				//the action like SEQUENCE.
	/*for not base type, such as FreqBandIndicator. if a IeItem have base 
	  type and combination at the same time, then set it as 
	  a base type. */
	BASE_TYPE_COMBINATION,			
	BASE_TYPE_HEADER,				//the type is left of ::=
	BASE_TYPE_BOOLEAN,
	BASE_TYPE_NULL,
	BASE_TYPE_max = 0x7FFFFFFF
}BASE_TYPE_E;

typedef enum {
	NEED_TYPE_OP,
	NEED_TYPE_ON,
	NEED_TYPE_OR,
	NEED_TYPE_max = 0x7FFFFFFF
}NEED_TYPE_E;

typedef enum {
	IE_ATTRI_MANDA,				/* ǿ�����ԣ���ѡ */
	IE_ATTRI_OPTION,			/* ��ѡ���� */
	IE_ATTRI_max = 0x7FFFFFFF
}IE_ATTRI_E;

typedef struct {
	char *startNumber;
	char *endNumber;		//maybe is NULL, when just have a startNumber.
}IERange;		//���͵ķ�Χ��ָʾ����Ĵ�С��

typedef struct {
	char *IEVariableName;		//IE��Ӧ�ı�������
	char *IETypeName;			//IE��Ӧ���������ƣ�����޷�ȷ���������ݲ�ȷ����
	BASE_TYPE_E baseType;		//ָʾͨ���������Ͱ�����ǰ�����SEQUENCE {IEItem},��ʾ��ǰ���������SEQUENCE��
								//��һ���ṹ��baseType �� IETypeName����ֻ����һ����
	IE_ATTRI_E ieAttri;			//ie����.
	void *subIeItem;			//IE�������ݣ����û�еĻ���ΪNULL��
	void *nextIEItem;			//ָ��ͬ����IE�ṹ�����ǹ�ͬ������飬���û�еĻ�����ΪNULL��
	IERange *sizeOf;			//���������Ļ���ָʾ���Ĵ�С��
	NEED_TYPE_E needType;
	char *condTag;
	char *defaultValue;
	bool isPrinted;
}IEItem;

typedef struct
{
	IEItem *item;
	void *nextNode;
}IEItemNode;

typedef struct
{
	char *variable;
	__int64 value;
	void *nextItem;
}ContentItem;

extern IEItem *CreateSetupRelease(char *IETypeName);
extern IEItem *CreateIEItem();
extern void SetSubItem(IEItem *ie, IEItem *subIe);
extern void SetIEItemVariableName(IEItem *ie, char *IEVariableName);
extern void SetIEItemTypeName(IEItem *ie, char *IETypeName);
extern void SetIEItemType(IEItem *ie, BASE_TYPE_E ieType);
extern void SetIEItemCondTag(IEItem *ie, char *condTag);
extern void SetIEItemArray(IEItem *ie, IERange *size);
extern void SetIEItemNeedType(IEItem *ie, NEED_TYPE_E needType);
extern void SetIEItemAttri(IEItem *ie, IE_ATTRI_E ieAttri);
extern void SetIEItemDefaultValue(IEItem *ie, char *defaultValue);
extern IERange *CreateArray(char *startNum, char *endNum);
//link the same type IEItems.
extern IEItem *LinkIEItem(IEItem *ie1, IEItem *ie2);
extern void printIEItem(IEItem *ie);
extern void FreeIEItem(IEItem *ie);

extern IEItem *FindIEItemHeader(char *typeName);