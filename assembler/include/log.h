#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <math.h>

static const int MAX_LEN_NUM = 100;

static const int LEN_LOG_STR = 74;
static const int INDENT_COL  = 6;

static const int TWO_COLUM   = 19;
static const int THREE_COLUM = 38;
static const int FOUR_COLUM  = 57;
static const int FIVE_COLUM  = 71;

enum LOG_FLAGS {
    PARTITION_FLAG = -2,
    TEMPLATES_FLAG = -1,
    TITLE_FLAG     = 0,
    FIR_COL_FLAG   = 1,
    SEC_COL_FLAG   = 2,
    THI_COL_FLAG   = 3,
    FOU_COL_FLAG   = 4
};

#ifdef LOG
    FILE *fp_log = NULL;

    #define OPEN_LOG_FIlE()              \
        fp_log = fopen(argv[3], "a");    \
        CHECK_OPEN_FILE(fp_log);

    #define CLOSE_LOG_FILE() fclose(fp_log) 
    #define PRINT_LOG(str ...) fprintf(fp_log, str)
#else
    #define OPEN_LOG_FIlE()  {}
    #define CLOSE_LOG_FILE() {}
    #define PRINT_LOG(str ...) {}
#endif //! LOG

#ifdef LOG
    #define CREATE_LOG_STR(first_col, tp1, second_col, tp2, third_col, tp3, fourth_col, tp4)     \
        create_log_str(log_str, TEMPLATES_FLAG, "VENOM");                                \
        create_log_str(log_str, FIR_COL_FLAG, first_col);                               \
        create_log_str(log_str, SEC_COL_FLAG, second_col);                              \
        create_log_str(log_str, THI_COL_FLAG, third_col);                               \
        create_log_str(log_str, FOU_COL_FLAG, fourth_col);                              \
        PRINT_LOG("%s", log_str);                                            \
        create_log_str(log_str, PARTITION_FLAG, "VENOM"); 
#else
    #define CREATE_LOG_STR(first_col, tp1, second_col, tp2, third_col, tp3, fourth_col, tp4) {};    
#endif

#define PRINT_PARTITION()                    \
    for (int i = 0; i < LEN_LOG_STR; i++)    \
    {                                        \
        if ((i == 0) || (i == FIVE_COLUM))   \
            str[i] = '|';                    \
        else if ( i == (LEN_LOG_STR - 2))    \
            str[i] = '\n';                   \
        else if (i == (LEN_LOG_STR - 1))     \
            str[i] = '\0';                   \
        else                                 \
            str[i] = '-';                    \
    }                                        \
    PRINT_LOG("%s", str);

#define PRINT_TITLE_DATE()                                      \
    for (int i = 0; i < LEN_LOG_STR; i++)                       \
    {                                                           \
        if ((i == 0) || (i == FIVE_COLUM))                      \
            str[i] = '|';                                       \
        else if ( i == (LEN_LOG_STR - 2))                       \
            str[i] = '\n';                                      \
        else if (i == (LEN_LOG_STR - 1))                        \
            str[i] = '\0';                                      \
        else                                                    \
            str[i] = ' ';                                       \
    }                                                           \
    int j = 0, k = 0;                                           \
    while (__DATE__[j] != '\0')                                 \
    {                                                           \
        str[((LEN_LOG_STR - 19) / 2) + j] = __DATE__[j];        \
        j++;                                                    \
    }                                                           \
    while (__TIME__[k] != '\0')                                 \
    {                                                           \
        str[((LEN_LOG_STR - 19) / 2) + j + 1] = __TIME__[k];    \
        j++;                                                    \
        k++;                                                    \
    }                                                           \
    PRINT_LOG("%s", str);

#define CREATE_SAMPLE()                                                                                    \
    for (int i = 0; i < LEN_LOG_STR; i++)                                                                  \
    {                                                                                                      \
        if ((i == 0) || (i == TWO_COLUM) || (i == THREE_COLUM) || (i == FOUR_COLUM) || (i == FIVE_COLUM))  \
            str[i] = '|';                                                                                  \
        else if (i == (LEN_LOG_STR - 2))                                                                   \
            str[i] = '\n';                                                                                 \
        else if (i == (LEN_LOG_STR - 1))                                                                   \
            str[i] = '\0';                                                                                 \
        else                                                                                               \
            str[i] = ' ';                                                                                  \
    }   

#define PRINT_TITLE()                               \
    for (int i = 0; "Name cmd"[i] != '\0'; i++)     \
        str[INDENT_COL + i] = "Name cmd"[i];                 \
    for (int i = 0; "Num cmd"[i] != '\0'; i++)      \
        str[TWO_COLUM + INDENT_COL + i] = "Num cmd"[i];      \
    for (int i = 0; "Parameter"[i] != '\0'; i++)    \
        str[THREE_COLUM + INDENT_COL + i] = "Parameter"[i];  \
    for (int i = 0; "CMD ID"[i] != '\0'; i++)       \
        str[FOUR_COLUM + INDENT_COL + i] = "CMD ID"[i];      \
    PRINT_LOG("%s", str);

char *itoa(void *num_ptr, char *str_res, char type);

void create_log_str(char *str, int flag, const char *info);

#endif //! LOG_H