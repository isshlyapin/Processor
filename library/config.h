#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

static const size_t STD_STACK_CAP = 10;

typedef double num_t;

#define NUM_MOD_PRINT "%-9.2lf"
#define NUM_MOD_SCAN  "%lf"

#ifdef INFO
    #define PRINT_INFO(str ...) printf(str)
#else
    #define PRINT_INFO(str ...) {}
#endif

#endif
