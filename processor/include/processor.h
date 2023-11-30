#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../../library/stack.h"
#include "../../library/config.h"

const int ERROR = 1;

typedef struct storage{
	Stack stk;
	num_t rax;
	num_t rbx;
	num_t rcx;
	num_t rdx;
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

const int ERROR_REG = 777;

int StorageCtor(Storage *str);
int StorageDtor(Storage *str);
int process_commands(FILE *fp_src, Storage *str);
int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err);
int call_asm();
num_t *ptr_reg(Storage *str, num_t id_reg);
char *create_buf(FILE *fp_src, size_t *sz_file);


#endif
