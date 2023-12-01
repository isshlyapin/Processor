#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"

int main()
{
	#ifdef INFO
	printf("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);
	#endif

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
}

int create_bite_code(FILE *fp_src, FILE *fp_res)
{
	assert(fp_res != NULL);
	assert(fp_src != NULL);

	size_t sz_src_file = 0;
	char   *buf        = create_buf(fp_src, &sz_src_file);

	int   number_commands = search_size_new_buf(buf, sz_src_file);
	num_t *new_buf        = (num_t*)calloc(number_commands + 1, sizeof(num_t));

    char  name_command[max_size_c]  = "VENOM";
	char  name_reg[max_size_c]      = "VENOM";
    int   num_command               = VENOM_NUM_COMMAND;
    bool  check_hlt                 = false;

	for (size_t i = 0; i < number_commands; )
    {
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
                    num_command = j; 
             
			    break;
            }
        }
		
		#ifdef INFO
        printf("Name_command: %s[%4s]%s", RED, name_command, RESET);
        printf("%s[%2d]%s\n", MAGENTA, num_command, RESET);
		#endif

		num_t num = 0;
		if (num_command == cmd_push)
		{            
			if (fscanf(fp_src, NUM_MOD_SCAN, &num) != 1)
			{
				fscanf (fp_src, "%s" , name_reg);
				new_buf[i]   = cmd_rpush;
				new_buf[i+1] = check_num_reg(name_reg);
			}
			else
			{
				new_buf[i]   = cmd_push;
				new_buf[i+1] = num;
			}
			i += 2;
		}
		else
		{
			new_buf[i] = num_command;

			if (num_command == cmd_pop)
			{
				fscanf (fp_src, "%s" , name_reg);
				new_buf[i+1] = check_num_reg(name_reg);
				i++;
			}
			else if (num_command >= cmd_jmp && num_command <= cmd_jne)
			{	
				fscanf(fp_src, NUM_MOD_SCAN, &num);
				new_buf[i+1] = num;
				i++;
			}
			i++;
		}
	}

	if (!check_hlt)
	{
		new_buf[number_commands] = cmd_hlt;
		
		#ifdef INFO
		printf("Name_command: %s[%4s]%s", RED, "hlt", RESET);
        printf("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);
		#endif

		fwrite(new_buf, sizeof(num_t), number_commands + 1, fp_res);
	}
	else
		fwrite(new_buf, sizeof(num_t), number_commands, fp_res);
	
	free(buf);
	free(new_buf);

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

int search_size_new_buf(char *buf, size_t sz_buf)
{
	int number_commands = 0;
	
	if (buf[sz_buf-1] != '\n')
		number_commands++;

	for (int i = 0; i < sz_buf; i++)
		if (buf[i] == '\n' | buf[i] == '\0' | buf[i] == ' ')
			number_commands++;

	return number_commands;
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