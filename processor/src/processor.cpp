#include "../include/processor.h"
#include "../../library/color.h"
#include "../../library/commands.h"
#include "../../library/error.h"
#include "../../library/stack.h"

static const size_t MAX_SIZE_STR = 100;
static const char VENOM_NUM_CMD = -13;

static const double EPSILON = 1e-9;

int process_commands(FILE *fp_src, Storage *str)
{



    
            PRINT_INFO("\n___%sWORKING PROCESSOR%s___\n\n", GREEN, RESET);

  size_t sz_src_file = 0;
  char *src_arr = create_src_arr(fp_src, &sz_src_file);

  for (size_t pc = 0; pc < sz_src_file;) {
    unsigned char num_cmd = src_arr[pc];

    PRINTF_INFO_CMD();

    num_t num1 = VENOM_ELEM;
    num_t num2 = VENOM_ELEM;
    int id_jmp = 0;

    switch (num_cmd) {
    case cmd_push:
      memcpy(&num1, src_arr + pc + 1, sizeof(num_t));
      PUSH(num1);
      pc += 1 + sizeof(num_t);

      PRINT_INFO("Numeric from file: <%.2lf>\n", num1);
      break;

    case 53:
      num_cmd = src_arr[pc + 1];
      PUSH(*PTR_REG(num_cmd));
      pc += 2;

      PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)num_cmd]);
      break;

    case cmd_pop:
      num_cmd = src_arr[pc + 1];
      POP(PTR_REG(num_cmd));
      pc += 2;

      PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)num_cmd]);
      break;

    case cmd_sqrt:
      POP(&num1);
      PUSH(sqrt(num1));
      pc++;
      break;

    case cmd_sin:
      POP(&num1);
      PUSH(sin(num1));
      pc++;
      break;

    case cmd_cos:
      POP(&num1);
      PUSH(cos(num1));
      pc++;
      break;

    case cmd_in:
      printf("Введите число: ");
      fscanf(stdin, NUM_MOD_SCAN, &num1);
      PUSH(num1);
      pc++;

      PRINT_INFO("Numeric user: <%.2lf>\n", num1);
      break;

    case cmd_add:
      TWO_POP(num1, num2);
      PUSH(num2 + num1);
      pc++;
      break;

    case cmd_sub:
      TWO_POP(num1, num2);
      PUSH(num2 - num1);
      pc++;
      break;

    case cmd_mul:
      TWO_POP(num1, num2);
      PUSH(num2 * num1);
      pc++;
      break;

    case cmd_div:
      TWO_POP(num1, num2);
      PUSH(num2 / num1);
      pc++;
      break;

    case cmd_jmp:
      memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));
      pc = (size_t)id_jmp;
      break;

    case cmd_jb:
      JMP_IF(num2 < num1, num1, num2, src_arr);
      break;

    case cmd_jbe:
      JMP_IF(num2 <= num1, num1, num2, src_arr);
      break;

    case cmd_ja:
      JMP_IF(num2 > num1, num1, num2, src_arr);
      break;

    case cmd_jae:
      JMP_IF(num2 >= num1, num1, num2, src_arr);
      break;

    case cmd_je:
      JMP_IF(fabs(num2 - num1) < EPSILON, num1, num2, src_arr);
      break;

    case cmd_jne:
      JMP_IF(fabs(num2 - num1) > EPSILON, num1, num2, src_arr);
      break;

    case cmd_call:
      memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));
      PUSH_PTR(pc + 1 + sizeof(int));
      pc = (size_t)id_jmp;
      break;

    case cmd_ret:
      POP_PTR(&id_ret);
      pc = (size_t)id_ret;
      break;

    case cmd_out:
      POP(&num1);
      printf("%sОтвет: [%.2lf]%s\n", YELLOW, num1, RESET);
      pc++;
      break;

    case cmd_hlt:
      pc++;
      free(src_arr);
      return WITHOUT_ERROR;

    default:
      free(src_arr);
      return ERROR;
    }
    PRINT_INFO("\n");
  }

  return WITHOUT_ERROR;
}

int StorageCtor(Storage *str) {
  StackCtor(&str->stk_cmd, STD_STACK_CAP);
  StackCtor(&str->stk_ptr, STD_STACK_CAP);

  str->regs[rax] = VENOM_ELEM;
  str->regs[rbx] = VENOM_ELEM;
  str->regs[rcx] = VENOM_ELEM;
  str->regs[rdx] = VENOM_ELEM;

  return WITHOUT_ERROR;
}

int StorageDtor(Storage *str) {
  StackDtor(&str->stk_cmd);
  StackDtor(&str->stk_ptr);
  str->regs[rax] = VENOM_ELEM;
  str->regs[rbx] = VENOM_ELEM;
  str->regs[rcx] = VENOM_ELEM;
  str->regs[rdx] = VENOM_ELEM;

  return WITHOUT_ERROR;
}

int StorageDump(Storage *str, const char *file_err, const char *func_err,
                const int line_err) {
  fprintf(stderr, "------------------------------------------------------------"
                  "----------------\n");
  fprintf(stderr, "Storage[%p] \"str\" called from %s(%d) %s\n", str, file_err,
          line_err, func_err);
  fprintf(stderr, "------------------------------------------------------------"
                  "----------------\n");
  fprintf(stderr, "Register [rax] = " ELEM_MOD "\n", str->regs[rax]);
  fprintf(stderr, "Register [rbx] = " ELEM_MOD "\n", str->regs[rbx]);
  fprintf(stderr, "Register [rcx] = " ELEM_MOD "\n", str->regs[rcx]);
  fprintf(stderr, "Register [rdx] = " ELEM_MOD "\n", str->regs[rdx]);
  STACK_DUMP(&str->stk_cmd);
  STACK_DUMP(&str->stk_ptr);

  return WITHOUT_ERROR;
}

char *create_src_arr(FILE *fp_src, size_t *sz_file) {
  long start = ftell(fp_src);
  fseek(fp_src, 0L, SEEK_END);
  *sz_file = (size_t)ftell(fp_src);
  fseek(fp_src, start, SEEK_SET);

  char *src_arr = (char *)calloc(*sz_file + 1, sizeof(char));
  fread(src_arr, sizeof(char), *sz_file, fp_src);
  src_arr[*sz_file] = '\0';

  fseek(fp_src, start, SEEK_SET);
  return src_arr;
}

num_t *ptr_reg(Storage *str, int id_reg) { return &str->regs[id_reg]; }
