#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../library/config.h"

const size_t MAX_SIZE_STR  = 100;
const num_t  VENOM_NUM_CMD = -13;
const int    LEN_NAME_REG  = 3;

struct Array{
    char   *arr_ptr;
    size_t size_arr;
};

int check_num_reg(const char *str);

int create_byte_code(FILE *fp_src, FILE *fp_res);

int ctor_src_arr(FILE *fp_src, struct Array *file_data);

size_t calc_sz_res_arr(FILE *fp_src, struct Array *file_data);

int command_process(const char *name_cmd, int* num_cmd);

size_t search_size_file(FILE *fp_src);

#define NUM_CMD_EQUALS_JMP_OR_CALL(num_cmd) ((num_cmd >= cmd_jmp) && (num_cmd <= cmd_jne)) || (num_cmd == cmd_call)

#define ORG_NOT_CORRECT(org_id, current_pc) ((size_t)org_id < current_pc) || (org_id < 0)

#define COMMENT_PROCESS(array, src_pc, ncr)   \
    if (strcmp(name_cmd, "#") == 0)           \
    {                                         \
        while(array[src_pc] != '\n')          \
            src_pc++;                         \
        src_pc++;                             \
        continue;                             \
    }                                         \
    else                                      \
        src_pc += ncr; 

#ifdef LOG
    FILE *fp_log = NULL;

    #define OPEN_LOG_FIlE()              \
        fp_log = fopen(argv[3], "a");    \
        CHECK_OPEN_FILE(fp_log);

    #define CLOSE_LOG_FILE() fclose(fp_log) 
    #define PRINT_LOG(str ...) fprintf(fp_log, str)
#else
    #define OPEN_LOG_FIlE()  {}
    #define CLOSE_LOG_FILE() {}
    #define PRINT_LOG(str ...) {}
#endif

#endif