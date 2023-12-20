#include <cstdio>
#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"
#include "../../library/error.h"

int main(int argc, char *argv[])
{
    PRINT_INFO("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);

    char test_ch = 2;
    printf("<%d><%c>\n", test_ch, (test_ch | 130));

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

    PRINT_LOG("\n\n\n---NEW_LOG_INFORMATION---\n");
    
    struct Array src_arr = {NULL, 0};

    ctor_src_arr(fp_src, &src_arr);

    size_t sz_res_arr = calc_sz_res_arr(fp_src, &src_arr);
    char *res_arr   = (char*)calloc(sz_res_arr + 1, sizeof(char));

    PRINT_LOG("Size source file: %lu\n", sz_res_arr);
    PRINT_LOG("Pointer array commands and parameters: %p\n\n", res_arr);

    char  name_cmd[MAX_SIZE_STR] = "VENOM";
    int   num_cmd                = VENOM_NUM_CMD;

    size_t pc     = 0;
    int    src_pc = 0;

    while (pc < sz_res_arr)
    {
        int ncr = 0;  // ncr - number of characters read
        
        bool read_cmd = sscanf(src_arr.arr_ptr + src_pc, "%s %n", name_cmd, &ncr);
        if (!read_cmd)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_READ_CMD]);
            return ERROR_READ_CMD;
        }

        COMMENT_PROCESS(src_arr.arr_ptr, src_pc, ncr);
        
        if (command_process(name_cmd, &num_cmd) == ERROR_NAME_CMD)
            return ERROR_NAME_CMD;
             
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num_cmd, RESET);

        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , name_cmd, num_cmd);
        
        num_t num = 0;
        if (num_cmd == cmd_push)
        {            
            if (sscanf(src_arr.arr_ptr + src_pc, NUM_MOD_SCAN "\n%n", &num, &ncr) == 1)
            {
                res_arr[pc] = (char)cmd_push;
                memcpy(res_arr + pc + 1, &num, sizeof(num_t));
                
                PRINT_LOG("PRM<" NUM_MOD_PRINT "> CMD_ID <%lu>\n", num, pc);
                
                pc += sizeof(num_t);
            }
            else
            {
                sscanf (src_arr.arr_ptr + src_pc, "%s \n%n", name_cmd, &ncr);
                res_arr[pc]   = 53;
                res_arr[pc+1] = (char)check_num_reg(name_cmd);
                
                if (res_arr[pc+1] == ERROR_REG_NAME)
                {
                    fprintf(stderr, "%s\n", ERROR_TEXT[5]); // 5 - ERROR_REG
                    return ERROR_REG_NAME;
                }
                
                PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", name_cmd, pc);
                pc++;
            }            
        }
        else if (num_cmd == cmd_pop)
        {
            res_arr[pc] = (char)num_cmd;

            sscanf (src_arr.arr_ptr + src_pc, "%s \n%n" , name_cmd, &ncr);
            res_arr[pc+1] = (char)check_num_reg(name_cmd);
            if (res_arr[pc+1] == ERROR_REG_NAME)
            {
                fprintf(stderr, "%s\n", ERROR_TEXT[5]); // 5 - ERROR_REG
                return ERROR_REG_NAME;
            }            

            PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", name_cmd, pc);
            pc++;
        }
        else if (NUM_CMD_EQUALS_JMP_OR_CALL(num_cmd))
        {	
            res_arr[pc] = (char)num_cmd;

            sscanf(src_arr.arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr);
            memcpy(res_arr + pc + 1, &num_cmd, sizeof(int));

            PRINT_LOG("PRM<%-9d> CMD_ID <%lu>\n", num_cmd, pc);
            pc += sizeof(int);
        }
        else if (num_cmd == dir_org)
        {
            sscanf(src_arr.arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr);

            if (ORG_NOT_CORRECT(num_cmd, pc + 1))
            {
                fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_ORG_DIR]);
                return ERROR_ORG_DIR;
            }

            for (int i = (int)pc + 1; i < num_cmd; i++)
                res_arr[i] = (char)cmd_hlt;
            
            PRINT_LOG("PRM<%-9d> CMD_ID <%s>\n", num_cmd, "---");
            pc = (size_t)num_cmd - 1;    
        }
        else
        {	
            res_arr[pc] = (char)num_cmd;
            ncr = 0;
            PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", "---", pc);
        }

        pc++;
        src_pc += ncr;
    }

    res_arr[sz_res_arr] = (char)cmd_hlt;
    
    PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[cmd_hlt] + 4, cmd_hlt);
    PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", "---", sz_res_arr);

    PRINT_INFO("name_cmd: %s[%4s]%s", RED, "hlt", RESET);
    PRINT_INFO("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);

    fwrite(res_arr, sizeof(char), sz_res_arr + 1, fp_res);

    free(res_arr);
    free(src_arr.arr_ptr);
    
    return WITHOUT_ERROR;
}

int command_process(const char *name_cmd, int* num_cmd)
{
    bool check_command = false;

    #define NEW_INSTRUCTIONS(name, num, ...)    \
        if (strcmp(name_cmd, #name + 4) == 0)   \
        {                                       \
            *num_cmd      = num;                \
            check_command = true;               \
        }                    

    #define NEW_DIRECTIVE(name, num, ...)       \
        if (strcmp(name_cmd, #name + 4) == 0)   \
        {                                       \
            *num_cmd      = num;                \
            check_command = true;               \
        }                    

    #include "../../library/test-def-cmd.txt"
    #undef NEW_INSTRUCTIONS
    #undef NEW_DIRECTIVE

    if (!check_command)
    {
        fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_NAME_CMD]);
        return ERROR_NAME_CMD;
    }

    return WITHOUT_ERROR;
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
            return ERROR_REG_NAME;
    }
}

int ctor_src_arr(FILE *fp_src, struct Array *file_data)
{
    long start_ptr_file = ftell(fp_src);

    size_t sz_file_src = search_size_file(fp_src);
    char   *src_array  = (char*)calloc(sz_file_src + 1, sizeof(char));

    fread(src_array, sizeof(char), sz_file_src, fp_src);

    file_data->arr_ptr  = src_array;
    file_data->size_arr = sz_file_src;
    
    fseek(fp_src, start_ptr_file, SEEK_SET);
    
    return WITHOUT_ERROR;
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

size_t calc_sz_res_arr(FILE *fp_src, struct Array *file_data)
{
    assert(fp_src    != NULL);
    assert(file_data != NULL);

    char *src_array = file_data->arr_ptr;

    size_t sz_res_arr             = 0;
    char   name_cmd[MAX_SIZE_STR] = "VENOM";
    int    num_cmd                = VENOM_NUM_CMD;

    for (int i = 0, ncr = 0; i < (int)file_data->size_arr; )   //ncr - number of characters read
    {
        if (sscanf(src_array + i, "%s %n", name_cmd, &ncr) != 1)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_READ_CMD]);
            return ERROR_READ_CMD;
        }

        COMMENT_PROCESS(src_array, i, ncr);

        if (command_process(name_cmd, &num_cmd) == ERROR_NAME_CMD)
            return ERROR_NAME_CMD;
        sz_res_arr++;

        num_t num = 0;
        if (num_cmd == cmd_push)
        {            
            if (sscanf(src_array + i, NUM_MOD_SCAN "\n%n", &num, &ncr) == 1)
            {
                i += ncr;
                sz_res_arr += sizeof(num_t);
            }
            else
            {
                sz_res_arr++;
                i += LEN_NAME_REG + 1;
            }
        }
        else if (num_cmd == cmd_pop)
        {
            sz_res_arr++;
            i += LEN_NAME_REG + 1;
        }
        else if (NUM_CMD_EQUALS_JMP_OR_CALL(num_cmd))
        {
            sz_res_arr += sizeof(int);
            sscanf(src_array + i, "%d \n%n", &num_cmd, &ncr);
            i += ncr;
        }
        else if (num_cmd == dir_org)
        {
            sscanf(src_array + i, "%d \n%n", &num_cmd, &ncr);
            
            if (ORG_NOT_CORRECT(num_cmd, sz_res_arr))
            {
                fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_ORG_DIR]);
                return ERROR_ORG_DIR;
            }

            sz_res_arr = (size_t)num_cmd;
            i += ncr;
        }
    }

    return sz_res_arr;
}
