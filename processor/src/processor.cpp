#include "../include/processor.h"
#include "../../library/stack.h"
#include "../../library/commands.h"
#include "../../library/color.h"

static const size_t max_size_c        = 100;
static const num_t  VENOM_NUM_COMMAND = -13;
static const double EPSILON = 1e-9;

int StorageCtor(Storage *str)
{
	StackCtor(&str->stk, std_stack_cap);
	str->rax = VENOM_ELEM;
	str->rbx = VENOM_ELEM;
	str->rcx = VENOM_ELEM;
	str->rdx = VENOM_ELEM;

	return 0;
}

int StorageDtor(Storage *str)
{
	StackDtor(&str->stk);
	str->rax  = 0;
	str->rbx  = 0;
	str->rcx  = 0;
	str->rdx  = 0;

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
	STACK_DUMP(&str->stk);
	return 0;
}

char *create_buf(FILE *fp_src, size_t *sz_file)
{
	long start = ftell(fp_src);
	fseek(fp_src, 0L, SEEK_END);
	*sz_file = (size_t)ftell(fp_src);
	fseek(fp_src, start, SEEK_SET);

	char *buf = (char*)calloc(*sz_file + 1, sizeof(char));
	fread(buf, sizeof(char), *sz_file, fp_src);
	buf[*sz_file] = '\0';

	fseek(fp_src, start, SEEK_SET);
	return buf;
}

int process_commands(FILE *fp_src, Storage *str)
{
	printf("\n___%sWORKING PROCESSOR%s___\n\n", GREEN, RESET);

	int command = VENOM_NUM_COMMAND;

	size_t sz_src_file = 0;
	char *buf = create_buf(fp_src, &sz_src_file);

	size_t number_commands = sz_src_file / sizeof(num_t);


	for (size_t pc = 0; pc < number_commands;)
    {
		command = (int)*((num_t*)buf + pc);

		#ifdef INFO
        if (command == cmd_hlt)
		{
            printf("%s[%s]%s", GREEN, commands[NUMBER_INSTRUCTIONS], RESET); // last index always hlt
        	printf("%s[%d]%s\n", MAGENTA, (int)command, RESET);
		}
		else if (command == cmd_rpush)
		{
			printf("%s[%s]%s", GREEN, commands[cmd_push], RESET);
        	printf("%s[%d]%s\n", MAGENTA, (int)command, RESET);
		}
		else
		{
            printf("%s[%s]%s", GREEN, commands[(int)command], RESET);
        	printf("%s[%d]%s\n", MAGENTA, (int)command, RESET);
		}
        #endif

		num_t num2 = VENOM_ELEM;
        num_t num1 = VENOM_ELEM;

		switch (command)
        {
			case cmd_push:
				num1 = *((num_t*)buf + pc + 1);
				StackPush(&str->stk, num1);
				pc += 2;

				#ifdef INFO
				printf("Numeric from file: <" NUM_MOD_PRINT ">\n", num1);
				#endif

				break;

			case cmd_rpush:
				num1 = *((num_t*)buf + pc + 1);
				StackPush(&str->stk, *ptr_reg(str, num1));
				pc += 2;

				#ifdef INFO
				printf("Register from file: <%s>\n", REGISTER[(int)num1]);
				#endif

				break;	

			case cmd_pop:
				num1 = *((num_t*)buf + pc + 1);
				StackPop(&str->stk, ptr_reg(str, num1));
				pc += 2;
				
				#ifdef INFO
				printf("Register from file: <%s>\n", REGISTER[(int)num1]); ////
				#endif
				
				break;

			case cmd_sqrt:
				StackPop(&str->stk, &num1);
				StackPush(&str->stk, sqrt(num1));
				pc++;
				printf("\n");
				break;

			case cmd_sin:
				StackPop(&str->stk, &num1);
				StackPush(&str->stk, sin(num1));
				printf("\n");
				pc++;
				break;

			case cmd_cos:
				StackPop(&str->stk, &num1);
				StackPush(&str->stk, cos(num1));
				pc++;
				break;

			case cmd_in:
				printf("Введите число: ");
				fscanf(stdin, NUM_MOD_SCAN, &num1);

				StackPush(&str->stk, num1);
				pc++;
				
				#ifdef INFO
				printf("Numeric user: <" NUM_MOD_PRINT ">\n", num1); ////
				#endif
				
				break;

			case cmd_add:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);

				StackPush(&str->stk, num1 + num2);
				pc++;
				break;

			case cmd_sub:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);

				StackPush(&str->stk, num2 - num1);
				pc++;
				break;

			case cmd_mul:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);

				StackPush(&str->stk, num1 * num2);
				pc++;
				break;

			case cmd_div:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);

				StackPush(&str->stk, num2 / num1);
				pc++;
				break;

			case cmd_jmp:
				pc = (size_t)*((num_t*)buf + pc + 1);
				break;
			
			case cmd_jb:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (num2 < num1)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;
			
			case cmd_jbe:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (num2 <= num1)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;
			
			case cmd_ja:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (num2 > num1)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;

			case cmd_jae:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (num2 >= num1)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;
			
			case cmd_je:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (llabs(num2 - num1) < EPSILON)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;

			case cmd_jne:
				StackPop(&str->stk, &num1);
				StackPop(&str->stk, &num2);
				if (llabs(num2 - num1) > EPSILON)
					pc = (size_t)*((num_t*)buf + pc + 1);
				else
					pc += 2;
				break;
			
			case cmd_out:
				StackPop(&str->stk, &num1);
				printf("%sОтвет: [" NUM_MOD_PRINT "]%s\n", YELLOW, num1, RESET);
				pc++;
				break;

			case cmd_hlt:
				pc++;
				free(buf);
				return 0;
				break;
			
			default:
				break;
        }
		printf("\n");
    }

	free(buf);
	return 0;
}

num_t *ptr_reg(Storage *str, num_t id_reg)
{
	switch ((int)id_reg)
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