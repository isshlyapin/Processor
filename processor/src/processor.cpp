#include "../include/processor.h"
#include "../../library/stack.h"
#include "../../library/commands.h"
#include "../../library/color.h"

static const size_t MAX_SIZE_STR        = 100;
static const char   VENOM_NUM_COMMAND   = -13;

static const double EPSILON = 1e-9;

int process_commands(FILE *fp_src, Storage *str)
{
    PRINT_INFO("\n___%sWORKING PROCESSOR%s___\n\n", GREEN, RESET);

    char num_command = VENOM_NUM_COMMAND;

    size_t sz_src_file = 0;
    char *array_cmd_and_prm = create_array_cmd_and_prm(fp_src, &sz_src_file);

    for (size_t pc = 0; pc < sz_src_file;)
    {
        num_command = array_cmd_and_prm[pc];

        PRINTF_INFO_CMD();
        
        num_t  num1   = VENOM_ELEM;
        num_t  num2   = VENOM_ELEM;
        char   id_reg = -13;
        int    id_jmp = 0;
        ELEM_T id_ret = 0;

        switch (num_command)
        {
            case cmd_push:
                memcpy(&num1, array_cmd_and_prm + pc + 1, sizeof(num_t));
                StackPush(&str->stk_cmd, num1); 
                pc += 1 + sizeof(num_t);

                PRINT_INFO("Numeric from file: <%.2lf>\n", num1);
                break;

            case cmd_rpush:
                id_reg = array_cmd_and_prm[pc+1];				
                StackPush(&str->stk_cmd, *ptr_reg(str, (int)id_reg));
                pc += 2;

                PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)id_reg]);
                break;	

            case cmd_pop:
                id_reg = array_cmd_and_prm[pc+1];				
                StackPop(&str->stk_cmd, ptr_reg(str, (int)id_reg));
                pc += 2;
                
                PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)id_reg]);				
                break;

            case cmd_sqrt:
                CMD_SQRT(num1);
                pc++;
                break;

            case cmd_sin:
                CMD_SIN(num1)
                pc++;
                break;

            case cmd_cos:
                CMD_COS(num1);
                pc++;
                break;

            case cmd_in:
                CMD_IN(num1);
                pc++;
                
                PRINT_INFO("Numeric user: <%.2lf>\n", num1); ////				
                break;

            case cmd_add:
                CMD_ADD(num1, num2);
                pc++;
                break;

            case cmd_sub:
                CMD_SUB(num1, num2);
                pc++;
                break;

            case cmd_mul:
                CMD_MUL(num1, num2);
                pc++;
                break;

            case cmd_div:
                CMD_DIV(num1, num2);
                pc++;
                break;

            case cmd_jmp:
                memcpy(&id_jmp, array_cmd_and_prm+ pc + 1, sizeof(int));
                pc = (size_t)id_jmp;
                break;
            
            case cmd_jb:
                JMP_IF(num2 < num1, num1, num2);
                break;
            
            case cmd_jbe:
                JMP_IF(num2 <= num1, num1, num2);
                break;
            
            case cmd_ja:
                JMP_IF(num2 > num1, num1, num2);
                break;

            case cmd_jae:
                JMP_IF(num2 >= num1, num1, num2);
                break;
            
            case cmd_je:
                JMP_IF(fabs(num2 - num1) < EPSILON, num1, num2);
                break;

            case cmd_jne:
                JMP_IF(fabs(num2 - num1) > EPSILON, num1, num2);
                break;

            case cmd_call:
                memcpy(&id_jmp, array_cmd_and_prm + pc + 1, sizeof(int));
                StackPush(&str->stk_ptr, (ELEM_T)pc + 1 + sizeof(int));
                pc = (size_t)id_jmp;
                break;

            case cmd_ret:
                StackPop(&str->stk_ptr, &id_ret);
                pc = (size_t)id_ret;
                break;
            
            case cmd_out:
                CMD_OUT(num1);
                pc++;
                break;

            case cmd_hlt:
                pc++;
                free(array_cmd_and_prm);
                return 0;
                break;
            
            default:
                break;
        }
        PRINT_INFO("\n");
    }

    free(array_cmd_and_prm);
    return 0;
}

int StorageCtor(Storage *str)                        
{
    StackCtor(&str->stk_cmd, std_stack_cap);
    StackCtor(&str->stk_ptr, std_stack_cap);
    str->rax = VENOM_ELEM;
    str->rbx = VENOM_ELEM;
    str->rcx = VENOM_ELEM;
    str->rdx = VENOM_ELEM;

    return 0;
}

int StorageDtor(Storage *str)
{
    StackDtor(&str->stk_cmd);
    StackDtor(&str->stk_ptr);
    str->rax  = VENOM_ELEM;
    str->rbx  = VENOM_ELEM;
    str->rcx  = VENOM_ELEM;
    str->rdx  = VENOM_ELEM;

    return 0;
}

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err)
{
    fprintf(stderr, "----------------------------------------------------------------------------\n");
    fprintf(stderr, "Storage[%p] \"str\" called from %s(%d) %s\n", str, file_err, line_err, func_err);
    fprintf(stderr, "----------------------------------------------------------------------------\n");
    fprintf(stderr, "Register [ax] = " ELEM_MOD "\n", str->rax);
    fprintf(stderr, "Register [bx] = " ELEM_MOD "\n", str->rbx);
    fprintf(stderr, "Register [cx] = " ELEM_MOD "\n", str->rcx);
    fprintf(stderr, "Register [dx] = " ELEM_MOD "\n", str->rdx);
    STACK_DUMP(&str->stk_cmd);
    STACK_DUMP(&str->stk_ptr);
    return 0;
}

char *create_array_cmd_and_prm(FILE *fp_src, size_t *sz_file)
{
    long start = ftell(fp_src);
    fseek(fp_src, 0L, SEEK_END);
    *sz_file = (size_t)ftell(fp_src);
    fseek(fp_src, start, SEEK_SET);

    char *array_cmd_and_prm = (char*)calloc(*sz_file + 1, sizeof(char));
    fread(array_cmd_and_prm, sizeof(char), *sz_file, fp_src);
    array_cmd_and_prm[*sz_file] = '\0';

    fseek(fp_src, start, SEEK_SET);
    return array_cmd_and_prm;
}

num_t *ptr_reg(Storage *str, int id_reg)
{
    switch (id_reg)
    {
        case rax:
            return &str->rax;
            break;
        
        case rbx:
            return &str->rbx;
            break;

        case rcx:
            return &str->rcx;
            break;

        case rdx:
            return &str->rdx;
            break;

        default:
            return NULL;
    }
}