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
	IE_ATTRI_MANDA,				/* 强制属性，必选 */
	IE_ATTRI_OPTION,			/* 可选属性 */
	IE_ATTRI_max = 0x7FFFFFFF
}IE_ATTRI_E;

typedef struct {
	char *startNumber;
	char *endNumber;		//maybe is NULL, when just have a startNumber.
}IERange;		//类型的范围，指示数组的大小。

typedef struct {
	char *IEVariableName;		//IE对应的变量名称
	char *IETypeName;			//IE对应的类型名称，如果无法确定，可以暂不确定。
	BASE_TYPE_E baseType;		//指示通过哪种类型包含当前项，比如SEQUENCE {IEItem},表示当前项的类型是SEQUENCE。
								//在一个结构中baseType 和 IETypeName二者只存在一个。
	IE_ATTRI_E ieAttri;			//ie属性.
	void *subIeItem;			//IE的子内容，如果没有的话则为NULL。
	void *nextIEItem;			//指向同级的IE结构，他们共同组成数组，如果没有的话设置为NULL。
	IERange *sizeOf;			//如果是数组的话，指示它的大小。
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