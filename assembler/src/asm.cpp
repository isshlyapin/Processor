#include "../include/asm.h"
#include <stdlib.h>
#include <iostream>
#include <string>

#define CREATE_LOG_STR(first_col, second_col, third_col, fourth_col)                            \
    create_log_str(log_str, -1, "VENOM");                                                       \
    create_log_str(log_str, 1, first_col);                                                      \
    create_log_str(log_str, 2, second_col);                                                     \
    create_log_str(log_str, 3, third_col);                                                      \
    create_log_str(log_str, 4, fourth_col);                                                     \
    PRINT_LOG("%s", log_str);                                                                   \
    create_log_str(log_str, -2, "VENOM"); 

#define PRINT_PARTITION()                    \
    for (int i = 0; i < LEN_LOG_STR; i++)    \
    {                                        \
        if ((i == 0) || (i == FIVE_COLUM))   \
            str[i] = '|';                    \
        else if ( i == (LEN_LOG_STR - 2))    \
            str[i] = '\n';                   \
        else if (i == (LEN_LOG_STR - 1))     \
            str[i] = '\0';                   \
        else                                 \
            str[i] = '-';                    \
    }                                        \
    PRINT_LOG("%s", str);

#define PRINT_TITLE_DATE()                                      \
    for (int i = 0; i < LEN_LOG_STR; i++)                       \
    {                                                           \
        if ((i == 0) || (i == FIVE_COLUM))                      \
            str[i] = '|';                                       \
        else if ( i == (LEN_LOG_STR - 2))                       \
            str[i] = '\n';                                      \
        else if (i == (LEN_LOG_STR - 1))                        \
            str[i] = '\0';                                      \
        else                                                    \
            str[i] = ' ';                                       \
    }                                                           \
    int j = 0, k = 0;                                           \
    while (__DATE__[j] != '\0')                                 \
    {                                                           \
        str[((LEN_LOG_STR - 19) / 2) + j] = __DATE__[j];        \
        j++;                                                    \
    }                                                           \
    while (__TIME__[k] != '\0')                                 \
    {                                                           \
        str[((LEN_LOG_STR - 19) / 2) + j + 1] = __TIME__[k];    \
        j++;                                                    \
        k++;                                                    \
    }                                                           \
    PRINT_LOG("%s", str);

#define CREATE_SAMPLE()                                                                                    \
    for (int i = 0; i < LEN_LOG_STR; i++)                                                                  \
    {                                                                                                      \
        if ((i == 0) || (i == TWO_COLUM) || (i == THREE_COLUM) || (i == FOUR_COLUM) || (i == FIVE_COLUM))  \
            str[i] = '|';                                                                                  \
        else if (i == (LEN_LOG_STR - 2))                                                                   \
            str[i] = '\n';                                                                                 \
        else if (i == (LEN_LOG_STR - 1))                                                                   \
            str[i] = '\0';                                                                                 \
        else                                                                                               \
            str[i] = ' ';                                                                                  \
    }   

#define PRINT_TITLE()                               \
    for (int i = 0; "Name cmd"[i] != '\0'; i++)     \
        str[5 + i] = "Name cmd"[i];                 \
    for (int i = 0; "Num cmd"[i] != '\0'; i++)      \
        str[TWO_COLUM + 5 + i] = "Num cmd"[i];      \
    for (int i = 0; "Parameter"[i] != '\0'; i++)    \
        str[THREE_COLUM + 5 + i] = "Parameter"[i];  \
    for (int i = 0; "CMD ID"[i] != '\0'; i++)       \
        str[FOUR_COLUM + 5 + i] = "CMD ID"[i];      \
    PRINT_LOG("%s", str);

int main(int argc, char *argv[])
{
    #ifdef LOG
        CHECK_ARGC(argc, 4);
    #else
        CHECK_ARGC(argc, 3);
    #endif //! LOG

    PRINT_INFO("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);

    OPEN_LOG_FIlE();

    FILE *fp_src = fopen(argv[1], "r");
    CHECK_OPEN_FILE(fp_src);   

    FILE *fp_res = fopen(argv[2], "wb");
    CHECK_OPEN_FILE(fp_res);   

    create_byte_code(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
    
    CLOSE_LOG_FILE();
}

int create_byte_code(FILE *fp_src, FILE *fp_res)
{
    assert(fp_res != NULL);
    assert(fp_src != NULL);

    char log_str[LEN_LOG_STR] = {};
    create_log_str(log_str, 0, "VENOM");
    
    struct Array *src_struct_arr = ctor_struct_arr(fp_src);
    
    assert(src_struct_arr != NULL);

    struct Array *res_struct_arr = (struct Array*)calloc(1, sizeof(struct Array));
    assert(res_struct_arr != NULL);

    struct Label *arr_label = (struct Label*)calloc(100, sizeof(struct Label));
    assert(arr_label != NULL);

    assembly(src_struct_arr, res_struct_arr, arr_label, 1);
    assembly(src_struct_arr, res_struct_arr, arr_label, 2);

    res_struct_arr->arr_ptr[res_struct_arr->size_arr] = (char)cmd_hlt;
    
    char num_str[30] = {};
    CREATE_LOG_STR(commands[cmd_hlt] + 4, my_int_to_string(cmd_hlt, num_str), "----", my_int_to_string((int)res_struct_arr->size_arr, num_str));
    
    // PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[cmd_hlt] + 4, cmd_hlt);
    // PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", "---", res_struct_arr->size_arr);

    PRINT_INFO("name_cmd: %s[%4s]%s", RED, "hlt", RESET);
    PRINT_INFO("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);

    fwrite(res_struct_arr->arr_ptr, sizeof(char), res_struct_arr->size_arr + 1, fp_res);

    free(src_struct_arr->arr_ptr);
    free(src_struct_arr);

    free(res_struct_arr->arr_ptr);
    free(res_struct_arr);

    free(arr_label);

    return ошибок_нет;
}

char *my_int_to_string(int num, char *str_res)
{
    char num_str[30] = {};

    if (num == 0)
    {
        str_res[0] = '0';
        str_res[1] = '\0';
    }
    else
    {
        for (int i = 0; i < 30; i++)
            num_str[i] = '\0';

        for (int j = 0; num != 0; j++)
        {
            num_str[j] = 48 + num % 10;
            num /= 10;        
        }

        
        int step = 0;
        for (int k = 29; k > 0; k--)
        {
            if (num_str[k] == '\0')
                continue;
            else
            {
                str_res[step] = num_str[k];
                step++;
            }
        } 
        str_res[step] = '\0';
    }

    return str_res;
}

void create_log_str(char *str, int flag, const char *info)
{
    if (flag == -1)
    {
        CREATE_SAMPLE();
    }
    else if (flag == -2)
    {
        PRINT_PARTITION();
    }
    else if (flag == 0)
    {
        PRINT_LOG("\n\n");
        PRINT_PARTITION();
        PRINT_TITLE_DATE();
        PRINT_PARTITION();
        CREATE_SAMPLE();
        PRINT_TITLE();
        PRINT_PARTITION();
    }
    else if (flag == 1)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[5 + i] = info[i];
    }
    else if (flag == 2)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[TWO_COLUM + 5 + i] = info[i];
    }
    else if (flag == 3)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[THREE_COLUM + 5 + i] = info[i];
    }
    else if (flag == 4)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[FOUR_COLUM + 5 + i] = info[i];
    }
}

struct Array *ctor_struct_arr(FILE *fp_src)
{
    struct Array *new_struct_arr = (struct Array*)malloc(sizeof(struct Array));

    long   start_ptr_file = ftell(fp_src);
    size_t sz_file        = search_size_file(fp_src);
    char   *array         = (char*)calloc(sz_file + 1, sizeof(char));

    fread(array, sizeof(char), sz_file, fp_src);

    new_struct_arr->arr_ptr  = array;
    new_struct_arr->size_arr = sz_file;
    
    fseek(fp_src, start_ptr_file, SEEK_SET);
    return new_struct_arr;
}

size_t search_size_file(FILE *fp_src)
{
    assert(fp_src != NULL);

	long start_ftell = ftell(fp_src);
    fseek(fp_src, 0, SEEK_END);
    
    size_t size_file = (size_t)ftell(fp_src);
    fseek(fp_src, start_ftell, SEEK_SET);

    return size_file;
}

int assembly(struct Array *src_struct_arr, struct Array *res_struct_arr, struct Label *arr_lab, int pass_num)
{
    #include "../../library/cmd_define.h"
    
    #define NEW_INSTRUCTIONS(name, num, ASM_DEF, ...)    \
        if (strcmp(name_cmd, #name + 4) == 0)            \
        {                                                \
            check_cmd = true;                            \
            ASM_DEF(pass_num, num);                      \
        }                   
        
    char *src_arr_ptr = src_struct_arr->arr_ptr;
    char *res_arr_ptr = res_struct_arr->arr_ptr;
    size_t sz_res_arr = res_struct_arr->size_arr;
    
    int    index_lab  = 1;
    num_t  num_user   = 0;
    size_t pc         = 0;
    size_t src_pc     = 0;

    char log_str[LEN_LOG_STR] = {};
    char num_str[30] = {};

    while (src_pc < src_struct_arr->size_arr)
    {
        bool check_cmd = false;

        int ncr      = 0;  // ncr - number of characters read}
        int num_cmd  = 0;
        
        char name_cmd[MAX_SIZE_STR] = "VENOM";

        bool read_cmd = sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr);
        if (!read_cmd)
        {
            PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_чтения_команды]);
            return ошибка_чтения_команды;
        }
        
        if (strcmp(name_cmd, "#") == 0)
        {
            while(src_arr_ptr[src_pc] != '\n')
                src_pc++;
            src_pc++;
            continue;
        }        
        else if (name_cmd[0] == ':')
        {
            if (pass_num == 1)
            {
                arr_lab[0].jmp_id++;

                arr_lab[index_lab].jmp_id = (int)sz_res_arr;
                strncpy(arr_lab[index_lab].name_lab, name_cmd + 1, MAX_SIZE_STR - 1);
                index_lab++;
            }
            src_pc += (size_t)ncr;
            continue;             
        }
        else
            src_pc += (size_t)ncr;

        #include "../../library/instructions_def.h"

        if (!check_cmd)
        {
            PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_в_имени_команды]);
            return ошибка_в_имени_команды;
        }
    }

    if (pass_num == 1)
    {
        res_struct_arr->size_arr = sz_res_arr;
        res_struct_arr->arr_ptr  = (char*)calloc(sz_res_arr + 2, sizeof(char));
    }

    #undef NEW_INSTRUCTIONS

    return ошибок_нет;
}

int check_num_reg(const char *str)
{
    switch(str[1])
    {
        case 'a':
            return rax;
        case 'b':
            return rbx;
        case 'c':
            return rcx;
        case 'd':
            return rdx;
        default:
            return ошибка_в_имени_регистра;
    }
}
