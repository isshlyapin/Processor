#include <stdio.h>

enum COMMANDS
{
    cmd_hlt = -1,
    cmd_push = 1,
    cmd_sqrt,
    cmd_sin,
    cmd_cos,
    cmd_in,
    cmd_add,
    cmd_sub,
    cmd_mul,
    cmd_div,
    cmd_out,
    cmd_pop,
    cmd_jmp,
    cmd_jb,
    cmd_jbe,
    cmd_ja,
    cmd_jae,
    cmd_je,
    cmd_jne,
    cmd_rpush,
};

const char *commands[] = {
    "VENOM",  // 0
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
    "hlt",    // 19 
};  

const size_t NUMBER_INSTRUCTIONS = sizeof(commands) / sizeof(commands[0]) - 1;

enum REGISTER{
    rax = 1,
    rbx,
    rcx,
    rdx 
};


const char* REGISTER[] = {
    "VENOM",
    "rax",
    "rbx",
    "rcx",
    "rdx",
};
