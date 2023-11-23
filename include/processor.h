#include "../library/stack.h"


typedef struct storage{
	Stack stk;
	ELEM_T ax;
	ELEM_T bx;
	ELEM_T cx;
	ELEM_T dx;
} Storage;

int StorageCtor(Storage *str);



