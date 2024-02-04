#include "../include/asm.h"

int main(int argc, char *argv[])
{
    #ifdef LOG
        CHECK_ARGC(argc, 4);
    #else
        CHECK_ARGC(argc, 3);
    #endif

    PRINT_INFO("\n%s___WORKING ASSEMBLER___%s\n\n", RED, RESET);

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

    PRINT_LOG_TITLE(fp_log);

    struct Array *src_struct_arr = ctor_struct_arr(fp_src);    
    assert(src_struct_arr != NULL);

    struct Array *res_struct_arr = (struct Array*)calloc(1, sizeof(struct Array));
    assert(res_struct_arr != NULL);

    struct Label *arr_label = (struct Label*)calloc(100, sizeof(struct Label)); //TODO Кол-во меток в константу
    assert(arr_label != NULL);

    assembly(src_struct_arr, res_struct_arr, arr_label, 1);
    assembly(src_struct_arr, res_struct_arr, arr_label, 2);

    res_struct_arr->arr_ptr[res_struct_arr->size_arr] = (char)cmd_hlt;
    
    PRINT_LOG_STR(fp_log, "s",  commands[cmd_hlt] + 4,
                          "s",  "----",
                          "d",  cmd_hlt,
                          "s",  "----",
                          "lu", res_struct_arr->size_arr);

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

    while (src_pc < src_struct_arr->size_arr)
    {
        bool check_cmd = false;

        int ncr      = 0;  // ncr - number of characters read
        int num_cmd  = 0;
        
        char name_cmd[MAX_SIZE_STR] = "VENOM";

        bool read_cmd = sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr);
        if (!read_cmd)
        {
            PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_чтения_команды]);
            return ошибка_чтения_команды;
        }
        
        if (name_cmd[0] == '#')
        {
            while(src_arr_ptr[src_pc] != '\n')
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
