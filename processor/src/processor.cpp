#include "../include/processor.h"
#include "../../library/commands.h"

int process_commands(FILE *fp_src, Storage *str)
{
    #include "../../library/cmd_define.h"
    
    #define NEW_INSTRUCTIONS(name, num, ASM_CMD, DASM_CMD, PROC_CMD)   \
        if (num == (num_cmd & 63))                                     \
        {                                                              \
            PROC_CMD(num)                                              \
        }

    PRINT_INFO("\n___%sWORKING PROCESSOR%s___\n\n", GREEN, RESET);

    struct Array *src_struct_arr = ctor_struct_arr(fp_src);
    assert(src_struct_arr != NULL);

    char *src_arr = src_struct_arr->arr_ptr;

    for (size_t pc = 0; pc < src_struct_arr->size_arr; )
    {
        char num_cmd   = src_arr[pc];

        PRINTF_INFO_CMD();

        num_t num1 = VENOM_ELEM;
        num_t num2 = VENOM_ELEM;
        int id_jmp = 0;
        
        #include "../../library/instructions_def.h"
 
        if (true)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ошибка_обработки_команды_процессором]);
            return ошибка_обработки_команды_процессором;
        }
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

int StorageCtor(Storage *str)
{
    StackCtor(&str->stk_cmd, STD_STACK_CAP);
    StackCtor(&str->stk_ptr, STD_STACK_CAP);

    str->regs[rax] = VENOM_ELEM;
    str->regs[rbx] = VENOM_ELEM;
    str->regs[rcx] = VENOM_ELEM;
    str->regs[rdx] = VENOM_ELEM;

    return ошибок_нет;
}

int StorageDtor(Storage *str) 
{
    StackDtor(&str->stk_cmd);
    StackDtor(&str->stk_ptr);
    str->regs[rax] = VENOM_ELEM;
    str->regs[rbx] = VENOM_ELEM;
    str->regs[rcx] = VENOM_ELEM;
    str->regs[rdx] = VENOM_ELEM;

    return ошибок_нет;
}

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err)
{
    fprintf(stderr, "------------------------------------------------------------"
                    "----------------\n");
    fprintf(stderr, "Storage[%p] \"str\" called from %s(%d) %s\n", str, file_err,
            line_err, func_err);
    fprintf(stderr, "------------------------------------------------------------"
                    "----------------\n");
    fprintf(stderr, "Register [rax] = " ELEM_MOD "\n", str->regs[rax]);
    fprintf(stderr, "Register [rbx] = " ELEM_MOD "\n", str->regs[rbx]);
    fprintf(stderr, "Register [rcx] = " ELEM_MOD "\n", str->regs[rcx]);
    fprintf(stderr, "Register [rdx] = " ELEM_MOD "\n", str->regs[rdx]);
    STACK_DUMP(&str->stk_cmd);
    STACK_DUMP(&str->stk_ptr);

    return ошибок_нет;
}

num_t *ptr_reg(Storage *str, int id_reg) { return &str->regs[id_reg]; }
