#include "../include/dasm.h"
#include "../../library/commands.h"
#include <stdlib.h>
#include "../../library/color.h"

int main()
{
    #ifdef INFO
    printf("\n___%sWORKING DISASSEMBLER%s___\n\n", CYAN, RESET);
    #endif

    FILE *fp_src = fopen(NAME_SRC_DASM, "rb");
    if (fp_src == NULL)
    {
        fprintf(stderr, "ERROR: Don't open source file\n");
        return ERROR;
    }

    FILE *fp_res = fopen(NAME_RES_DASM, "w");
    if (fp_res == NULL)
    {
        fprintf(stderr, "ERROR: Don't open source file\n");
        return ERROR;
    }       

    bite_code_in_text(fp_src, fp_res);

    fclose(fp_src);
    fclose(fp_res);
}

int bite_code_in_text(FILE *fp_src, FILE *fp_res)
{
    assert(fp_res != NULL);
    assert(fp_src != NULL);

    size_t sz_src_file = 0;
    char *buf = create_buf(fp_src, &sz_src_file);

    char  num_command          = VENOM_NUM_COMMAND;
    num_t num                  = 0;
    int   jmp_id               = 0;

    for (size_t pc = 0; pc < sz_src_file; )
    {
        num_command = buf[pc];

        if (num_command == cmd_hlt)
        {
            fprintf(fp_res, "%s", commands[NUMBER_INSTRUCTIONS]);  // index hlt always = NUMBER_INSTRUCTIONS			
            pc++;

            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[NUMBER_INSTRUCTIONS], RESET);
            #endif
        }
        else if (num_command == cmd_push)
        {
            fprintf(fp_res, "%s ", commands[cmd_push]);
            memcpy(&num, buf + pc + 1, sizeof(num_t));
            fprintf(fp_res, NUM_MOD_PRINT "\n", num);
            pc += 1 + sizeof(num_t);    
        
            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[cmd_push], RESET);
            #endif
        }
        else if (num_command == cmd_rpush)
        {
            fprintf(fp_res, "%s " , commands[cmd_push]);
            fprintf(fp_res, "%s\n", REGISTER[(int)buf[pc+1]]);
            pc += 2;    

            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[cmd_push], RESET);
            #endif
        }
        else if (num_command == cmd_pop)
        {
            fprintf(fp_res, "%s " , commands[cmd_pop]);
            fprintf(fp_res, "%s\n", REGISTER[(int)buf[pc+1]]);    
            pc += 2;

            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[cmd_pop], RESET);
            #endif 
        }
        else if (num_command >= cmd_jmp && num_command <= cmd_jne)
        {
            fprintf(fp_res, "%s " , commands[(int)num_command]);
            memcpy(&jmp_id, buf + pc + 1, sizeof(int));
            fprintf(fp_res, "%d\n", jmp_id);    
            pc += 1 + sizeof(int);

            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[(int)num_command], RESET);
            #endif
        }
        else
        {
            fprintf(fp_res, "%s\n", commands[(int)num_command]);
            pc++;
            
            #ifdef INFO
            printf("Name_command: %s[%4s]%s", MAGENTA, commands[(int)num_command], RESET);
            #endif
        }
        #ifdef INFO
        printf("%s[%2d]%s\n", RED, (int)num_command, RESET);
        #endif
    }

    free(buf);

    return 0;
}

char *create_buf(FILE *fp_src, size_t *sz_file)
{
    long start_file = ftell(fp_src);
    fseek(fp_src, 0L, SEEK_END);
    *sz_file = (size_t)ftell(fp_src);
    fseek(fp_src, start_file, SEEK_SET);

    char *buf = (char*)calloc(*sz_file + 1, sizeof(char));
    fread(buf, sizeof(char), *sz_file, fp_src);
    buf[*sz_file] = '\0';

    fseek(fp_src, start_file, SEEK_SET);
    return buf;
}
