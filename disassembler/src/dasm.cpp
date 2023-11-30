#include "../include/dasm.h"
#include "../../library/commands.h"
#include <stdlib.h>

int main()
{
    FILE *fp_src = fopen(NAME_SRC_DASM, "rb");
	if (fp_src == NULL)
	{
		printf("ERROR: Don't open source file\n");
		return ERROR;
	}

    FILE *fp_res = fopen(NAME_RES_DASM, "w");
	if (fp_res == NULL)
	{
		printf("ERROR: Don't open source file\n");
		return ERROR;
	}       

    write_file(fp_src, fp_res);
	
    fclose(fp_src);
    fclose(fp_res);
}

char *create_buf(FILE *fp_src, size_t *sz_file)
{
	size_t start = ftell(fp_src);
	fseek(fp_src, 0L, SEEK_END);
	*sz_file = ftell(fp_src);
	fseek(fp_src, start, SEEK_SET);

	char *buf = (char*)calloc(*sz_file + 1, sizeof(char));
	fread(buf, sizeof(char), *sz_file, fp_src);
	buf[*sz_file] = '\0';

	fseek(fp_src, start, SEEK_SET);
	return buf;
}

int write_file(FILE *fp_src, FILE *fp_res)
{
	assert(fp_res != NULL);
	assert(fp_src != NULL);
	
	size_t sz_src_file = 0;
	char *buf = create_buf(fp_src, &sz_src_file);

	size_t number_commands = sz_src_file / sizeof(num_t);

	double num_command         = VENOM_NUM_COMMAND;
	char  name_reg[max_size_c] = "VENOM";
    num_t num                  = 0;

    for (size_t pc = 0; pc < number_commands; )
    {
		num_command = *((double*)buf + pc);

        // if (sscanf(buf + pc*sizeof(num_t), NUM_MOD_SCAN, &num_command) != 1)
		// {
		// 	fprintf(stderr, "ERROR: Don`t num command\n");
		// 	return ERROR;
		// }

        if (num_command == hlt)
		{
			fprintf(fp_res, "%s", commands[NUMBER_INSTRUCTIONS]);  // index hlt always = NUMBER_INSTRUCTIONS			
			pc++;
		}
		else if (num_command == push)
		{
            fprintf(fp_res, "%s ", commands[push]);
			num = *((double*)buf + pc + 1);
            fprintf(fp_res, NUM_MOD_PRINT "\n", num);
			pc += 2;    
        }
		else if (num_command == r_push)
		{
            fprintf(fp_res, "%s " , commands[push]);
			num = *((double*)buf + pc + 1);
			fprintf(fp_res, "%s\n", REGISTER[(int)num]);
			pc += 2;    
		}
		else if (num_command == pop)
		{
            fprintf(fp_res, "%s " , commands[pop]);
			num = *((double*)buf + pc + 1);
            fprintf(fp_res, "%s\n", REGISTER[(int)num]);    
			pc += 2;
		}
		else
		{
            fprintf(fp_res, "%s\n", commands[(int)num_command]);
			pc++;
		}
    }
	
	free(buf);

	return 0;
}

