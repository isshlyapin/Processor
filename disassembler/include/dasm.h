#ifndef DASM_H
#define DASM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../library/config.h"


// STRUCT LIST
struct Array{
    char   *arr_ptr;
    size_t size_arr;
};


const size_t MAX_SIZE_STR      = 100;
const num_t  VENOM_NUM_COMMAND = -13;


// FUNCTIONS LIST
int byte_code_in_text(FILE *fp_src, FILE *fp_res);

struct Array *ctor_struct_arr(FILE *fp_src);

size_t search_size_file(FILE *fp_src);

#endif