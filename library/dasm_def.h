#ifndef DASM_DEF
#define DASM_DEF

#define DASM_CMD_WITHOUT_PRM(prm)                                            \
    fprintf(fp_res, "%s\n", commands[prm] + 4);                                             \
    pc++;                                                                             \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);             \
    check_cmd = true;

#define DASM_PUSH(prm)                                                                \
    if (num_cmd != (char)cmd_push)                                                    \
    {                                                                                 \
        fprintf(fp_res, "%s " , commands[prm] + 4);                                       \
        fprintf(fp_res, "%s\n", REGISTER[(int)array[pc+1]]);                               \
        pc += 2;                                                                      \
        PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);         \
    check_cmd = true;\
    }                                                                                 \
    else                                                                              \
    {                                                                                 \
        fprintf(fp_res, "%s ", commands[prm] + 4);                                        \
        memcpy(&num, array + pc + 1, sizeof(num_t));                                  \
        fprintf(fp_res, NUM_MOD_PRINT "\n", num);                                     \
        pc += 1 + sizeof(num_t);                                                      \
        PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);         \
        check_cmd = true;\
    }

#define DASM_POP(prm)                                                        \
    fprintf(fp_res, "%s " , commands[prm] + 4);                                  \
    fprintf(fp_res, "%s\n", REGISTER[(int)array[pc+1]]);                          \
    pc += 2;                                                                 \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);    \
    check_cmd = true;


#define DASM_JMP_and_CALL(prm)                                                                    \
    fprintf(fp_res, "%s " , commands[prm] + 4);                                                       \
    memcpy(&jmp_id, array + pc + 1, sizeof(int));                                                 \
    fprintf(fp_res, "%d\n", jmp_id);                                                              \
    pc += 1 + sizeof(int);                                                                        \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);                         \
    check_cmd = true;
    
#define DASM_VENOM(prm) {};

#endif