#include "../include/log.h"

static const char *NAME_COLS[] = {
    "Name cmd",
    "RAM ID",
    "Num cmd",
    "Parameter",
    "CMD ID"
};

void print_log_partition(FILE *fp_log)
{
    // Ширина столбцов и кол-во разделительных линий
    // +2: добавление в конце '\n' и '\0'
    int len_log_str = (INDENT_COL + WIDTH_COL) * AMOUNT_COL + (AMOUNT_COL + 1) + 2; 

    char *log_str = (char *)calloc((size_t)len_log_str, sizeof(char));

    for (int i = 0; i < len_log_str; i++)
        log_str[i] = '-';

    log_str[0]               = '|';
    log_str[len_log_str - 3] = '|' ;
    log_str[len_log_str - 2] = '\n';
    log_str[len_log_str - 1] = '\0';
    fprintf(fp_log, "%s", log_str);
    
    free(log_str);
}
    
void print_log_title(FILE* fp_log)
{
    assert(fp_log != NULL);

    fprintf(fp_log, "\n\n");
    print_log_partition(fp_log);
    
    // Ширина столбцов и кол-во разделительных линий
    // +2: добавление в конце '\n' и '\0'
    int len_log_str = (INDENT_COL + WIDTH_COL) * AMOUNT_COL + (AMOUNT_COL + 1) + 2; 

    char *log_str = (char *)calloc((size_t)len_log_str, sizeof(char));

    // Создание шаблона под строку с датой
    for (int i = 0; i < len_log_str; i++)                           
        log_str[i] = ' ';                                      

    log_str[0]               = '|';
    log_str[len_log_str - 3] = '|' ;
    log_str[len_log_str - 2] = '\n';
    log_str[len_log_str - 1] = '\0';

    // Добавление в строку даты и печать
    int j = 0, k = 0;                                           
    while (__DATE__[j] != '\0')                                
    {                                                           
        log_str[INDENT_DATE + j] = __DATE__[j];                     
        j++;
    }
    while (__TIME__[k] != '\0')
    {
        log_str[INDENT_DATE + j + k + 1] = __TIME__[k];
        k++;
    }
    fprintf(fp_log, "%s", log_str);
    
    print_log_partition(fp_log);
    
    // Печать заголовков столбцов
    for (int i = 0; i < AMOUNT_COL; i++)
        fprintf(fp_log, "|%*s" "%*s", INDENT_COL, " ", -WIDTH_COL, NAME_COLS[i]);
    fprintf(fp_log, "|\n");
    
    print_log_partition(fp_log);
    free(log_str);
}
