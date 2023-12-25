#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../../library/config.h"
#include "../../library/stack.h"
#include "../../library/error.h"

#include "../../library/color.h"

static const double EPSILON = 1e-9;


// STRUCT LIST
struct Storage {
  Stack stk_cmd;
  Stack stk_ptr;
  num_t regs[4];
};

struct Array{
    char   *arr_ptr;
    size_t size_arr;
};


// FUNCTIONS LIST
int StorageCtor(Storage *str);

int StorageDtor(Storage *str);

int process_commands(FILE *fp_src, Storage *str);

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err);

num_t *ptr_reg(Storage *str, int id_reg);

struct Array *ctor_struct_arr(FILE *fp_src);

size_t search_size_file(FILE *fp_src);


// DEFINE LIST
#ifdef INFO
    #define PRINTF_INFO_CMD()                                                   \
        PRINT_INFO("%s[%s]%s", GREEN, commands[num_cmd & 63] + 4, RESET);       \
        PRINT_INFO("%s[%d]%s\n", MAGENTA, num_cmd & 63, RESET);
#else
    #define PRINTF_INFO_CMD() {};
#endif

#define STORAGE_DUMP(storage) StorageDump(storage, __FILE__, __func__, __LINE__);

#define STACK_DUMP(stack)                                                          \
    {                                                                              \
        int *code_err = stack_verification(stack, __func__);                       \
        stack_dump(stack, code_err, __FILE__, __func__, __LINE__);                 \
        free(code_err);                                                            \
    }

#endif
