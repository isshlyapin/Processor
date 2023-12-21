#ifndef PROC_DEF
#define PROC_DEF

#define POP(num_ptr) StackPop(&str->stk_cmd, num_ptr);
#define POP_PTR(ptr_id) StackPop(&str->stk_ptr, ptr_id);
#define PUSH_PTR(ptr) StackPush(&str->stk_ptr, ptr);
#define PUSH(num) StackPush(&str->stk_cmd, num);
#define PTR_REG(id) ptr_reg(str, id)

#define TWO_POP(num1, num2)                                                      \
    StackPop(&str->stk_cmd, &num1);                                              \
    StackPop(&str->stk_cmd, &num2);

#define JMP_IF(condition, num1, num2)                                             \
    TWO_POP(num1, num2);                                                          \
    if (condition)                                                                \
    {                                                                             \
        memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));                           \
        pc = (size_t)id_jmp;                                                      \
    }                                                                             \
    else                                                                          \
        pc += 1 + sizeof(int);

#define PROC_HLT(venom_prm) break;  

#define PROC_PUSH(prm)                                                        \
    if (num_cmd != prm)                                                       \
    {                                                                         \
        num_cmd = src_arr[pc + 1];                                            \
        PUSH(*PTR_REG(num_cmd));                                              \
        pc += 2;                                                              \
                                                                              \
        PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)num_cmd]);     \
        continue;                                                             \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        memcpy(&num1, src_arr + pc + 1, sizeof(num_t));                       \
        PUSH(num1);                                                           \
        pc += 1 + sizeof(num_t);                                              \
                                                                              \
        PRINT_INFO("Numeric from file: <%.2lf>\n", num1);                     \
        continue;                                                             \
    }

#define PROC_POP(venom_prm)                                                   \
    num_cmd = src_arr[pc + 1];                                                \
    POP(PTR_REG(num_cmd));                                                    \
    pc += 2;                                                                  \
                                                                              \
    PRINT_INFO("Register from file: <%s>\n", REGISTER[(int)num_cmd]);         \
    continue;

#define PROC_IN(venom_prm)                                                    \
    printf("Введите число: ");                                                \
    fscanf(stdin, NUM_MOD_SCAN, &num1);                                       \
    PUSH(num1);                                                               \
    pc++;                                                                     \
                                                                              \
    PRINT_INFO("Numeric user: <%.2lf>\n", num1);                              \
    continue;

#define PROC_SQRT(venom_prm)                                                  \
    POP(&num1);                                                               \
    PUSH(sqrt(num1));                                                         \
    pc++;                                                                     \
    continue;

#define PROC_SIN(venom_prm)                                                   \
    POP(&num1);                                                               \
    PUSH(sin(num1));                                                          \
    pc++;                                                                     \
    continue;

#define PROC_COS(venom_prm)                                                   \
    POP(&num1);                                                               \
    PUSH(cos(num1));                                                          \
    pc++;                                                                     \
    continue;

#define PROC_ADD(venom_prm)                                                   \
    TWO_POP(num1, num2);                                                      \
    PUSH(num2 + num1);                                                        \
    pc++;                                                                     \
    continue;

#define PROC_SUB(venom_prm)                                                   \
    TWO_POP(num1, num2);                                                      \
    PUSH(num2 - num1);                                                        \
    pc++;                                                                     \
    continue;

#define PROC_MUL(venom_prm)                                                   \
    TWO_POP(num1, num2);                                                      \
    PUSH(num2 * num1);                                                        \
    pc++;                                                                     \
    continue;

#define PROC_DIV(venom_prm)                                                   \
    TWO_POP(num1, num2);                                                      \
    PUSH(num2 / num1);                                                        \
    pc++;                                                                     \
    continue;

#define PROC_JMP(venom_prm)                                                   \
    memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));                           \
    pc = (size_t)id_jmp;                                                      \
    continue;

#define PROC_JB(venom_prm)                                                    \
    JMP_IF(num2 < num1, num1, num2);                                          \
    continue;

#define PROC_JBE(venom_prm)                                                   \
    JMP_IF(num2 <= num1, num1, num2);                                         \
    continue;

#define PROC_JA(venom_prm)                                                    \
    JMP_IF(num2 > num1, num1, num2);                                          \
    continue;                                                                 \

#define PROC_JAE(venom_prm)                                                   \
    JMP_IF(num2 >= num1, num1, num2);                                         \
    continue;

#define PROC_JE(venom_prm)                                                    \
    JMP_IF(fabs(num2 - num1) < EPSILON, num1, num2);                          \
    continue;

#define PROC_JNE(venom_prm)                                                   \
    JMP_IF(fabs(num2 - num1) > EPSILON, num1, num2);                          \
    continue;

#define PROC_CALL(venom_prm)                                                  \
    memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));                           \
    PUSH_PTR((ELEM_T)(pc + 1 + sizeof(int)));                                 \
    pc = (size_t)id_jmp;                                                      \
    continue;

#define PROC_RET(venom_prm)                                                   \
    POP_PTR(&num1);                                                           \
    pc = (size_t)num1;                                                        \
    continue;

#define PROC_OUT(venom_prm)                                                   \
    POP(&num1);                                                               \
    printf("%sОтвет: [%.2lf]%s\n", YELLOW, num1, RESET);                      \
    pc++;                                                                     \
    continue;

#define PROC_VENOM(prm_venom) {};
#endif