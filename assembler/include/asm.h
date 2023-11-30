#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../library/config.h"

enum REGISTER{
	rax = 1,
	rbx,
	rcx,
	rdx
};

const int ERROR_REG = 777;
const int ERROR     = 1;

const char *NAME_SRC_ASM  = "src.txt"    ; 
const char *NAME_RES_ASM  = "res_asm.txt"; 

const size_t max_size_c        = 100;
const num_t  VENOM_NUM_COMMAND = -13;

#define NUM_MOD_PRINT "%.2lf"
#define NUM_MOD_SCAN  "%lf"

typedef double elem_t;

int create_bite_code(FILE *fp_src, FILE *fp_res);

int check_num_reg(const char *str);

char *create_buf(FILE *fp_src, size_t *sz_file);

int search_size_new_buf(char *buf, size_t sz_buf);

#endif