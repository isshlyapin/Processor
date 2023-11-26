#include "../include/processor.h"
#include "../include/config.h"

const char *NAME_SRC_PROC = "res_asm.txt";

int main()
{
	Storage str = {};
	StorageCtor(&str);

    FILE *fp_src = fopen(NAME_SRC_PROC, "r");
    if (fp_src == NULL)
	{
		if (call_asm() == 0)
			fp_src = fopen(NAME_SRC_PROC, "r");
		else 
			return ERROR;
	}

	process_commands(fp_src, &str);

	// STORAGE_DUMP(&str);
	StorageDtor(&str);
	fclose(fp_src);
}
