#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../include/config.h"

const char *NAME_SRC_ASM  = "src.txt"    ; 
const char *NAME_RES_ASM  = "res_asm.txt"; 

const size_t max_size_c        = 100;
const NUM_T  VENOM_NUM_COMMAND = -13;

int write_file(FILE *fp_src, FILE *fp_res);

#endif