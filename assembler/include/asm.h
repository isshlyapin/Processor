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

#define NUM_MOD_PRINT "%.2lf"
#define NUM_MOD_SCAN  "%lf"

// char *create_buf(FILE *fp_src, size_t *sz_file);

int check_num_reg(const char *str);

int create_bite_code(FILE *fp_src, FILE *fp_res, FILE *fp_log);

// int search_size_array_cmd_and_param(char *buf, size_t sz_buf);

size_t calculate_size_array_cmd_and_parameters(FILE *fp_src);


#endif