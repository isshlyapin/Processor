#include "../include/asm.h"
#include "../../include/commands.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define GREEN   "\033[1;32m"
#define WHITE   "\033[1;37m"
#define MAGENTA "\033[1;35m"

int main()
{
	printf("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);
    FILE *fp_src = fopen(NAME_SRC_ASM, "r");
    assert(fp_src != NULL);
	
    FILE *fp_res = fopen(NAME_RES_ASM, "w");
    assert(fp_res != NULL);
	
    write_file(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
}

int write_file(FILE *fp_src, FILE *fp_res)
{
    char  name_command[max_size_c]  = "VENOM";
    int   num_command               = VENOM_NUM_COMMAND;
    bool  check_hlt                 = false;
	NUM_T num                       = 0;

    while (!feof(fp_src))
    {
        assert(fscanf(fp_src, "%s", name_command) == 1);

        bool str = true;
        for (size_t i = 1; i <= NUMBER_INSTRUCTIONS; i++)
        {
            str = strcmp(name_command, commands[i]);
            if (!str)
            {
                if (i == NUMBER_INSTRUCTIONS) // last index always hlt
                {
                    num_command = hlt;
                    check_hlt = true;
                }
                else
                    num_command = i; 
                break;
            }
        }
        assert(!str);
        assert(num_command != VENOM_NUM_COMMAND);

        printf("Name_command: %s[%4s]%s", RED, name_command, RESET);
        printf("%s[%2d]%s\n", MAGENTA, num_command, RESET);

		if (num_command == push)
		{
			char name_reg[max_size_c] = "VENOM";
            
			if (fscanf(fp_src, NUM_MOD_SCAN, &num) != 1)
			{
				fprintf(fp_res, "%d", r_push);
				
				fscanf (fp_src, "%s" , name_reg);
				fprintf(fp_res, " %s", name_reg);
			}
			else
			{
				fprintf(fp_res, "%d", num_command);
				fscanf(fp_src, NUM_MOD_SCAN, &num);
				fprintf(fp_res, " " NUM_MOD_PRINT, num);
			}

            fprintf(fp_res, "\n");
			continue;
		}

		fprintf(fp_res, "%d", num_command);
        if (num_command == pop)
		{
			char name_reg[max_size_c] = "VENOM";

			fscanf (fp_src, "%s" , name_reg);
			fprintf(fp_res, " %s", name_reg);
		}
		if (num_command != -1)
			fprintf(fp_res, "\n");
	}

	if (!check_hlt)
		fprintf(fp_res, "%d", hlt);

	return 0;
}
