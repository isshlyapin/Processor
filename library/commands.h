#ifndef COMMANDS_H
#define COMMANDS_H 

#include <stdio.h>

#define NEW_INSTRUCTIONS(name, num, ...) name = num,

enum commands{
    #include "instructions_def.h"
    #undef NEW_DIRECTIVE
    #undef NEW_INSTRUCTIONS
    #undef ENUM

    venom = -13
};

#define NEW_INSTRUCTIONS(name, ...) #name,

static const char *commands[] = {
    #include "instructions_def.h"
    #undef NEW_DIRECTIVE
    #undef NEW_INSTRUCTIONS
    
    "VENOM"
};

enum REGISTER{
    rax = 0,
    rbx = 1,
    rcx = 2,
    rdx = 3 
};

static const char* REGISTER[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx",
};

#endif
