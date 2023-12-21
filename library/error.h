#ifndef ERROR_H
#define ERROR_H

enum ERROR{
    ERROR_REG_NAME = -1,
    WITHOUT_ERROR,
    ERROR,
    ERROR_OPEN_FILE,
    ERROR_READ_CMD,
    ERROR_NAME_CMD,
    VENOM_ENUM,
    ERROR_REG_NUM,
    ERROR_ORG_DIR
};

static const char *ERROR_TEXT[] = {
    "WITHOUT ERROR --- GOOD JOB",
    "Error without name",
    "ERROR: File don't open",
    "ERROR: Don't read name command",
    "ERROR: Command not found",
    "ERROR: Incorrect register name",
    "ERROR: Incorrect register num",
    "ERROR: The org directive points to an occupied memory location"
};

#define CHECK_OPEN_FILE(file_ptr)                            \
    if (file_ptr == NULL)                                    \
    {                                                        \
        fprintf(stderr, "ERROR: Don't open source file");    \
        return ERROR;                                        \
    }

#define CHECK_ARGC(num, right_num)                                                   \
    if (num < right_num)                                                             \
    {                                                                                \
        fprintf(stderr, "Недостаточное количество аргументов командной строки\n");   \
        return ERROR;                                                                \
    }
    
#endif
