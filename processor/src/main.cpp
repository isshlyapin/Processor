#include "../include/processor.h"
#include "../../library/config.h"

const char *NAME_SRC_PROC = "res_asm.txt";

int main()
{
    Storage str = {};
    StorageCtor(&str);

    FILE *fp_src = fopen(NAME_SRC_PROC, "rb");

    process_commands(fp_src, &str);

    StorageDtor(&str);
    fclose(fp_src);
}
