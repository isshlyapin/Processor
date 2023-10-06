#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../library/stack.h"

const char *commands[] = {
    "push", // 0
    "sqrt", // 1
    "sin",  // 2
    "cos",  // 3
    "in",   // 4
    "add",  // 5
    "sub",  // 6
    "mul",  // 7
    "div",  // 8
    "out",  // 9
    "hlt",  // 10
};

enum COMMANDS {  push,  sqrt,  sin,  cos,  in,
                 add,   sub,   mul,  div,  out,
                 hlt};
const size_t size_comm = 100;

int main()
{
  char command_char[size_comm] = "venom";
  int command = -9;
  int     num =  0;
  
  FILE *fp_src = fopen("src.txt", "r");
  assert(fscanf(fp_src, "%s", command_char) == 1);

  size_t size_commands = sizeof(commands) / sizeof(commands[0]);
  for (size_t i = 0; i < size_commands; i++)
  {
    if (!strcmp(command_char, commands[i]))
      command = i;
  }
  switch (command)
  {
  case push:
    fscanf(fp_src, "%d", &num);
    break;
  case sqrt:
    
  
  default:
    break;
  }
    bool command_with_num = 0;
    bool command_in = 0;
    
    for (size_t i = 0; i < size_c_with_n; i++)
    {
      if (!strcmp(command, commands_with_num[i]))
      {
        if (!strcmp(command, commands_with_num[4]))
          command_in = 1;
        
        command_with_num = 1;
        break;
      }
    }

    if (command_with_num)
    {
      if (command_in)
      {
        printf("Please, input num\n");
        fscanf(stdin, "%d", &num);
      }
      else
        fscanf(fp_src, "%d", &num);
    }
  }
  printf("%d\n", num);
  // printf("%s\n", command);
  // printf("%s\n", commands[10]);
  // printf("%d", strcmp(command, commands[10]));

  if (!strcmp(command, commands[10]))
    printf("working\n");

  // while()
  fclose(fp_src);
}
