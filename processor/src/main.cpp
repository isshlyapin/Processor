#include "../include/processor.h"
#include "../../library/error.h"

int main(int argc, char *argv[])
{
    CHECK_ARGC(argc, 2);
    
    Storage str = {};
    StorageCtor(&str);

    FILE *fp_src = fopen(argv[1], "rb");
    CHECK_OPEN_FILE(fp_src);

    process_commands(fp_src, &str);

    StorageDtor(&str);
    fclose(fp_src);
}

