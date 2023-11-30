#include <stdio.h>

enum COMMANDS
{
	r_push = 17,
    push = 1,
    c_sqrt,
    c_sin,
    c_cos,
    in,
    add,
    sub,
    mul,
    c_div,
    out,
	pop,
    hlt = -1
};

const char *commands[] = {
	"VENOM",   // 0
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
    "hlt",    // 12
};

const size_t NUMBER_INSTRUCTIONS = sizeof(commands) / sizeof(commands[0]) - 1;

/*Нумерация команд
  push   1
  c_sqrt 2
  c_sin  3
  c_cos  4
  in     5
  add    6
  sub    7
  mul    8
  c_div  9
  out    10
  pop    11
  hlt    -1
*/
