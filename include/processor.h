#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../library/stack.h"
#include "config.h"

const int ERROR = 1;

typedef struct storage{
	Stack stk;
	NUM_T ax;
	NUM_T bx;
	NUM_T cx;
	NUM_T dx;
} Storage;

#define STORAGE_DUMP(storage)                              \
{                                                          \
	StorageDump(storage, __FILE__, __func__, __LINE__);    \
}                                                  

#define STACK_DUMP(stack)                                        \
{                                                                \
	int *code_err = stack_verification(stack, __func__);         \
	stack_dump(stack, code_err, __FILE__, __func__, __LINE__);   \
	free(code_err);                                              \
}         

int StorageCtor(Storage *str);
int StorageDtor(Storage *str);
int process_commands(FILE *fp_src, Storage *str);
int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err);
int call_asm();

#endif
