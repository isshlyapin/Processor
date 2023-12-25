#ifndef ASM_H
#define ASM_H

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

struct Label {
    char name_lab[MAX_SIZE_STR];
    int jmp_id;
};


// FUNCTIONS LIST
int create_byte_code(FILE *fp_src, FILE *fp_res);

int assembly(struct Array *src_struct_arr, struct Array *res_struct_arr, struct Label *arr_lab, int pass_num);

struct Array *ctor_struct_arr(FILE *fp_src);

size_t search_size_file(FILE *fp_src);

int check_num_reg(const char *str);


// DEFINE LIST
#define NUM_CMD_EQUALS_JMP_OR_CALL(num_cmd) ((num_cmd >= cmd_jmp) && (num_cmd <= cmd_jne)) || (num_cmd == cmd_call)

#define ORG_NOT_CORRECT(org_id, current_pc) ((size_t)org_id < current_pc) || (org_id < 0)

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
