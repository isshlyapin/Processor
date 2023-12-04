#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"

int main()
{
    PRINT_INFO("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);

    OPEN_LOG_FIlE();

    FILE *fp_src = fopen(NAME_SRC_ASM, "r");
    if (fp_src == NULL)
    {
        fprintf(stderr, "ERROR: Don't open source file\n");
        return ERROR;
    }   

    FILE *fp_res = fopen(NAME_RES_ASM, "wb");
    if (fp_res == NULL)
    {
        fprintf(stderr, "ERROR: Don't open result file\n");
        return ERROR;
    }   

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

    size_t sz_array_cmd_and_prm = calculate_size_array_cmd_and_parameters(fp_src);
    char   *array_cmd_and_param = (char*)calloc(sz_array_cmd_and_prm + 1, sizeof(char));

    PRINT_LOG("Size source file: %lu\n", sz_array_cmd_and_prm);
    PRINT_LOG("Pointer array commands and parameters: %p\n\n", array_cmd_and_param);

    char  name_command[max_size_c]  = "VENOM";
    char  name_reg[max_size_c]      = "VENOM";
    int   num_command               = VENOM_NUM_COMMAND;
    int   jmp_id                    = VENOM_NUM_COMMAND;
    bool  check_hlt                 = false;

    for (size_t pc = 0; pc < sz_array_cmd_and_prm; )
    {
        bool check_command = false;

        if (fscanf(fp_src, "%s", name_command) != 1)
        {
            fprintf(stderr, "ERROR: Don't read name command\n");
            return ERROR;
        }

        for (size_t j = 1; j <= NUMBER_INSTRUCTIONS; j++)
        {
            if (strcmp(name_command, commands[j]) == 0)
            {
                if (j == NUMBER_INSTRUCTIONS) // last index always hlt
                {
                    num_command = cmd_hlt;
                    check_hlt = true;
                }
                else
                    num_command = (int)j; 

                check_command = true;
                break;
            }
        }

        if (!check_command)
        {
            fprintf(stderr, "ERROR: Command not found\n");
            return ERROR;
        }

        PRINT_INFO("Name_command: %s[%4s]%s", RED, name_command, RESET);
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num_command, RESET);
        
        num_t num = 0;
        if (num_command == cmd_push)
        {            
            if (fscanf(fp_src, NUM_MOD_SCAN, &num) != 1)
            {
                fscanf (fp_src, "%s" , name_reg);
                array_cmd_and_param[pc]   = (char)cmd_rpush;
                array_cmd_and_param[pc+1] = (char)check_num_reg(name_reg);
                
                PRINT_LOG("String source <%-4s %-9s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %d ]\n",
                            name_command, name_reg, pc, array_cmd_and_param[pc], "  -----  ", array_cmd_and_param[pc+1]);
                pc += 2;
            }
            else
            {
                array_cmd_and_param[pc] = (char)cmd_push;
                memcpy(array_cmd_and_param + pc + 1, &num, sizeof(num_t));
                
                PRINT_LOG("String source <%-4s " NUM_MOD_PRINT "> --> id_cmd[%-3lu] || cmd{%-2d} || param{" NUM_MOD_PRINT "} || reg[ %s ]\n",
                            name_command, num, pc, array_cmd_and_param[pc], num, "-");
                pc += 1 + sizeof(num_t);
            }
        }
        else
        {
            array_cmd_and_param[pc] = (char)num_command;

            if (num_command == cmd_pop)
            {
                fscanf (fp_src, "%s" , name_reg);
                array_cmd_and_param[pc+1] = (char)check_num_reg(name_reg);
                
                PRINT_LOG("String source <%-4s %-9s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %d ]\n",
                            name_command, name_reg, pc, array_cmd_and_param[pc], "  -----  ", array_cmd_and_param[pc+1]);

                pc++;
            }
            else if (num_command >= cmd_jmp && num_command <= cmd_jne)
            {	
                fscanf(fp_src, "%d", &jmp_id);
                memcpy(array_cmd_and_param + pc + 1, &jmp_id, sizeof(int));

                PRINT_LOG("String source <%-4s %-9d> --> id_cmd[%-3lu] || cmd{%-2d} || param{%-9d} || reg[ %s ]\n",
                            name_command, jmp_id, pc, array_cmd_and_param[pc], jmp_id, "-");
                
                pc += sizeof(int);
            }
            else
            {	
                PRINT_LOG("String source <%-14s> --> id_cmd[%-3lu] || cmd{%-2d} || param{%9s} || reg[ %s ]\n",
                            name_command, pc, array_cmd_and_param[pc], "  -----  ", "-");
            }

            pc++;
        }
    }

    if (!check_hlt)
    {
        array_cmd_and_param[sz_array_cmd_and_prm] = (char)cmd_hlt;
        
        PRINT_INFO("Name_command: %s[%4s]%s", RED, "hlt", RESET);
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);

        PRINT_LOG("!!! Hlt not found in src       --> id_cmd[%-3lu] || cmd{%-2d}\n",
                    sz_array_cmd_and_prm, array_cmd_and_param[sz_array_cmd_and_prm]);
        
        fwrite(array_cmd_and_param, sizeof(char), sz_array_cmd_and_prm + 1, fp_res);
    }
    else
        fwrite(array_cmd_and_param, sizeof(char), sz_array_cmd_and_prm, fp_res);

    free(array_cmd_and_param);

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

size_t calculate_size_array_cmd_and_parameters(FILE *fp_src)
{
    assert(fp_src != NULL);

    long start_file = ftell(fp_src);

    size_t sz_array_cmd_and_prm     = 0;

    char   name_command[max_size_c] = "VENOM";
    char   name_reg[max_size_c]     = "VENOM";
    int    num_command              = VENOM_NUM_COMMAND;

    while (!feof(fp_src))
    {
        if (fscanf(fp_src, "%s", name_command) != 1)
        {
            fprintf(stderr, "ERROR: Don't read name command\n");
            return ERROR;
        }

        bool check_command = false;
        for (size_t j = 1; j <= NUMBER_INSTRUCTIONS; j++)
        {
            if (strcmp(name_command, commands[j]) == 0)
            {
                if (j == NUMBER_INSTRUCTIONS) // last index always hlt
                    num_command = cmd_hlt;
                else
                    num_command = (int)j; 
                
                check_command = true;
                break;
            }
        }

        if (!check_command)
        {
            fprintf(stderr, "ERROR: Command not found\n");
            return ERROR;
        }
        sz_array_cmd_and_prm++;

        num_t num = 0;
        if (num_command == cmd_push)
        {            
            if (fscanf(fp_src, NUM_MOD_SCAN, &num) != 1)
            {
                sz_array_cmd_and_prm++;
                fscanf(fp_src, "%s", name_reg);
            }
            else
                sz_array_cmd_and_prm += sizeof(num_t);
        }
        else
        {
            if (num_command == cmd_pop)
            {
                sz_array_cmd_and_prm++;
                fscanf(fp_src, "%s", name_reg);
            }
            else if (num_command >= cmd_jmp && num_command <= cmd_jne)
            {
                sz_array_cmd_and_prm += sizeof(int);
                fscanf(fp_src, NUM_MOD_SCAN, &num);
            }
        }
    }

    fseek(fp_src, start_file, SEEK_SET);
    return sz_array_cmd_and_prm;
}
