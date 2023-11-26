#ifndef DASM_H
#define DASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../include/config.h"

const size_t max_size_c        = 100;
const NUM_T  VENOM_NUM_COMMAND = -13;
const int    ERROR             = 1;

const char *NAME_SRC_DASM = "res_asm.txt" ;
const char *NAME_RES_DASM = "res_dasm.txt";

int write_file(FILE *fp_src, FILE *fp_res);

#endif