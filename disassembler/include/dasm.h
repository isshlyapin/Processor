#ifndef DASM_H
#define DASM_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "../../library/config.h"
#include "../../library/commands.h"
#include "../../library/error.h"

#include "../../library/color.h"


// STRUCT LIST
    struct Array{
    char   *arr_ptr;
    size_t size_arr;
};


// FUNCTIONS LIST
int byte_code_in_text(FILE *fp_src, FILE *fp_res);

struct Array *ctor_struct_arr(FILE *fp_src);

size_t search_size_file(FILE *fp_src);

#endif
