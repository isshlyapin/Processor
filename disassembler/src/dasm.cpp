#include "../include/dasm.h"
#include "../../include/commands.h"

int main()
{
    FILE *fp_src = fopen(NAME_SRC_DASM, "r");
    assert(fp_src != NULL);

    FILE *fp_res = fopen(NAME_RES_DASM, "w");
    assert(fp_res != NULL);
    
    write_file(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
}

int write_file(FILE *fp_src, FILE *fp_res)
{
    int   num_command          = VENOM_NUM_COMMAND;
	char  name_reg[max_size_c] = "VENOM";
    bool  check_hlt            = false;
    NUM_T num                  = 0;

    while (!feof(fp_src))
    {
        if (fscanf(fp_src, "%d", &num_command) != 1)
		{
			fprintf(stderr, "ERROR: Don`t num command\n");
			return ERROR;
		}

        if (num_command == hlt)
        {
			fprintf(fp_res, "%s", commands[NUMBER_INSTRUCTIONS]);  // index hlt always = NUMBER_INSTRUCTIONS
            check_hlt = true;
        }
        else if (num_command == push)
		{
            fprintf(fp_res, "%s "             , commands[push]);
            fscanf (fp_src, NUM_MOD_SCAN      , &num);
            fprintf(fp_res, NUM_MOD_PRINT "\n", num);    
        }
		else if (num_command == r_push)
		{
            fprintf(fp_res, "%s " , commands[push]);
			fscanf (fp_src, "%s"  , name_reg);
            fprintf(fp_res, "%s\n", name_reg);    
		}
		else if (num_command == pop)
		{
            fprintf(fp_res, "%s " , commands[pop]);
			fscanf (fp_src, "%s"  , name_reg);
            fprintf(fp_res, "%s\n", name_reg);    
		}
		else
            fprintf(fp_res, "%s\n", commands[num_command]);
    }

    if (!check_hlt)
        fprintf(fp_res, "%s\0", commands[NUMBER_INSTRUCTIONS]);   // index hlt always = NUMBER_INSTRUCTIONS

	return 0;
}
