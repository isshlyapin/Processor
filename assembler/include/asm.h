#ifndef ASM_H
#define ASM_H

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../../library/config.h"
#include "../../library/commands.h"
#include "../../library/error.h"

#include "../../library/color.h"


// Список структур
struct Array {
    char   *arr_ptr;
    size_t size_arr;
};

struct Label {
    char name_lab[MAX_SIZE_STR];
    int jmp_id;
};


// Блок функций
int create_byte_code(FILE *fp_src, FILE *fp_res);

int assembly(struct Array *src_struct_arr, struct Array *res_struct_arr, struct Label *arr_lab, int pass_num);

struct Array *ctor_struct_arr(FILE *fp_src);

size_t search_size_file(FILE *fp_src);

int check_num_reg(const char *str);


// Блок директив #define
#ifdef LOG
    #include "./log.h"

    #define PRINT_LOG_PARTITION(fp) print_log_partition(fp)
    #define PRINT_LOG_TITLE(fp)     print_log_title(fp)

    FILE *fp_log = NULL;
    #define OPEN_LOG_FIlE()              \
        fp_log = fopen(argv[3], "a");    \
        CHECK_OPEN_FILE(fp_log);

    #define CLOSE_LOG_FILE() fclose(fp_log)
    
    #define PRINT_LOG_STR(fp, tp1, col1, tp2, col2, tp3, col3, tp4, col4, tp5, col5)    \
        fprintf(fp, "|%*s%*" tp1 "|",   INDENT_COL, " ", -WIDTH_COL, col1);             \
        fprintf(fp, "%*s%*"  tp2 "|",   INDENT_COL, " ", -WIDTH_COL, col2);             \
        fprintf(fp, "%*s%*"  tp3 "|",   INDENT_COL, " ", -WIDTH_COL, col3);             \
        fprintf(fp, "%*s%*"  tp4 "|",   INDENT_COL, " ", -WIDTH_COL, col4);             \
        fprintf(fp, "%*s%*"  tp5 "|\n", INDENT_COL, " ", -WIDTH_COL, col5);             \
        print_log_partition(fp);
#else
    #define OPEN_LOG_FIlE()          {}
    #define CLOSE_LOG_FILE()         {}
    #define PRINT_LOG_PARTITION(...) {}
    #define PRINT_LOG_TITLE(...)     {}
    #define PRINT_LOG_STR(...)       {}    
#endif // LOG

#define NUM_CMD_EQUALS_JMP_OR_CALL(num_cmd) ((num_cmd >= cmd_jmp) && (num_cmd <= cmd_jne)) || (num_cmd == cmd_call)

#define ORG_NOT_CORRECT(org_id, current_pc) ((size_t)org_id < current_pc) || (org_id < 0)

#endif // ASM_H
