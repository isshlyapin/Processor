#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../library/config.h"

const int ERROR_REG = 777;
const int ERROR     = 1;

const char *NAME_SRC_ASM = "src(2).txt"  ; 
const char *NAME_RES_ASM = "res_asm.txt" ; 
const char *NAME_LOG_ASM = "log_file.txt";

const size_t max_size_c        = 100;
const num_t  VENOM_NUM_COMMAND = -13;

int check_num_reg(const char *str);

int create_bite_code(FILE *fp_src, FILE *fp_res);

size_t calculate_size_array_cmd_and_parameters(FILE *fp_src);

#ifdef LOG
    FILE *fp_log = NULL;
    #define OPEN_LOG_FIlE()                                     \
    fp_log = fopen(NAME_LOG_ASM, "a");                          \
    if (fp_log == NULL)                                         \
    {                                                           \
        fprintf(stderr, "ERROR: Don't open source file\n");     \
        return ERROR;                                           \
    }                                
    #define CLOSE_LOG_FILE() fclose(fp_log) 
    #define PRINT_LOG(str ...) fprintf(fp_log, str)
#else
    #define OPEN_LOG_FIlE()  {}
    #define CLOSE_LOG_FILE() {}
    #define PRINT_LOG(str ...) {}
#endif

#endif