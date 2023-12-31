#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

static const size_t STD_STACK_CAP = 10;
static const size_t MAX_SIZE_STR  = 100;
static const char   VENOM_NUM_CMD = -13;
static const int    LEN_NAME_REG  = 3;

typedef double num_t;

#define NUM_MOD_PRINT "%-9.2lf"
#define NUM_MOD_SCAN  "%lf"

#ifdef INFO
    #define PRINT_INFO(str ...) printf(str);
#else
    #define PRINT_INFO(str ...) {};
#endif //! INFO

#define PRINT_ERROR(str ...)                                                            \
    fprintf(stderr, str);                                                               \
    fprintf(stderr, "file: %s; func: %s; line: %d\n", __FILE__ , __func__ , __LINE__ ); \

#endif //! CONFIG_H
