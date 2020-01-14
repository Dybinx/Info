#ifndef _INFO_H_
#define _INFO_H_

#include "info_type.h"

extern Item* CreateItem();
extern void SetItemVariableName(Item* ie, char* variableName);
extern Item* LinkItem(Item* ie1, Item* ie2);
extern void ParseItem(Item* item);
extern void SetItemRange(Item* ie, char* startNumber, char* endNumber);


#endif // !_INFO_H_
