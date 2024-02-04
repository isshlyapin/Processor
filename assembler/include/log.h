#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

static const int INDENT_COL  = 7;
static const int INDENT_DATE = 40;
static const int WIDTH_COL   = 13;
static const int AMOUNT_COL  = 5;

void print_log_partition(FILE *fp_log);

void print_log_title(FILE* fp_log);

#endif // LOG_H
