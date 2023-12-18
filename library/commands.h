#ifndef COMMANDS_H
#define COMMANDS_H 

#include <stdio.h>

enum COMMANDS
{
    cmd_hlt   = 0,
    cmd_push  = 1,
    cmd_sqrt  = 2,
    cmd_sin   = 3,
    cmd_cos   = 4,
    cmd_in    = 5,
    cmd_add   = 6,
    cmd_sub   = 7,
    cmd_mul   = 8,
    cmd_div   = 9,
    cmd_out   = 10,
    cmd_pop   = 11,
    cmd_jmp   = 12,
    cmd_jb    = 13,
    cmd_jbe   = 14,
    cmd_ja    = 15,
    cmd_jae   = 16,
    cmd_je    = 17,
    cmd_jne   = 18,
    cmd_call  = 19,
    cmd_ret   = 20,
    dir_org   = 21,
    cmd_rpush = 22
};

static const char *commands[] = {
    "hlt",    // 0
    "push",   // 1
    "c_sqrt", // 2
    "c_sin",  // 3
    "c_cos",  // 4
    "in",     // 5
    "add",    // 6
    "sub",    // 7
    "mul",    // 8
    "c_div",  // 9
    "out",    // 10
    "pop",    // 11
    "jmp",    // 12
    "jb",     // 13
    "jbe",    // 14
    "ja",     // 15
    "jae",    // 16
    "je",     // 17
    "jne",    // 18
    "call",   // 19
    "ret",    // 20
    "org"     // 21
};  

static const size_t NUMBER_INSTRUCTIONS = sizeof(commands) / sizeof(commands[0]);

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
