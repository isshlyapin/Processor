#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"

int main()
{
	#ifdef INFO
	printf("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);
	#endif

	FILE *fp_log = NULL;
	#ifdef LOG
	fp_log = fopen(NAME_LOG_ASM, "a");
	if (fp_log == NULL)
	{
		fprintf(stderr, "ERROR: Don't open source file\n");
		return ERROR;
	}   
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
		
    create_bite_code(fp_src, fp_res, fp_log);

	// printf("<%lu>\n", calculate_size_array_cmd_and_parameters(fp_src));

    fclose(fp_res);
    fclose(fp_src);
	#ifdef LOG
	fclose(fp_log);
	#endif
}

int create_bite_code(FILE *fp_src, FILE *fp_res, FILE *fp_log)
{
	assert(fp_res != NULL);
	assert(fp_src != NULL);

	#ifdef LOG
	assert(fp_log != NULL);
	fprintf(fp_log, "\n\n\n---NEW_LOG_INFORMATION---\n");
	#endif
	
	size_t sz_array_cmd_and_prm = calculate_size_array_cmd_and_parameters(fp_src);
	char   *array_cmd_and_param = (char*)calloc(sz_array_cmd_and_prm + 1, sizeof(char));

	#ifdef LOG
	fprintf(fp_log, "Size source file: %lu\n", sz_array_cmd_and_prm);
	fprintf(fp_log, "Pointer array commands and parameters: %p\n\n", array_cmd_and_param);
	#endif 

    char  name_command[max_size_c]  = "VENOM";
	char  name_reg[max_size_c]      = "VENOM";
    int   num_command               = VENOM_NUM_COMMAND;
	int   jmp_id                    = VENOM_NUM_COMMAND;
    bool  check_hlt                 = false;
	bool  check_command             = false;

	for (size_t pc = 0; pc < sz_array_cmd_and_prm; )
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
		else
			check_command = false;

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
				array_cmd_and_param[pc]   = (char)cmd_rpush;
				array_cmd_and_param[pc+1] = (char)check_num_reg(name_reg);
				
				// #ifdef LOG
				// fprintf(fp_log, "String source <%-4s %-3s> --> array_cmd_and_param[%-3lu] = {%-2.0lf} array_cmd_and_param[%-3lu] = {%-5.0lf}\n",
				// 			name_command, name_reg, i, array_cmd_and_param[i], i+1, array_cmd_and_param[i+1]); 
				// #endif

				pc += 2;
			}
			else
			{
				array_cmd_and_param[pc]                 = (char)cmd_push;
				*((num_t*)(array_cmd_and_param + pc + 1)) = num;
				
				// #ifdef LOG
				// fprintf(fp_log, "String source <%-4s %-3.0lf> --> array_cmd_and_param[%-3lu] = {%-2.0lf} array_cmd_and_param[%-3lu] = {%-5.0lf}\n",
				// 			name_command, num, i, array_cmd_and_param[i], i+1, array_cmd_and_param[i+1]); 
				// #endif

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
				
				// #ifdef LOG
				// fprintf(fp_log, "String source <%-4s %-3s> --> array_cmd_and_param[%-3lu] = {%-2.0lf} array_cmd_and_param[%-3lu] = {%-5.0lf}\n",
				// 			name_command, name_reg, i, array_cmd_and_param[i], i+1, array_cmd_and_param[i+1]); 
				// #endif

				pc++;
			}
			else if (num_command >= cmd_jmp && num_command <= cmd_jne)
			{	
				fscanf(fp_src, "%d", &jmp_id);
				*((int*)(array_cmd_and_param + pc + 1)) = jmp_id;
				
				// #ifdef LOG
				// fprintf(fp_log, "String source <%-4s %-3.0lf> --> array_cmd_and_param[%-3lu] = {%-2.0lf} array_cmd_and_param[%-3lu] = {%-5.0lf}\n",
				// 			name_command, num, i, array_cmd_and_param[i], i+1, array_cmd_and_param[i+1]); 
				// #endif
				
				pc += sizeof(int);
			}
			// else
			// {
			// 	#ifdef LOG
			// 	fprintf(fp_log, "String source <%-8s> --> array_cmd_and_param[%-3lu] = {%-2.0lf}\n",
			// 				name_command, i, array_cmd_and_param[i]); 
			// 	#endif
			// }

			pc++;
		}
	}

	if (!check_hlt)
	{
		array_cmd_and_param[sz_array_cmd_and_prm] = (char)cmd_hlt;
		
		#ifdef INFO
		printf("Name_command: %s[%4s]%s", RED, "hlt", RESET);
        printf("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);
		#endif

		// #ifdef LOG
		// fprintf(fp_log, "In source file no \"hlt\". Add hlt --> array_cmd_and_param[%d] = {%3.0lf}\n",
		// 			number_commands, array_cmd_and_param[number_commands]); 
		// #endif
		
		fwrite(array_cmd_and_param, sizeof(char), sz_array_cmd_and_prm + 1, fp_res);
	}
	else
		fwrite(array_cmd_and_param, sizeof(num_t), sz_array_cmd_and_prm, fp_res);
	
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

// size_t search_size_array_cmd_and_param(char *buf, size_t sz_buf)
// {
// 	size_t sz_arr_cmd_and_prm = 0;
	
// 	if (buf[sz_buf-1] != '\n')
// 		number_commands++;

// 	for (int i = 0; i < sz_buf; i++)
// 		if (buf[i] == '\n' | buf[i] == '\0' | buf[i] == ' ')
// 			number_commands++;

// 	return number_commands;
// }

// char *create_buf(FILE *fp_src, size_t *sz_file)
// {
// 	size_t start = ftell(fp_src);
// 	fseek(fp_src, 0L, SEEK_END);
// 	*sz_file = ftell(fp_src);
// 	fseek(fp_src, start, SEEK_SET);

// 	char *buf = (char*)calloc(*sz_file + 1, sizeof(char));
// 	fread(buf, sizeof(char), *sz_file, fp_src);
// 	buf[*sz_file] = '\0';

// 	fseek(fp_src, start, SEEK_SET);
// 	return buf;
// }

size_t calculate_size_array_cmd_and_parameters(FILE *fp_src)
{
	assert(fp_src != NULL);

	long start_file = ftell(fp_src);

	size_t sz_array_cmd_and_prm     = 0;

    char   name_command[max_size_c] = "VENOM";
	char   name_reg[max_size_c]     = "VENOM";
    int    num_command              = VENOM_NUM_COMMAND;
	bool   check_command            = false;

	while (!feof(fp_src))
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
		else
		{
			sz_array_cmd_and_prm++;
			check_command = false;
		}

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
