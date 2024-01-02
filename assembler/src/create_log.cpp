#include "../include/log.h"

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
    if (flag == TITLE_FLAG)
    {
        PRINT_LOG("\n\n");
        PRINT_PARTITION();
        PRINT_TITLE_DATE();
        PRINT_PARTITION();
        CREATE_SAMPLE();
        PRINT_TITLE();
        PRINT_PARTITION();
    }
    else if (flag == PARTITION_FLAG)
    {
        PRINT_PARTITION();
    }
    else if (flag == TEMPLATES_FLAG)
    {
        CREATE_SAMPLE();
    }
    else if (flag == FIR_COL_FLAG)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[INDENT_COL + i] = info[i];
    }
    else if (flag == SEC_COL_FLAG)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[TWO_COLUM + INDENT_COL + i] = info[i];
    }
    else if (flag == THI_COL_FLAG)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[THREE_COLUM + INDENT_COL + i] = info[i];
    }
    else if (flag == FOU_COL_FLAG)
    {
        for (int i = 0; info[i] != '\0'; i++)
            str[FOUR_COLUM + INDENT_COL + i] = info[i];
    }
}
