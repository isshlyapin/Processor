#include "../include/processor.h"
#include "../library/stack.h"
#include "../include/commands.h"

static const size_t max_size_c        = 100;
static const NUM_T  VENOM_NUM_COMMAND = -13;

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define GREEN   "\033[1;32m"
#define WHITE   "\033[1;37m"
#define MAGENTA "\033[1;35m"

int call_asm()
{
	if (system("./asm") != 0)
	{
		if (system("g++ assembler/src/asm.cpp -o asm") == 0)
		{
			if (system("./asm") != 0)
			{
				fprintf(stderr, "ERROR: Assembler don`t work\n");
				return ERROR;
			}
			else
				return 0;
		}
		else	
		{
			fprintf(stderr, "ERROR: Assembler compile don`t work\n");
			return ERROR;
		}	
	}

	return 0;
}

int StorageCtor(Storage *str)
{
	StackCtor(&str->stk, std_stack_cap);
	str->ax = VENOM_ELEM;
	str->bx = VENOM_ELEM;
	str->cx = VENOM_ELEM;
	str->dx = VENOM_ELEM;

	return 0;
}

int StorageDtor(Storage *str)
{
	StackDtor(&str->stk);
	str->ax  = 0;
	str->bx  = 0;
	str->cx  = 0;
	str->dx  = 0;

	return 0;
}

int StorageDump(Storage *str, const char *file_err, const char *func_err, const int line_err)
{
	fprintf(stderr, "----------------------------------------------------------------------------\n");
    fprintf(stderr, "Storage[%p] \"str\" called from %s(%d) %s\n", str, file_err, line_err, func_err);
    fprintf(stderr, "----------------------------------------------------------------------------\n");
	fprintf(stderr, "Register [ax] = " ELEM_MOD "\n", str->ax);
	fprintf(stderr, "Register [bx] = " ELEM_MOD "\n", str->bx);
	fprintf(stderr, "Register [cx] = " ELEM_MOD "\n", str->cx);
	fprintf(stderr, "Register [dx] = " ELEM_MOD "\n", str->dx);
	STACK_DUMP(&str->stk);
	return 0;
}

int process_commands(FILE *fp_src, Storage *str)
{
	printf("\n___%sWORKING PROCESSOR%s___\n\n", GREEN, RESET);

	int command = VENOM_NUM_COMMAND;

	while (!feof(fp_src))
    {
        if (fscanf(fp_src, "%d", &command) != 1)
		{
			fprintf(stderr, "ERROR: Don`t num command");
			return ERROR;
		}

        if (command == -1)
		{
            printf("%s[%s]%s", GREEN, commands[NUMBER_INSTRUCTIONS], RESET); // last index always hlt
        	printf("%s[%d]%s\n", MAGENTA, command, RESET);
		}
		else if (command == 17)
		{
			printf("%s[%s]%s", GREEN, commands[push], RESET);
        	printf("%s[%d]%s\n", MAGENTA, command, RESET);
		}
		else
		{
            printf("%s[%s]%s", GREEN, commands[command], RESET);
        	printf("%s[%d]%s\n", MAGENTA, command, RESET);
		}
        
		NUM_T num2                = VENOM_ELEM;
        NUM_T num1                = VENOM_ELEM;
		char name_reg[max_size_c] = "VENOM";
		switch (command)
        {
        case push:
            fscanf(fp_src, NUM_MOD_SCAN, &num1);
            printf("Numeric from file: <" NUM_MOD_PRINT ">\n", num1); ////
            StackPush(&str->stk, num1);
            printf("\n");
            break;

		case r_push:
			fscanf(fp_src, "%s", name_reg);
            printf("Register from file: <%s>\n", name_reg); ////
            switch (name_reg[0])
			{
			case 'a':
				StackPush(&str->stk, str->ax);
				break;
			
			case 'b':
				StackPush(&str->stk, str->bx);
				break;

			case 'c':
				StackPush(&str->stk, str->cx);
				break;

			case 'd':
				StackPush(&str->stk, str->dx);
				break;
		
			default:
				break;
			}
            printf("\n");
            break;	

		case pop:
			fscanf(fp_src, "%s", name_reg);
            printf("Register from file: <%s>\n", name_reg); ////
            switch (name_reg[0])
			{
			case 'a':
				StackPop(&str->stk, &num1);
				str->ax = num1;
				break;
			
			case 'b':
				StackPop(&str->stk, &num1);
				str->bx = num1;
				break;

			case 'c':
				StackPop(&str->stk, &num1);
				str->cx = num1;
				break;

			case 'd':
				StackPop(&str->stk, &num1);
				str->dx = num1;
				break;
		
			default:
				break;
			}
            printf("\n");
            break;

        case c_sqrt:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, sqrt(num1));
            printf("\n");
            break;

        case c_sin:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, sin(num1));
            printf("\n");
            break;

        case c_cos:
            StackPop(&str->stk, &num1);
			StackPush(&str->stk, cos(num1));
            printf("\n");
            break;

        case in:
            printf("Введите число: \n");
            fscanf(stdin, NUM_MOD_SCAN, &num1);
            printf("Numeric user: <" NUM_MOD_PRINT ">\n", num1); ////

            StackPush(&str->stk, num1);
            printf("\n");
            break;

        case add:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num1 + num2);
            printf("\n");
            break;

        case sub:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num2 - num1);
            printf("\n");
            break;

        case mul:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num1 * num2);
            printf("\n");
            break;

        case c_div:
            StackPop(&str->stk, &num1);
            StackPop(&str->stk, &num2);

            StackPush(&str->stk, num2 / num1);
            printf("\n");
            break;

        case out:
            StackPop(&str->stk, &num1);
            printf("%sОтвет: [" NUM_MOD_PRINT "]%s\n", YELLOW, num1, RESET);
            printf("\n");
            break;

        case hlt:
            return 0;
        
		default:
            break;
        }
    }

	return 0;
}

