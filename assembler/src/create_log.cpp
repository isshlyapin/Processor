#include <stdio.h>
#include <malloc.h>

#include "../include/log.h"

static const char *NAME_COL1 = "Name cmd";
static const char *NAME_COL2 = "Num cmd";
static const char *NAME_COL3 = "Parameter";
static const char *NAME_COL4 = "CMD ID";

void print_log_partition(FILE *fp_log);
void print_log_title(FILE* fp_log);

void print_log_partition(FILE *fp_log)
{
    int len_log_str = (INDENT_COL + WIDTH_COL) * 4 + 5 + 2;

    char *log_str = (char *)calloc((size_t)len_log_str, sizeof(char));

    for (int i = 0; i < len_log_str; i++)
        log_str[i] = '-';

    log_str[0] = '|';
    log_str[len_log_str - 3] = '|' ;
    log_str[len_log_str - 2] = '\n';
    log_str[len_log_str - 1] = '\0';
    fprintf(fp_log, "%s", log_str);
    
    free(log_str);
}
    
void print_log_title(FILE* fp_log)
{
    int len_log_str = (INDENT_COL + WIDTH_COL) * 4 + 5 + 2;

    char *log_str = (char *)calloc((size_t)len_log_str, sizeof(char));

    fprintf(fp_log, "\n\n");
    print_log_partition(fp_log);
    
    // PRINT_TITLE_DATE
    for (int i = 0; i < len_log_str; i++)                           
    {                                                           
        if ((i == 0) || (i == (len_log_str - 3)))                   
            log_str[i] = '|';                                       
        else if ( i == (len_log_str - 2))                           
            log_str[i] = '\n';                                      
        else if (i == (len_log_str - 1))                            
            log_str[i] = '\0';                                 
        else                                                    
            log_str[i] = ' ';                                      
    }                                                           
    int j = 0, k = 0;                                           
    while (__DATE__[j] != '\0')                                
    {                                                           
        log_str[INDENT_DATE + j] = __DATE__[j];                     
        j++;
    }
    while (__TIME__[k] != '\0')
    {
        log_str[INDENT_DATE + j + 1 + k] = __TIME__[k];
        k++;
    }
    fprintf(fp_log, "%s", log_str);
    
    print_log_partition(fp_log);
    
    // PRINT_TITLE_COLUMNS
    fprintf(fp_log, "|%*s" "%*s" "|",   INDENT_COL, " ", -WIDTH_COL, NAME_COL1);
    fprintf(fp_log, "%*s"  "%*s" "|",   INDENT_COL, " ", -WIDTH_COL, NAME_COL2);
    fprintf(fp_log, "%*s"  "%*s" "|",   INDENT_COL, " ", -WIDTH_COL, NAME_COL3);
    fprintf(fp_log, "%*s"  "%*s" "|\n", INDENT_COL, " ", -WIDTH_COL, NAME_COL4);
    print_log_partition(fp_log);

    free(log_str);
}

// char *itoa(void *num_ptr, char *str_res, char type);

// char *itoa(void *num_ptr, char *str_res, char type)
// {
//     char num_str[MAX_LEN_NUM] = {};

//     if (type == 'd')
//     {
//         int num = *(int *)num_ptr;
//         if (num == 0)
//         {
//             str_res[0] = '0';
//             str_res[1] = '\0';
//             return str_res;
//         }

//         int id_res_str = 0;
//         if (num < 0)
//         {
//             num  = abs(num);
//             id_res_str = 1;
//             str_res[0] = '-';
//         }
        
//         for (int i = 0; i < MAX_LEN_NUM; i++)
//         {
//             if (num == 0)
//                 for ( ; i < MAX_LEN_NUM; i++)
//                     num_str[i] = '\0';
//             else
//             {
//                 num_str[i] = (char)(48 + num % 10);
//                 num /= 10;        
//             }
//         }

//         for (int i = MAX_LEN_NUM - 1; i >= 0; i--)
//         {
//             if (num_str[i] == '\0')
//                 continue;
//             else
//             {
//                 str_res[id_res_str] = num_str[i];
//                 id_res_str++;
//             }
//         }
//         str_res[id_res_str] = '\0';
//     }
//     else if (type == 'f')
//     {
//         int num = (int)(*(double *)num_ptr * 1000);

//         if (num == 0)
//         {
//             str_res[0] = '0';
//             str_res[1] = '\0';
//             return str_res;
//         }

//         int id_res_str = 0;
//         if (num < 0)
//         {
//             num  = abs(num);
//             str_res[0] = '-';
//             id_res_str = 1;
//         }
        
//         for (int i = 0; i < MAX_LEN_NUM; i++)
//         {
//             if (i < 3)
//             {
//                 num_str[i] = (char)(48 + num % 10);
//                 num /= 10;
//                 continue;
//             }
//             else if (i == 3)
//             {
//                 num_str[i] = '.';
//                 continue;                
//             }

            
//             if (num == 0)
//             {
//                 if (i == 4)
//                 {
//                     num_str[i] = '0';
//                     continue;
//                 }
//                 else
//                     for ( ; i < MAX_LEN_NUM; i++)
//                         num_str[i] = '\0';
//             }
//             else
//             {
//                 num_str[i] = (char)(48 + num % 10);
//                 num /= 10;        
//             }
//         }

//         for (int k = MAX_LEN_NUM; k >= 0; k--)
//         {
//             if (num_str[k] == '\0')
//                 continue;
//             else
//             {
//                 str_res[id_res_str] = num_str[k];
//                 id_res_str++;
//             }
//         } 
//         str_res[id_res_str] = '\0';
//     }

//     return str_res;
// }
