#ifndef CMD_DEFINE_H
#define CMD_DEFINE_H

/*------------------------------------------------------------------------------------------------------------*/
/*                               ASSEMBLER COMMANDS DESCRIPTION                                               */
/*------------------------------------------------------------------------------------------------------------*/
#ifdef ASM_H

#define ASM_CMD_WITHOUT_PRM(pass_num, num)                                                                    \
    if (pass_num == 1)                                                                                        \
    {                                                                                                         \
        sz_res_arr++;                                                                                         \
        src_pc++;                                                                                             \
    }                                                                                                         \
    else if (pass_num == 2)                                                                                   \
    {                                                                                                         \
        PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                        \
                              "s",  "----",                                                                   \
                              "d",  num,                                                                      \
                              "s",  "----",                                                                   \
                              "lu", pc);                                                                      \
                                                                                                              \
        PRINT_INFO("name_cmd: %s[%-4s]%s", RED, name_cmd, RESET);                                             \
        PRINT_INFO("%s[%-2d]%s\n", MAGENTA, num, RESET);                                                      \
                                                                                                              \
        res_arr_ptr[pc] = (char)num;                                                                          \
        pc++;                                                                                                 \
    }

#define ASM_PUSH(pass_num, num)                                                                               \
    if (pass_num == 1)                                                                                        \
    {                                                                                                         \
        if (sscanf(src_arr_ptr + src_pc, NUM_MOD_SCAN "%n", &num_user, &ncr) == 1)                            \
        {                                                                                                     \
            src_pc     += (size_t)ncr;                                                                        \
            sz_res_arr += 1 + sizeof(num_t);                                                                  \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%d] %n", &num_cmd, &ncr) == 1)                               \
        {                                                                                                     \
            src_pc     += (size_t)ncr;                                                                        \
            sz_res_arr += 1 + sizeof(int);                                                                    \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%s %c %d] %n", name_cmd, &ch, &num_cmd, &ncr) == 3)          \
        {                                                                                                     \
            src_pc     += (size_t)ncr;                                                                        \
            sz_res_arr += 3 + sizeof(int);                                                                    \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr) == 1)                                  \
        {                                                                                                     \
            sz_res_arr += 2;                                                                                  \
            src_pc     += (size_t)ncr;                                                                        \
        }                                                                                                     \
    }                                                                                                         \
    else if (pass_num == 2)                                                                                   \
    {                                                                                                         \
        PRINT_INFO("name_cmd: %s[%-4s]%s", RED, name_cmd, RESET);                                             \
        PRINT_INFO("%s[%-2d]%s\n", MAGENTA, num, RESET);                                                      \
                                                                                                              \
        if (sscanf(src_arr_ptr + src_pc, NUM_MOD_SCAN "%n", &num_user, &ncr) == 1)                            \
        {                                                                                                     \
            res_arr_ptr[pc] = (char)num;                                                                      \
                                                                                                              \
            memcpy(res_arr_ptr + pc + 1, &num_user, sizeof(num_t));                                           \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",    commands[num] + 4,                                                  \
                                  "s",    "----",                                                             \
                                  "d",    num,                                                                \
                                  ".3lf", num_user,                                                           \
                                  "lu",   pc);                                                                \
                                                                                                              \
            pc     += 1 + sizeof(num_t);                                                                      \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%d] %n", &num_cmd, &ncr) == 1)                               \
        {                                                                                                     \
            res_arr_ptr[pc]   = (char)(num^0x20);                                                             \
                                                                                                              \
            memcpy(res_arr_ptr + pc + 1, &num_cmd, sizeof(int));                                              \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "d",  num_cmd,                                                              \
                                  "d",  num,                                                                  \
                                  "s",  "----",                                                               \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 1 + sizeof(int);                                                                        \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%s %c %d] %n", name_cmd, &ch, &num_cmd, &ncr) == 3)          \
        {                                                                                                     \
            res_arr_ptr[pc]   = (char)(num^0x60);                                                             \
            res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                                \
            res_arr_ptr[pc+2] = ch;                                                                           \
                                                                                                              \
            memcpy(res_arr_ptr + pc + 3, &num_cmd, sizeof(int));                                              \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "d",  num_cmd,                                                              \
                                  "d",  num,                                                                  \
                                  "s",  "----",                                                               \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 3 + sizeof(int);                                                                        \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr) == 1)                                  \
        {                                                                                                     \
            res_arr_ptr[pc]   = (char)(num^0x40);                                                             \
            res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                                \
                                                                                                              \
            if (res_arr_ptr[pc+1] == ошибка_в_имени_регистра)                                                 \
            {                                                                                                 \
                PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_в_имени_регистра]);                                     \
                return ошибка_в_имени_регистра;                                                               \
            }                                                                                                 \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "s",  "----",                                                               \
                                  "d",  num,                                                                  \
                                  "s",  name_cmd,                                                             \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 2;                                                                                      \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
    }                                                                                            

#define ASM_POP(pass_num, num)                                                                                \
    if (pass_num == 1)                                                                                        \
    {                                                                                                         \
        if (sscanf(src_arr_ptr + src_pc, " [%d] %n", &num_cmd, &ncr) == 1)                                    \
        {                                                                                                     \
            src_pc     += (size_t)ncr;                                                                        \
            sz_res_arr += 1 + sizeof(int);                                                                    \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%s %c %d] %n", name_cmd, &ch, &num_cmd, &ncr) == 3)          \
        {                                                                                                     \
            src_pc     += (size_t)ncr;                                                                        \
            sz_res_arr += 3 + sizeof(int);                                                                    \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr) == 1)                                  \
        {                                                                                                     \
            sz_res_arr += 2;                                                                                  \
            src_pc     += (size_t)ncr;                                                                        \
        }                                                                                                     \
    }                                                                                                         \
    else if (pass_num == 2)                                                                                   \
    {                                                                                                         \
        PRINT_INFO("name_cmd: %s[%-4s]%s", RED, name_cmd, RESET);                                             \
        PRINT_INFO("%s[%-2d]%s\n", MAGENTA, num, RESET);                                                      \
                                                                                                              \
        if (sscanf(src_arr_ptr + src_pc, " [%d] %n", &num_cmd, &ncr) == 1)                                    \
        {                                                                                                     \
            res_arr_ptr[pc] = (char)(num^0x40);                                                               \
                                                                                                              \
            memcpy(res_arr_ptr + pc + 1, &num_cmd, sizeof(int));                                              \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "d",  num_cmd,                                                              \
                                  "d",  num,                                                                  \
                                  "s",  "----",                                                               \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 1 + sizeof(int);                                                                        \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
        else if (sscanf(src_arr_ptr + src_pc, " [%s %c %d] %n", name_cmd, &ch, &num_cmd, &ncr) == 3)          \
        {                                                                                                     \
            res_arr_ptr[pc]   = (char)(num^0x20);                                                             \
            res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                                \
            res_arr_ptr[pc+2] = ch;                                                                           \
                                                                                                              \
            memcpy(res_arr_ptr + pc + 3, &num_cmd, sizeof(int));                                              \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "d",  num_cmd,                                                              \
                                  "d",  num,                                                                  \
                                  "s",  "----",                                                               \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 3 + sizeof(int);                                                                        \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
        else if (sscanf (src_arr_ptr + src_pc, "%s %n" , name_cmd, &ncr) == 1)                                \
        {                                                                                                     \
            res_arr_ptr[pc]   = (char)num;                                                                    \
            res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                                \
                                                                                                              \
            if (res_arr_ptr[pc+1] == ошибка_в_имени_регистра)                                                 \
            {                                                                                                 \
                PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_в_имени_регистра]);                                     \
                return ошибка_в_имени_регистра;                                                               \
            }                                                                                                 \
                                                                                                              \
            PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                    \
                                  "s",  "----",                                                               \
                                  "d",  num,                                                                  \
                                  "s",  name_cmd,                                                             \
                                  "lu", pc);                                                                  \
                                                                                                              \
            pc     += 2;                                                                                      \
            src_pc += (size_t)ncr;                                                                            \
        }                                                                                                     \
    }
        
#define ASM_JMP_and_CALL(pass_num, num)                                                                       \
    if (pass_num == 1)                                                                                        \
    {                                                                                                         \
        if (sscanf(src_arr_ptr + src_pc, "%d %n", &num_cmd, &ncr) != 1)                                       \
            sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr);                                            \
                                                                                                              \
        sz_res_arr += 1 + sizeof(int);                                                                        \
        src_pc     += (size_t)ncr;                                                                            \
    }                                                                                                         \
    else if (pass_num == 2)                                                                                   \
    {                                                                                                         \
        PRINT_INFO("name_cmd: %s[%-4s]%s", RED, name_cmd, RESET);                                             \
        PRINT_INFO("%s[%-2d]%s\n", MAGENTA, num, RESET);                                                      \
                                                                                                              \
        res_arr_ptr[pc] = num;                                                                                \
                                                                                                              \
        if (sscanf(src_arr_ptr + src_pc, "%d %n", &num_cmd, &ncr) != 1)                                       \
        {                                                                                                     \
            sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr);                                            \
                                                                                                              \
            for (int i = 1; i <= arr_lab[0].jmp_id; i++)                                                      \
            {                                                                                                 \
                if (!strcmp(arr_lab[i].name_lab, name_cmd))                                                   \
                {                                                                                             \
                    num_cmd = arr_lab[i].jmp_id;                                                              \
                    break;                                                                                    \
                }                                                                                             \
            }                                                                                                 \
        }                                                                                                     \
        memcpy(res_arr_ptr + pc + 1, &num_cmd, sizeof(int));                                                  \
                                                                                                              \
        PRINT_LOG_STR(fp_log, "s",  commands[num] + 4,                                                        \
                              "s",  "----",                                                                   \
                              "d",  num,                                                                      \
                              "d",  num_cmd,                                                                  \
                              "lu", pc);                                                                      \
                                                                                                              \
        pc     += 1 + sizeof(int);                                                                            \
        src_pc += (size_t)ncr;                                                                                \
    }

#define ASM_ORG(pass_num, num)                                                                                \
    if (pass_num == 1)                                                                                        \
    {                                                                                                         \
        sscanf(src_arr_ptr + src_pc, "%d %n", &num_cmd, &ncr);                                                \
                                                                                                              \
        if (ORG_NOT_CORRECT(num_cmd, sz_res_arr))                                                             \
        {                                                                                                     \
            PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_в_директиве_org]);                                          \
            return ошибка_в_директиве_org;                                                                    \
        }                                                                                                     \
                                                                                                              \
        sz_res_arr = (size_t)num_cmd;                                                                         \
        src_pc    += (size_t)ncr;                                                                             \
    }                                                                                                         \
    else if (pass_num == 2)                                                                                   \
    {                                                                                                         \
        PRINT_INFO("name_cmd: %s[%-4s]%s", RED, name_cmd, RESET);                                             \
        PRINT_INFO("%s[%-2d]%s\n", MAGENTA, num, RESET);                                                      \
                                                                                                              \
        sscanf(src_arr_ptr + src_pc, "%d %n", &num_cmd, &ncr);                                                \
                                                                                                              \
        if (ORG_NOT_CORRECT(num_cmd, pc + 1))                                                                 \
        {                                                                                                     \
            PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_в_директиве_org]);                                          \
            return ошибка_в_директиве_org;                                                                    \
        }                                                                                                     \
                                                                                                              \
        for (int i = (int)pc + 1; i < num_cmd; i++)                                                           \
            res_arr_ptr[i] = 0;                                                                               \
                                                                                                              \
        PRINT_LOG_STR(fp_log, "s", commands[num] + 4,                                                         \
                              "s", "----",                                                                    \
                              "d", num,                                                                       \
                              "d", num_cmd,                                                                   \
                              "s", "----");                                                                   \
                                                                                                              \
        pc      = (size_t)num_cmd;                                                                            \
        src_pc += (size_t)ncr;                                                                                \
    }

#endif // !ASM_H


/*------------------------------------------------------------------------------------------------------------*/
/*                                   PROCESSOR COMMANDS DESCRIPTION                                           */
/*------------------------------------------------------------------------------------------------------------*/
#ifdef PROCESSOR_H

#define POP(num_ptr)    StackPop(&str->stk_cmd, num_ptr);
#define POP_PTR(ptr_id) StackPop(&str->stk_ptr, ptr_id);
#define PUSH_PTR(ptr)   StackPush(&str->stk_ptr, ptr);
#define PUSH(num)       StackPush(&str->stk_cmd, num);
#define PTR_REG(id)     ptr_reg(str, id)

#define TWO_POP(num1, num2)                                                       \
    StackPop(&str->stk_cmd, &num1);                                               \
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

#define PROC_PUSH(prm)                                                                  \
    if (num_cmd == (prm^0x40))                                                          \
    {                                                                                   \
        num_cmd = src_arr[pc + 1];                                                      \
        PUSH(*PTR_REG(num_cmd));                                                        \
        pc += 2;                                                                        \
                                                                                        \
        PRINT_INFO("Stack <-- {%s}\n", REGISTER[(int)num_cmd]);                         \
        continue;                                                                       \
    }                                                                                   \
    else if (num_cmd == (prm^0x20))                                                     \
    {                                                                                   \
        memcpy(&id_jmp, &src_arr[pc + 1], sizeof(int));                                 \
        memcpy(&num1, &RAM[id_jmp], sizeof(num_t));                                     \
        PUSH(num1);                                                                     \
        pc += 1 + sizeof(int);                                                          \
                                                                                        \
        PRINT_INFO("Stack <-- RAM[%d]\n", id_jmp);                                      \
        continue;                                                                       \
    }                                                                                   \
    else if (num_cmd == (prm^0x60))                                                     \
    {                                                                                   \
        memcpy(&id_jmp, &src_arr[pc + 3], sizeof(int));                                 \
        int tmp = id_jmp;                                                               \
                                                                                        \
        num_cmd = src_arr[pc+1];                                                        \
        sign    = src_arr[pc+2];                                                        \
                                                                                        \
        switch (sign)                                                                   \
        {                                                                               \
            case '+':                                                                   \
                id_jmp += (int)*PTR_REG(num_cmd);                                       \
                break;                                                                  \
            case '-':                                                                   \
                id_jmp = (int)*PTR_REG(num_cmd) - id_jmp;                               \
                break;                                                                  \
            case '*':                                                                   \
                id_jmp *= (int)*PTR_REG(num_cmd);                                       \
                break;                                                                  \
            case '/':                                                                   \
                id_jmp = (int)*PTR_REG(num_cmd) / id_jmp;                               \
                break;                                                                  \
            default:                                                                    \
                return 1;                                                               \
                break;                                                                  \
        }                                                                               \
        memcpy(&num1, &RAM[id_jmp], sizeof(num_t));                                     \
        PUSH(num1);                                                                     \
                                                                                        \
        pc += 3 + sizeof(int);                                                          \
        PRINT_INFO("Stack <-- RAM[%s %c %d](%d)\n",                                     \
                    REGISTER[(int)num_cmd], sign, tmp, id_jmp);                         \
        continue;                                                                       \
    }                                                                                   \
    else if (num_cmd == prm)                                                            \
    {                                                                                   \
        memcpy(&num1, src_arr + pc + 1, sizeof(num_t));                                 \
        PUSH(num1);                                                                     \
                                                                                        \
        pc += 1 + sizeof(num_t);                                                        \
        PRINT_INFO("Stack <-- File\n");                                                 \
        continue;                                                                       \
    }

#define PROC_POP(prm)                                                                   \
    if (num_cmd == prm)                                                                 \
    {                                                                                   \
        num_cmd = src_arr[pc + 1];                                                      \
        POP(PTR_REG(num_cmd));                                                          \
                                                                                        \
        pc += 2;                                                                        \
        PRINT_INFO("Stack --> {%s}\n", REGISTER[(int)num_cmd]);                         \
        continue;                                                                       \
    }                                                                                   \
    else if (num_cmd == (prm^0x40))                                                     \
    {                                                                                   \
        memcpy(&id_jmp, &src_arr[pc + 1], sizeof(int));                                 \
        POP(&num1);                                                                     \
        memcpy(&RAM[id_jmp], &num1, sizeof(num_t));                                     \
                                                                                        \
        pc += 1 + sizeof(int);                                                          \
        PRINT_INFO("Stack --> RAM[%d]\n", id_jmp);                                      \
        continue;                                                                       \
    }                                                                                   \
    else if (num_cmd == (prm^0x20))                                                     \
    {                                                                                   \
        memcpy(&id_jmp, &src_arr[pc + 3], sizeof(int));                                 \
        int tmp = id_jmp;                                                               \
                                                                                        \
        num_cmd = src_arr[pc+1];                                                        \
        sign    = src_arr[pc+2];                                                        \
                                                                                        \
        switch (sign)                                                                   \
        {                                                                               \
            case '+':                                                                   \
                id_jmp += (int)*PTR_REG(num_cmd);                                       \
                break;                                                                  \
            case '-':                                                                   \
                id_jmp = (int)*PTR_REG(num_cmd) - id_jmp;                               \
                break;                                                                  \
            case '*':                                                                   \
                id_jmp *= (int)*PTR_REG(num_cmd);                                       \
                break;                                                                  \
            case '/':                                                                   \
                id_jmp = (int)*PTR_REG(num_cmd) / id_jmp;                               \
                break;                                                                  \
            default:                                                                    \
                break;                                                                  \
        }                                                                               \
                                                                                        \
        POP(&num1);                                                                     \
        memcpy(&RAM[id_jmp], &num1, sizeof(num_t));                                     \
                                                                                        \
        pc += 3 + sizeof(int);                                                          \
                                                                                        \
        PRINT_INFO("Stack --> RAM[%s %c %d](%d)\n",                                     \
                    REGISTER[(int)num_cmd], sign, tmp, id_jmp);                         \
        continue;                                                                       \
    }                                                                                   \

#define PROC_IN(venom_prm)                                                        \
    printf("Введите число: ");                                                    \
    fscanf(stdin, NUM_MOD_SCAN, &num1);                                           \
    PUSH(num1);                                                                   \
    pc++;                                                                         \
                                                                                  \
    PRINT_INFO("Stack <-- User\n");                                               \
    continue;

#define PROC_SQRT(venom_prm)                                                      \
    POP(&num1);                                                                   \
    PUSH(sqrt(num1));                                                             \
    pc++;                                                                         \
    continue;

#define PROC_SIN(venom_prm)                                                       \
    POP(&num1);                                                                   \
    PUSH(sin(num1));                                                              \
    pc++;                                                                         \
    continue;

#define PROC_COS(venom_prm)                                                       \
    POP(&num1);                                                                   \
    PUSH(cos(num1));                                                              \
    pc++;                                                                         \
    continue;

#define PROC_ADD(venom_prm)                                                       \
    TWO_POP(num1, num2);                                                          \
    PUSH(num2 + num1);                                                            \
    pc++;                                                                         \
    continue;

#define PROC_SUB(venom_prm)                                                       \
    TWO_POP(num1, num2);                                                          \
    PUSH(num2 - num1);                                                            \
    pc++;                                                                         \
    continue;

#define PROC_MUL(venom_prm)                                                       \
    TWO_POP(num1, num2);                                                          \
    PUSH(num2 * num1);                                                            \
    pc++;                                                                         \
    continue;

#define PROC_DIV(venom_prm)                                                       \
    TWO_POP(num1, num2);                                                          \
    PUSH(num2 / num1);                                                            \
    pc++;                                                                         \
    continue;

#define PROC_JMP(venom_prm)                                                       \
    memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));                               \
    pc = (size_t)id_jmp;                                                          \
    continue;

#define PROC_JB(venom_prm)                                                        \
    JMP_IF(num2 < num1, num1, num2);                                              \
    continue;

#define PROC_JBE(venom_prm)                                                       \
    JMP_IF(num2 <= num1, num1, num2);                                             \
    continue;

#define PROC_JA(venom_prm)                                                        \
    JMP_IF(num2 > num1, num1, num2);                                              \
    continue;                                                                     \

#define PROC_JAE(venom_prm)                                                       \
    JMP_IF(num2 >= num1, num1, num2);                                             \
    continue;

#define PROC_JE(venom_prm)                                                        \
    JMP_IF(fabs(num2 - num1) < EPSILON, num1, num2);                              \
    continue;

#define PROC_JNE(venom_prm)                                                       \
    JMP_IF(fabs(num2 - num1) > EPSILON, num1, num2);                              \
    continue;

#define PROC_CALL(venom_prm)                                                      \
    memcpy(&id_jmp, src_arr + pc + 1, sizeof(int));                               \
    PUSH_PTR((ELEM_T)(pc + 1 + sizeof(int)));                                     \
    pc = (size_t)id_jmp;                                                          \
    continue;

#define PROC_RET(venom_prm)                                                       \
    POP_PTR(&num1);                                                               \
    pc = (size_t)num1;                                                            \
    continue;

#define PROC_OUT(venom_prm)                                                       \
    POP(&num1);                                                                   \
    printf("%sОтвет: [%.3lf]%s\n", YELLOW, num1, RESET);                          \
    pc++;                                                                         \
    continue;

#define PROC_VENOM(prm_venom) {};

#endif // !PROCESSOR_H


/*------------------------------------------------------------------------------------------------------------*/
/*                                   DISASSEMBLER COMMANDS DESCRIPTION                                        */
/*------------------------------------------------------------------------------------------------------------*/
#ifdef DASM_H

#define DASM_CMD_WITHOUT_PRM(prm)                                                          \
    fprintf(fp_res, "%s\n", commands[prm] + 4);                                            \
    pc++;                                                                                  \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);             \
    check_cmd = true;

#define DASM_PUSH(prm)                                                                     \
    if (num_cmd != (char)cmd_push)                                                         \
    {                                                                                      \
        fprintf(fp_res, "%s " , commands[prm] + 4);                                        \
        fprintf(fp_res, "%s\n", REGISTER[(int)array[pc+1]]);                               \
        pc += 2;                                                                           \
        PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);         \
    check_cmd = true;                                                                      \
    }                                                                                      \
    else                                                                                   \
    {                                                                                      \
        fprintf(fp_res, "%s ", commands[prm] + 4);                                         \
        memcpy(&num, array + pc + 1, sizeof(num_t));                                       \
        fprintf(fp_res, NUM_MOD_PRINT "\n", num);                                          \
        pc += 1 + sizeof(num_t);                                                           \
        PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);         \
        check_cmd = true;                                                                  \
    }

#define DASM_POP(prm)                                                              \
    fprintf(fp_res, "%s " , commands[prm] + 4);                                    \
    fprintf(fp_res, "%s\n", REGISTER[(int)array[pc+1]]);                           \
    pc += 2;                                                                       \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);     \
    check_cmd = true;


#define DASM_JMP_and_CALL(prm)                                                                         \
    fprintf(fp_res, "%s " , commands[prm] + 4);                                                        \
    memcpy(&jmp_id, array + pc + 1, sizeof(int));                                                      \
    fprintf(fp_res, "%d\n", jmp_id);                                                                   \
    pc += 1 + sizeof(int);                                                                             \
    PRINT_INFO("Name_command: %s[%-4s]%s", MAGENTA, commands[prm] + 4, RESET);                         \
    check_cmd = true;
    
#define DASM_VENOM(prm) {};

#endif // !DASM_H

#endif // !CMD_DEFINE_H
