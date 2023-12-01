#ifndef DASM_H
#define DASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../library/config.h"

const size_t max_size_c        = 100;
const num_t  VENOM_NUM_COMMAND = -13;
const int    ERROR             = 1;

const char *NAME_SRC_DASM = "res_asm.txt" ;
const char *NAME_RES_DASM = "res_dasm.txt";

int bite_code_in_text(FILE *fp_src, FILE *fp_res);

char *create_buf(FILE *fp_src, size_t *sz_file);

#endif