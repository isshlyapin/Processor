#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../library/stack.h"

const char *commands[] = {
    "push",   // 0
    "c_sqrt", // 1
    "c_sin",  // 2
    "c_cos",  // 3
    "in",     // 4
    "add",    // 5
    "sub",    // 6
    "mul",    // 7
    "c_div",  // 8
    "out",    // 9
    "hlt",    // 10
};

/*Нумерация команд
  push   1
  c_sqrt 2
  c_sin  3
  c_cos  4
  in     5
  add    6
  sub    7
  mul    8
  c_div  9
  out    10
  hlt    -1
  */

enum COMMANDS
{
    push = 1,
    c_sqrt,
    c_sin,
    c_cos,
    in,
    add,
    sub,
    mul,
    c_div,
    out,
    hlt = -1
};

const char *NAME_SRC_FILE = "res_asm.txt";

const size_t size_comm = 100;
const int COMPLETED = 0;

typedef double NUM_T;
#define NUM_MOD "%lf"

struct COMMANDS{
    short push   : 2;
    short pop    : 1;
    short c_sqrt : 1;
    short c_sin  : 1;
    short c_cos  : 1;
    short in     : 1;
    short add    : 1;
    short sub    : 1;
    short mul    : 1;
    short c_div  : 1;
    short out    : 1;
    short hlt    : 1;
};

int main()
{
    // printf("%lu\n", sizeof(short));

    Stack stk = {};
    StackCtor(&stk, 10);

    // char command_char[size_comm] = "venom";
    double num = 0;

    FILE *fp_src = fopen(NAME_SRC_FILE, "r");
    assert(fp_src != NULL);
    // size_t size_commands = sizeof(commands) / sizeof(commands[0]);

    while (!feof(fp_src))
    {
        assert(fscanf(fp_src, "%d", &command) == 1);

        if (command == -1)
            printf("[[%s]]\n", commands[10]);
        else
            printf("[[%s]]\n", commands[command - 1]);

        // printf("[[%s]]\n", command_char);
        // printf("%lu\n", sizeof(commands) / sizeof(commands[0]));
        printf("Number command:: %d\n", command);
        NUM_T num2 = VENOM_ELEM;
        NUM_T num1 = VENOM_ELEM;

        switch (command)
        {
        case push:
            fscanf(fp_src, NUM_MOD, &num);
            printf("Numeric from file: <%lf>\n", num); ////
            StackPush(&stk, num);
            printf("\n");
            break;

        case c_sqrt:
            fscanf(fp_src, NUM_MOD, &num);
            printf("Numeric from file: <%lf>\n", num); ////

            StackPush(&stk, sqrt(num));
            printf("\n");
            break;

        case c_sin:
            fscanf(fp_src, NUM_MOD, &num);
            printf("Numeric from file: <%lf>\n", num); ////

            StackPush(&stk, sin(num));
            printf("\n");
            break;

        case c_cos:
            fscanf(fp_src, NUM_MOD, &num);
            printf("Numeric from file: <%lf>\n", num); ////

            StackPush(&stk, cos(num));
            printf("\n");
            break;

        case in:
            printf("Введите число: \n");
            fscanf(stdin, NUM_MOD, &num);
            printf("Numeric user: <%lf>\n", num); ////

            StackPush(&stk, num);
            printf("\n");
            break;

        case add:
            // printf("Add:  \n");
            StackPop(&stk, &num1);
            StackPop(&stk, &num2);

            StackPush(&stk, num1 + num2);
            printf("\n");
            break;

        case sub:
            // printf("Sub:  \n");

            StackPop(&stk, &num1);
            StackPop(&stk, &num2);

            StackPush(&stk, num2 - num1);
            printf("\n");
            break;

        case mul:
            // printf("Mul:  \n");

            StackPop(&stk, &num1);
            StackPop(&stk, &num2);

            StackPush(&stk, num1 * num2);
            printf("\n");
            break;

        case c_div:
            // printf("Div:  \n");

            StackPop(&stk, &num1);
            StackPop(&stk, &num2);

            StackPush(&stk, num2 / num1);
            printf("\n");
            break;

        case out:
            // printf("Out:  \n");

            StackPop(&stk, &num1);

            printf("Ответ: [" NUM_MOD "]\n", num1);
            printf("\n");
            break;

        case hlt:
            fclose(fp_src);
            StackDtor(&stk);
            // printf("Hlt:  \n");

            return COMPLETED;

        default:
            break;
        }
    }
}
