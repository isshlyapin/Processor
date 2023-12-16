#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../../library/stack.h"
#include "../../library/config.h"

const int ERROR     = 1;
const int ERROR_REG = 777;

typedef struct storage{
    Stack stk_cmd;
    Stack stk_ptr;
    num_t rax;
    num_t rbx;
    num_t rcx;
    num_t rdx;
} Storage;

int StorageCtor(Storage *str);

int StorageDtor(Storage *str);

int process_commands(FILE *fp_src, Storage *str);

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err);

num_t *ptr_reg(Storage *str, int id_reg);

char *create_array_cmd_and_prm(FILE *fp_src, size_t *sz_file);

#ifdef INFO
    #define PRINTF_INFO_CMD()                                                     \
        if (num_command == cmd_hlt)                                               \
        {                                                                         \
            PRINT_INFO("%s[%s]%s", GREEN, commands[NUMBER_INSTRUCTIONS], RESET);  \
        }                                                                         \
        else if (num_command == cmd_rpush)                                        \
        {                                                                         \
            PRINT_INFO("%s[%s]%s", GREEN, commands[cmd_push], RESET);             \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            PRINT_INFO("%s[%s]%s", GREEN, commands[(int)num_command], RESET);     \
        }                                                                         \
        PRINT_INFO("%s[%d]%s\n", MAGENTA, num_command, RESET);                     
#else
    #define PRINTF_INFO_CMD() {}
#endif

#define CHECK_OPEN_FILE(file_ptr)                        \
if (file_ptr == NULL)                                    \
{                                                        \
    fprintf(stderr, "ERROR: Don't open source file\n");  \
    return ERROR;                                        \
}

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

#define PUSHH(num, array)                     \
    memcpy(&num, array, sizeof(num_t));       \
    StackPush(&str->stk_cmd, num1);               \

#define CMD_SQRT(num)                 \
    StackPop(&str->stk_cmd, &num);        \
    StackPush(&str->stk_cmd, sqrt(num));  
                
#define CMD_SIN(num)                   \
    StackPop(&str->stk_cmd, &num);         \
    StackPush(&str->stk_cmd, sin(num));    

#define CMD_COS(num)                   \
    StackPop(&str->stk_cmd, &num);         \
    StackPush(&str->stk_cmd, cos(num));    

#define CMD_IN(num)                    \
    printf("Введите число: ");         \
    fscanf(stdin, NUM_MOD_SCAN, &num); \
    StackPush(&str->stk_cmd, num);

#define CMD_ADD(num1, num2)            \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);        \
    StackPush(&str->stk_cmd, num1 + num2); 

#define CMD_SUB(num1, num2)            \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);        \
    StackPush(&str->stk_cmd, num2 - num1); 

#define CMD_MUL(num1, num2)            \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);        \
    StackPush(&str->stk_cmd, num1 * num2); 

#define CMD_DIV(num1, num2)            \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);        \
    StackPush(&str->stk_cmd, num2 / num1); 

#define TWO_POP(num1, num2)            \
    StackPop(&str->stk_cmd, &num1);        \
    StackPop(&str->stk_cmd, &num2);  

#define CMD_OUT(num)                                                 \
    StackPop(&str->stk_cmd, &num);                                       \
    printf("%sОтвет: [%.2lf]%s\n", YELLOW, num, RESET);  
                
#define JMP_IF(condition, num1, num2)                            \
    TWO_POP(num1, num2)                                          \
    if (condition)                                               \
    {                                                            \
        memcpy(&id_jmp, array_cmd_and_prm + pc + 1, sizeof(int)); \
        pc = (size_t)id_jmp;                                     \
    }                                                            \
    else                                                         \
        pc += 1 + sizeof(int);

#endif
