#include "../include/dasm.h"

int main(int argc, char *argv[])
{
    CHECK_ARGC(argc, 3);

    PRINT_INFO("\n___%sWORKING DISASSEMBLER%s___\n\n", CYAN, RESET);

    FILE *fp_src = fopen(argv[1], "rb");
    CHECK_OPEN_FILE(fp_src);

    FILE *fp_res = fopen(argv[2], "w");
    CHECK_OPEN_FILE(fp_res);

    byte_code_in_text(fp_src, fp_res);

    fclose(fp_src);
    fclose(fp_res);
}

int byte_code_in_text(FILE *fp_src, FILE *fp_res)
{
    assert(fp_res != NULL);
    assert(fp_src != NULL);

    #include "../../library/cmd_define.h"

    #define NEW_INSTRUCTIONS(name, num, ASM_CMD, DASM_CMD, ...)   \
        if ((num_cmd & 63) == num)                                \
        {                                                         \
            DASM_CMD(num);                                        \
        } 

    struct Array *src_struct_arr = ctor_struct_arr(fp_src);
    assert(src_struct_arr != NULL);

    char *array = src_struct_arr->arr_ptr;

    num_t num    = 0;
    int   jmp_id = 0;

    for (size_t pc = 0; pc < src_struct_arr->size_arr; )
    {
        char num_cmd   = array[pc];
        bool check_cmd = false;

        #include "../../library/instructions_def.h"

        if (!check_cmd)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ошибка_в_имени_команды]);
            return ошибка_в_имени_команды;
        }

        PRINT_INFO("%s[%2d]%s\n", RED, num_cmd & 63, RESET);
    }

    free(src_struct_arr->arr_ptr);
    free(src_struct_arr);

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
