#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../../library/stack.h"
#include "../../library/config.h"

typedef struct{
    Stack stk_cmd;
    Stack stk_ptr;
    num_t regs[4];
} Storage;

int StorageCtor(Storage *str);

int StorageDtor(Storage *str);

int process_commands(FILE *fp_src, Storage *str);

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err);

num_t *ptr_reg(Storage *str, int id_reg);

char *create_src_arr(FILE *fp_src, size_t *sz_file);

#ifdef INFO
    #define PRINTF_INFO_CMD()                                                     \
        if (num_cmd == cmd_rpush)                                            \
            PRINT_INFO("%s[%s]%s", GREEN, commands[cmd_push], RESET);             \
        else                                                                      \
            PRINT_INFO("%s[%s]%s", GREEN, commands[(int)num_cmd], RESET);         \
        PRINT_INFO("%s[%d]%s\n", MAGENTA, num_cmd, RESET);                     
#else
    #define PRINTF_INFO_CMD() {}
#endif

#define STORAGE_DUMP(storage) StorageDump(storage, __FILE__, __func__, __LINE__);

#define STACK_DUMP(stack)                                        \
{                                                                \
    int *code_err = stack_verification(stack, __func__);         \
    stack_dump(stack, code_err, __FILE__, __func__, __LINE__);   \
    free(code_err);                                              \
}

#define POP(num_ptr)    StackPop(&str->stk_cmd, num_ptr);
#define POP_PTR(ptr_id) StackPop(&str->stk_ptr, ptr_id);
#define PUSH_PTR(ptr)   StackPush(&str->stk_ptr, ptr);
#define PUSH(num)       StackPush(&str->stk_cmd, num);
#define PTR_REG(id)     ptr_reg(str, id)

#define TWO_POP(num1, num2)                \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);  

#define JMP_IF(condition, num1, num2, name_arr)                   \
    TWO_POP(num1, num2)                                           \
    if (condition)                                                \
    {                                                             \
        memcpy(&id_jmp, name_arr + pc + 1, sizeof(int));          \
        pc = (size_t)id_jmp;                                      \
    }                                                             \
    else                                                          \
        pc += 1 + sizeof(int);

#endif
