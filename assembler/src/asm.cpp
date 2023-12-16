#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"

int main()
{
    PRINT_INFO("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);

    OPEN_LOG_FIlE();

    FILE *fp_src = fopen(NAME_SRC_ASM, "r");
    CHECK_OPEN_FILE(fp_src);   

    FILE *fp_res = fopen(NAME_RES_ASM, "wb");
    CHECK_OPEN_FILE(fp_res);   

    create_bite_code(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
    
    CLOSE_LOG_FILE();
}

int create_bite_code(FILE *fp_src, FILE *fp_res)
{
    assert(fp_res != NULL);
    assert(fp_src != NULL);

    PRINT_LOG("\n\n\n---NEW_LOG_INFORMATION---\n");

    src_data src = {NULL, 0};

    ctor_src_arr(fp_src, &src);

    size_t sz_res_arr = calc_sz_res_arr(fp_src, &src);
    char   *res_arr   = (char*)calloc(sz_res_arr + 1, sizeof(char));

    PRINT_LOG("Size source file: %lu\n", sz_res_arr);
    PRINT_LOG("Pointer array commands and parameters: %p\n\n", res_arr);

    char  name_cmd[max_size_c] = "VENOM";
    char  name_reg[max_size_c] = "VENOM";
    int   num_cmd              = VENOM_NUM_CMD;
    int   jmp_id               = VENOM_NUM_CMD;
    int   org_id               = VENOM_NUM_CMD;
    bool  check_hlt            = false;

    size_t pc     = 0;
    int    src_pc = 0;
    int    ncr    = 0;  //ncr - number of characters read

    while (pc < sz_res_arr)
    {
        bool check_command = false;

        if (sscanf(src.src_arr + src_pc, "%s %n", name_cmd, &ncr) != 1)
        {
            fprintf(stderr, "ERROR: Don't read name command\n");
            return ERROR;
        }
        if (strcmp(name_cmd, "#") == 0)
        {
            while(src.src_arr[src_pc] != '\n')
                src_pc++;
            continue;
        }
        else
            src_pc += ncr;

        for (size_t j = 1; j <= NUMBER_INSTRUCTIONS; j++)
        {
            if (strcmp(name_cmd, commands[j]) == 0)
            {
                if (j == NUMBER_INSTRUCTIONS) // last index always hlt
                {
                    num_cmd   = cmd_hlt;
                    check_hlt = true;
                }
                else
                    num_cmd = (int)j; 

                check_command = true;
                break;
            }
        }

        if (!check_command)
        {
            fprintf(stderr, "ERROR: Command not found\n");
            return ERROR;
        }

        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num_cmd, RESET);
        
        num_t num = 0;
        if (num_cmd == cmd_push)
        {            
            if (sscanf(src.src_arr + src_pc, NUM_MOD_SCAN "\n%n", &num, &ncr) == 1)
            {
                res_arr[pc] = (char)cmd_push;
                memcpy(res_arr + pc + 1, &num, sizeof(num_t));
                
                PRINT_LOG("String source <%-4s " NUM_MOD_PRINT "> --> id_cmd[%-3lu] || cmd{%-2d} || param{" 
                           NUM_MOD_PRINT "} || reg[ %s ]\n", name_cmd, num, pc, res_arr[pc], num, "-");
                
                pc += 1 + sizeof(num_t);
            }
            else
            {
                sscanf (src.src_arr + src_pc, "%s \n%n", name_reg, &ncr);
                res_arr[pc]   = (char)cmd_rpush;
                res_arr[pc+1] = (char)check_num_reg(name_reg);
                
                PRINT_LOG("String source <%-4s %-9s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %d ]\n",
                            name_cmd, name_reg, pc, res_arr[pc], "  -----  ", res_arr[pc+1]);
                pc += 2;
            }
            
            src_pc += ncr;
        }
        else if (num_cmd == cmd_pop)
        {
            res_arr[pc] = (char)num_cmd;

            sscanf (src.src_arr + src_pc, "%s \n%n" , name_reg, &ncr);
            res_arr[pc+1] = (char)check_num_reg(name_reg);
            
            PRINT_LOG("String source <%-4s %-9s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %d ]\n",
                        name_cmd, name_reg, pc, res_arr[pc], "  -----  ", res_arr[pc+1]);

            pc += 2;
            src_pc += ncr;
        }
        else if (((num_cmd >= cmd_jmp) && (num_cmd <= cmd_jne)) || (num_cmd == cmd_call))
        {	
            res_arr[pc] = (char)num_cmd;

            sscanf(src.src_arr + src_pc, "%d \n%n", &jmp_id, &ncr);
            memcpy(res_arr + pc + 1, &jmp_id, sizeof(int));

            PRINT_LOG("String source <%-4s %-9d> --> id_cmd[%-3lu] || cmd{%-2d} || param{%-9d} || reg[ %s ]\n",
                        name_cmd, jmp_id, pc, res_arr[pc], jmp_id, "-");
            
            pc += 1 + sizeof(int);
            src_pc += ncr;
        }
        else if (num_cmd == dir_org)
        {
            sscanf(src.src_arr + src_pc, "%d \n%n", &org_id, &ncr);

            for (int i = (int)pc + 1; i < org_id; i++)
            {
                res_arr[i] = (char)cmd_hlt;
            }
            PRINT_LOG("String source <%-4s %-9d> --> id_cmd[%-3lu] || cmd{%-2d} || param{%-9d} || reg[ %s ]\n",
                        name_cmd, org_id, pc, res_arr[pc], org_id, "-");
            
            pc = (size_t)org_id;
            // PRINT_INFO("pc org = %lu\n", pc);
            src_pc += ncr;
        }
        else
        {	
            res_arr[pc] = (char)num_cmd;

            PRINT_LOG("String source <%-14s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %s ]\n",
                        name_cmd, pc, res_arr[pc], "  -----  ", "-");
            pc++;
        }
    }

    if (!check_hlt)
    {
        res_arr[sz_res_arr] = (char)cmd_hlt;
        
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, "hlt", RESET);
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);

        PRINT_LOG("!!! Hlt not found in src       --> id_cmd[%-3lu] || cmd{%-2d}\n",
                    sz_res_arr, res_arr[sz_res_arr]);
        
        fwrite(res_arr, sizeof(char), sz_res_arr + 1, fp_res);
    }
    else
        fwrite(res_arr, sizeof(char), sz_res_arr, fp_res);

    free(res_arr);
    free(src.src_arr);
    
    return 0;
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
            return ERROR_REG;
    }
}

size_t search_size_file(FILE *fp_src)
{
    assert(fp_src != NULL);

	long start_ftell = ftell(fp_src);
    fseek(fp_src, 0, SEEK_END);
    size_t size_file = (size_t)ftell(fp_src);
    fseek(fp_src, start_ftell, SEEK_SET);

    assert(size_file > 0);
    // PRINT_INFO(">>Size file: [%lu]\n", size_file);
    
    return size_file;
}

int ctor_src_arr(FILE *fp_src, src_data *file_data)
{
    long start_file = ftell(fp_src);

    size_t sz_fp_src = search_size_file(fp_src);
    char   *array    = (char*)calloc(sz_fp_src, sizeof(char));

    fread(array, sizeof(char), sz_fp_src, fp_src);

    file_data->sz_src_file   = sz_fp_src;
    file_data->src_arr = array;

    // for (int i = 0; i < file_data->sz_src_file; i++)
    //     PRINT_INFO("%c", array[i]);
    
    fseek(fp_src, start_file, SEEK_SET);
    return 0;
}

size_t calc_sz_res_arr(FILE *fp_src, src_data *file_data)
{
    assert(fp_src    != NULL);
    assert(file_data != NULL);

    char *array = file_data->src_arr;

    size_t sz_res_arr           = 0;
    char   name_cmd[max_size_c] = "VENOM";
    int    num_cmd              = VENOM_NUM_CMD;

    for (int i = 0, ncr = 0; i < (int)file_data->sz_src_file; )   //ncr - number of characters read
    {
        if (sscanf(array + i, "%s %n", name_cmd, &ncr) != 1)
        {
            fprintf(stderr, "ERROR: Don't read name command\n");
            return ERROR;
        }

        if (strcmp(name_cmd, "#") == 0)
        {
            while (array[i] != '\n')
                i++;
            continue;
        }
        else
            i += ncr;
                
        bool check_command = false;
        

        for (size_t j = 1; j <= NUMBER_INSTRUCTIONS; j++)
        {
            if (strcmp(name_cmd, commands[j]) == 0)
            {
                if (j == NUMBER_INSTRUCTIONS) // last index always hlt
                    num_cmd = cmd_hlt;
                else
                    num_cmd = (int)j; 

                check_command = true;
                break;
            }
        }

        if (!check_command)
        {
            fprintf(stderr, "ERROR: Command not found\n");
            return ERROR;
        }
        if (num_cmd != dir_org)
            sz_res_arr++;

        num_t num = 0;
        if (num_cmd == cmd_push)
        {            
            if (sscanf(array + i, NUM_MOD_SCAN "\n%n", &num, &ncr) == 1)
            {
                i += ncr;
                sz_res_arr += sizeof(num_t);
            }
            else
            {
                sz_res_arr++;
                i += len_name_reg + 1;
            }
        }
        else if (num_cmd == cmd_pop)
        {
            sz_res_arr++;
            i += len_name_reg + 1;
        }
        else if (((num_cmd >= cmd_jmp) && (num_cmd <= cmd_jne)) || (num_cmd == cmd_call))
        {
            sz_res_arr += sizeof(int);
            int jmp_id = 0;
            sscanf(array + i, "%d \n%n", &jmp_id, &ncr);
            i += ncr;
        }
        else if (num_cmd == dir_org)
        {
            int org_id = 0;
            sscanf(array + i, "%d \n%n", &org_id, &ncr);
            
            if (((size_t)org_id < sz_res_arr) || org_id < 0)
            {
                fprintf(stderr, "ERROR: The org directive points to an occupied memory location\n");
                return ERROR;
            }

            sz_res_arr = (size_t)org_id;
            i += ncr;
        }
    }

    return sz_res_arr;
}
