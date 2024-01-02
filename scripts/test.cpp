#include <stdio.h>
#include <math.h>

const int MAX_LEN_NUM = 100;
const int LEN_LOG_STR = 74;
const int TWO_COLUM   = 19;
const int THREE_COLUM = 38;
const int FOUR_COLUM  = 57;
const int FIVE_COLUM  = 71;

#define CREATE_LOG_STR(first_col, second_col, third_col, fourth_col)     \
    create_log_str(log_str, -1, "VENOM");                                \
    create_log_str(log_str, 1, first_col);                               \
    create_log_str(log_str, 2, second_col);                              \
    create_log_str(log_str, 3, third_col);                               \
    create_log_str(log_str, 4, fourth_col);                              \
    PRINT_LOG("%s", log_str);                                            \
    create_log_str(log_str, -2, "VENOM"); 

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
        str[5 + i] = "Name cmd"[i];                 \
    for (int i = 0; "Num cmd"[i] != '\0'; i++)      \
        str[TWO_COLUM + 5 + i] = "Num cmd"[i];      \
    for (int i = 0; "Parameter"[i] != '\0'; i++)    \
        str[THREE_COLUM + 5 + i] = "Parameter"[i];  \
    for (int i = 0; "CMD ID"[i] != '\0'; i++)       \
        str[FOUR_COLUM + 5 + i] = "CMD ID"[i];      \
    PRINT_LOG("%s", str);

char *itoa(void *num_ptr, char *str_res, char type);

void create_log_str(char *str, int flag, const char *info);

char *itoa(void *num_ptr, char *str_res, char type)
{
    char num_str[MAX_LEN_NUM] = {};

    if (type == 'd')
    {
        int num = *(int *)num_ptr;
        if (num == 0)
        {
            str_res[0] = '0';
            str_res[1] = '\0';
            return str_res;
        }

        int id_res_str = 0;
        if (num < 0)
        {
            num  = abs(num);
            id_res_str = 1;
            str_res[0] = '-';
        }
        
        for (int i = 0; i < MAX_LEN_NUM; i++)
        {
            if (num == 0)
                for ( ; i < MAX_LEN_NUM; i++)
                    num_str[i] = '\0';
            else
            {
                num_str[i] = (char)(48 + num % 10);
                num /= 10;        
            }
        }

        for (int i = MAX_LEN_NUM - 1; i >= 0; i--)
        {
            if (num_str[i] == '\0')
                continue;
            else
            {
                str_res[id_res_str] = num_str[i];
                id_res_str++;
            }
        }
        str_res[id_res_str] = '\0';
    }
    else if (type == 'f')
    {
        int num = (int)(*(double *)num_ptr * 1000);

        if (num == 0)
        {
            str_res[0] = '0';
            str_res[1] = '\0';
            return str_res;
        }

        int id_res_str = 0;
        if (num < 0)
        {
            num  = abs(num);
            str_res[0] = '-';
            id_res_str = 1;
        }
        
        for (int i = 0; i < MAX_LEN_NUM; i++)
        {
            if (i < 3)
            {
                num_str[i] = (char)(48 + num % 10);
                num /= 10;
                continue;
            }
            else if (i == 3)
            {
                num_str[i] = '.';
                continue;                
            }

            
            if (num == 0)
            {
                if (i == 4)
                {
                    num_str[i] = '0';
                    continue;
                }
                else
                    for ( ; i < MAX_LEN_NUM; i++)
                        num_str[i] = '\0';
            }
            else
            {
                num_str[i] = (char)(48 + num % 10);
                num /= 10;        
            }
        }

        for (int k = MAX_LEN_NUM; k >= 0; k--)
        {
            if (num_str[k] == '\0')
                continue;
            else
            {
                str_res[id_res_str] = num_str[k];
                id_res_str++;
            }
        } 
        str_res[id_res_str] = '\0';
    }

    return str_res;
}

void create_log_str(char *str, int flag, const char *info)
{
    if (flag == -1)
    {
        CREATE_SAMPLE();
    }
    else if (flag == -2)
    {
        PRINT_PARTITION();
    }
    else if (flag == 0)
    {
        PRINT_LOG("\n\n");
        PRINT_PARTITION();
        PRINT_TITLE_DATE();
        PRINT_PARTITION();
        CREATE_SAMPLE();
        PRINT_TITLE();
        PRINT_PARTITION();
    }
    else if (flag == 1)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[5 + i] = info[i];
    }
    else if (flag == 2)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[TWO_COLUM + 5 + i] = info[i];
    }
    else if (flag == 3)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[THREE_COLUM + 5 + i] = info[i];
    }
    else if (flag == 4)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[FOUR_COLUM + 5 + i] = info[i];
    }
}
