#include <stdio.h>
#include <assert.h>
#include <string.h>

const size_t max_size_c = 100;
const int COMPLETED = 0;

typedef double NUM_T;
#define NUM_MOD "%lf"

enum name_commandS
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

const char *name_commands[] = {
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
const char *NAME_SRC = "src.txt";
const char *NAME_RES = "res_asm.txt";

void write_file(FILE *fp_src, FILE *fp_res);

int main()
{

    FILE *fp_src = fopen(NAME_SRC, "r");
    assert(fp_src != NULL);

    FILE *fp_res = fopen(NAME_RES, "a");
    assert(fp_res != NULL);
    
    write_file(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
}


void write_file(FILE *fp_src, FILE *fp_res)
{
    char name_command[max_size_c] = "VENOM";
    int num_command = -9;
    NUM_T num = 0;
    bool check_hlt = false;

    while (!feof(fp_src))
    {
        assert(fscanf(fp_src, "%s", name_command) == 1);

        printf("name_command: [%s]\n", name_command);
        
        bool str = 1;
        for (size_t i = 0; i < sizeof(name_commands) / sizeof(name_commands[0]); i++)
        {
            str = strcmp(name_command, name_commands[i]);
            if (!str)
            {
                if (i == 10)
                {
                    num_command = -1;
                    check_hlt = true;
                }
                else
                    num_command = i + 1;
                break;
            }
        }
        assert(!str);
        assert(num_command != -9);

        printf("Numeric name_command: %d\n", num_command);

        fprintf(fp_res, "%d", num_command);

        switch (num_command)
        {
        case push:  case c_sqrt:  case c_sin:  case c_cos:
            fscanf(fp_src, NUM_MOD, &num);
            printf("Numeric from file: <%lf>\n", num);
            fprintf(fp_res, " " NUM_MOD, num); ////
            fprintf(fp_res, "\n");
            break;

        case in:  case add:  case sub:  case mul:  case c_div:  case out:  case hlt:
            fprintf(fp_res, "\n");
            break;


        default:
            printf("Unknown command\n");
            assert(num_command <= 10 && num_command >= -1 && num_command != 0);
            break;
        }
    }
    if (!check_hlt)
        fprintf(fp_res, "%d\0", -1);
}
