#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/config.h"


const size_t max_size_c = 100;
const int COMPLETED = 0;

typedef int NUM_T;
#define ELEM_MOD "%d"

enum name_commandS
{
	r_push = 17,
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
	pop,
    hlt = -1
};

const char *name_commands[] = {
	"VENOM",   // 0
    "push",   // 1
    "c_sqrt", // 2
    "c_sin",  // 3
    "c_cos",  // 4
    "in",     // 5
    "add",    // 6
    "sub",    // 7
    "mul",    // 8
    "c_div",  // 9
    "out",    // 10
	"pop",    // 11   
    "hlt",    // 12
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
  pop    11
  hlt    -1
*/

void write_file(FILE *fp_src, FILE *fp_res);

int main()
{

    FILE *fp_src = fopen(NAME_SRC, "r");
    assert(fp_src != NULL);

    FILE *fp_res = fopen(NAME_RES, "w");
    assert(fp_res != NULL);
    
    write_file(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
}


void write_file(FILE *fp_src, FILE *fp_res)
{
    char name_command[max_size_c] = "VENOM";
    int num_command = -13;
    NUM_T num = 0;
    bool check_hlt = false;

    while (!feof(fp_src))
    {
        assert(fscanf(fp_src, "%s", name_command) == 1);

        printf("name_command: [%s]\n", name_command);
        
        bool str = 1;
        for (size_t i = 1; i <= NUMBER_INSTRUCTIONS; i++)
        {
            str = strcmp(name_command, name_commands[i]);
            if (!str)
            {
                if (i == NUMBER_INSTRUCTIONS)
                {
                    num_command = -1;
                    check_hlt = true;
                }
                else
                    num_command = i;
                break;
            }
        }
        assert(!str);
        assert(num_command != -9);

        printf("Numeric name_command: %d\n", num_command);

		if (num_command == push)
		{
			char name_reg[max_size_c] = "VENOM";
            
			if (fscanf(fp_src, ELEM_MOD, &num) != 1)
			{
				fprintf(fp_res, "%d", r_push);
				
				fscanf (fp_src, "%s" , name_reg);
				fprintf(fp_res, " %s", name_reg);
			}
			else
			{
				fprintf(fp_res, "%d", num_command);
				fscanf(fp_src, ELEM_MOD, &num);
				fprintf(fp_res, " " ELEM_MOD, num);
			}

            fprintf(fp_res, "\n");
			continue;
		}
		// fprintf(fp_res, "%d\n", 153);

		fprintf(fp_res, "%d", num_command);
        if (num_command == pop)
		{
			char name_reg[max_size_c] = "VENOM";

			fscanf (fp_src, "%s" , name_reg);
			fprintf(fp_res, " %s", name_reg);
		}
		fprintf(fp_res, "\n");
    	
	}
}
