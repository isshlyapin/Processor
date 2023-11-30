#include "../include/processor.h"
#include "../../library/stack.h"
#include "../../library/commands.h"
#include "../../library/color.h"
static const size_t max_size_c        = 100;
static const num_t  VENOM_NUM_COMMAND = -13;

#define INFO

enum REGISTER{
	rax = 1,
	rbx,
	rcx,
	rdx
};

const char* REGISTER[] = {
	"VENOM",
	"rax",
	"rbx",
	"rcx",
	"rdx",
};

int call_asm()
{
	return system("./run_asm.sh");
}
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

	num_t command = VENOM_NUM_COMMAND;

	size_t sz_src_file = 0;
	char *buf = create_buf(fp_src, &sz_src_file);

	size_t number_commands = sz_src_file / sizeof(num_t);


	for (size_t pc = 0; pc < number_commands;)
    {
		command = *((num_t*)buf + pc);

		#ifdef INFO
        if ((int)command == hlt)
		{
            printf("%s[%s]%s", GREEN, commands[NUMBER_INSTRUCTIONS], RESET); // last index always hlt
        	printf("%s[%d]%s\n", MAGENTA, (int)command, RESET);
		}
		else if ((int)command == r_push)
		{
			printf("%s[%s]%s", GREEN, commands[push], RESET);
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

		switch ((int)command)
        {
        case push:
			num1 = *((num_t*)buf + pc + 1);
            printf("Numeric from file: <" NUM_MOD_PRINT ">\n", num1); ////
            StackPush(&str->stk, num1);
            printf("\n");
			pc += 2;
            break;

		case r_push:
			num1 = *((num_t*)buf + pc + 1);
            printf("Register from file: <%s>\n", REGISTER[(int)num1]); ////
            StackPush(&str->stk, *ptr_reg(str, num1));
            printf("\n");
			pc += 2;
            break;	

		case pop:
			num1 = *((num_t*)buf + pc + 1);
            printf("Register from file: <%s>\n", REGISTER[(int)num1]); ////
            StackPop(&str->stk, ptr_reg(str, num1));
            printf("\n");
			pc += 2;
            break;

        case c_sqrt:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, sqrt(num1));
            printf("\n");
			pc++;
            break;

        case c_sin:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, sin(num1));
            printf("\n");
			pc++;
            break;

        case c_cos:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, cos(num1));
            printf("\n");
            pc++;
			break;

        case in:
            printf("Введите число: ");
            fscanf(stdin, NUM_MOD_SCAN, &num1);
            printf("Numeric user: <" NUM_MOD_PRINT ">\n", num1); ////

            StackPush(&str->stk, num1);
            printf("\n");
            pc++;
			break;

        case add:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num1 + num2);
            printf("\n");
            pc++;
			break;

        case sub:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num2 - num1);
            printf("\n");
            pc++;
			break;

        case mul:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num1 * num2);
            printf("\n");
            pc++;
			break;

        case c_div:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num2 / num1);
            printf("\n");
            pc++;
			break;

        case out:
            StackPop(&str->stk, &num1);
            printf("%sОтвет: [" NUM_MOD_PRINT "]%s\n", YELLOW, num1, RESET);
            printf("\n");
			pc++;
            break;

        case hlt:
			pc++;
			free(buf);
            return 0;
			break;
        
		default:
            break;
        }
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