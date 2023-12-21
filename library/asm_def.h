#define ASM_CMD_WITHOUT_PRM(pass_num, num)                                                       \
    if (pass_num == 1)                                                                           \
        sz_res_arr++;                                                                            \
    else if (pass_num == 2)                                                                      \
    {                                                                                            \
        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[num] + 4, num);                   \
        PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", "---", pc);                                        \
                                                                                                 \
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);                                 \
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num, RESET);                                          \
                                                                                                 \
        res_arr_ptr[pc] = (char)num;                                                             \
        pc++;                                                                                    \
    }

#define ASM_PUSH(pass_num, num)                                                                  \
    if (pass_num == 1)                                                                           \
    {                                                                                            \
        if (sscanf(src_arr_ptr + src_pc, NUM_MOD_SCAN "\n%n", &num_user, &ncr) == 1)             \
        {                                                                                        \
            src_pc     += ncr;                                                                   \
            sz_res_arr += 1 + sizeof(num_t);                                                     \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            sz_res_arr += 2;                                                                     \
            src_pc     += LEN_NAME_REG + 1;                                                      \
        }                                                                                        \
    }                                                                                            \
    else if (pass_num == 2)                                                                      \
    {                                                                                            \
        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[num] + 4, num);                   \
                                                                                                 \
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);                                 \
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num, RESET);                                          \
                                                                                                 \
        if (sscanf(src_arr_ptr + src_pc, NUM_MOD_SCAN "\n%n", &num_user, &ncr) == 1)             \
        {                                                                                        \
            res_arr_ptr[pc] = (char)num;                                                         \
            memcpy(res_arr_ptr + pc + 1, &num_user, sizeof(num_t));                              \
                                                                                                 \
            PRINT_LOG("PRM<" NUM_MOD_PRINT "> CMD_ID <%lu>\n", num_user, pc);                    \
                                                                                                 \
            pc     += 1 + sizeof(num_t);                                                         \
            src_pc += ncr;                                                                       \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            sscanf (src_arr_ptr + src_pc, "%s \n%n", name_cmd, &ncr);                            \
            res_arr_ptr[pc]   = 53;                                                              \
            res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                   \
                                                                                                 \
            if (res_arr_ptr[pc+1] == ERROR_REG_NAME)                                             \
            {                                                                                    \
                fprintf(stderr, "%s\n", ERROR_TEXT[5]);                                          \
                return ERROR_REG_NAME;                                                           \
            }                                                                                    \
                                                                                                 \
            PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", name_cmd, pc);                                 \
                                                                                                 \
            pc     += 2;                                                                         \
            src_pc += ncr;                                                                       \
                                                                                                 \
        }                                                                                        \
    }                                                                                            

#define ASM_POP(pass_num, num)                                                                    \
    if (pass_num == 1)                                                                            \
    {                                                                                             \
        sz_res_arr += 2;                                                                          \
        src_pc     += LEN_NAME_REG + 1;                                                           \
    }                                                                                             \
    else if (pass_num == 2)                                                                       \
    {                                                                                             \
        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[num] + 4, num);                    \
                                                                                                  \
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);                                  \
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num, RESET);                                           \
                                                                                                  \
        res_arr_ptr[pc] = num;                                                                    \
                                                                                                  \
        sscanf (src_arr_ptr + src_pc, "%s \n%n" , name_cmd, &ncr);                                \
        res_arr_ptr[pc+1] = (char)check_num_reg(name_cmd);                                        \
        if (res_arr_ptr[pc+1] == ERROR_REG_NAME)                                                  \
        {                                                                                         \
            fprintf(stderr, "%s\n", ERROR_TEXT[5]);                                               \
            return ERROR_REG_NAME;                                                                \
        }                                                                                         \
                                                                                                  \
        PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", name_cmd, pc);                                      \
                                                                                                  \
        pc     += 2;                                                                              \
        src_pc += ncr;                                                                            \
    }

#define ASM_JMP_and_CALL(pass_num, num)                                                           \
    if (pass_num == 1)                                                                            \
    {                                                                                             \
        if (sscanf(src_arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr) != 1)                         \
            sscanf(src_arr_ptr + src_pc, "%s \n%n", name_cmd, &ncr);                              \
                                                                                                  \
        sz_res_arr += 1 + sizeof(int);                                                            \
        src_pc += ncr;                                                                            \
    }                                                                                             \
    else if (pass_num == 2)                                                                       \
    {                                                                                             \
        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[num] + 4, num);                    \
                                                                                                  \
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);                                  \
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num, RESET);                                           \
                                                                                                  \
        res_arr_ptr[pc] = num;                                                                    \
                                                                                                  \
        if (sscanf(src_arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr) != 1)                         \
        {                                                                                         \
            sscanf(src_arr_ptr + src_pc, "%s \n%n", name_cmd, &ncr);                              \
            for (int i = 1; i <= arr_lab[0].jmp_id; i++)                                          \
            {                                                                                     \
                if (!strcmp(arr_lab[i].name_lab, name_cmd))                                       \
                {                                                                                 \
                    num_cmd = arr_lab[i].jmp_id;                                                  \
                    break;                                                                        \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
        memcpy(res_arr_ptr + pc + 1, &num_cmd, sizeof(int));                                      \
                                                                                                  \
        PRINT_LOG("PRM<%-9d> CMD_ID <%lu>\n", num_cmd, pc);                                       \
                                                                                                  \
        pc     += 1 + sizeof(int);                                                                \
        src_pc += ncr;                                                                            \
    }

#define ASM_ORG(pass_num, num)                                                                    \
    if (pass_num == 1)                                                                            \
    {                                                                                             \
        sscanf(src_arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr);                                  \
                                                                                                  \
        if (ORG_NOT_CORRECT(num_cmd, sz_res_arr))                                                 \
        {                                                                                         \
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_ORG_DIR]);                                   \
            return ERROR_ORG_DIR;                                                                 \
        }                                                                                         \
                                                                                                  \
        sz_res_arr = (size_t)num_cmd;                                                             \
        src_pc += ncr;                                                                            \
    }                                                                                             \
    else if (pass_num == 2)                                                                       \
    {                                                                                             \
        PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[num] + 4, num);                    \
                                                                                                  \
        PRINT_INFO("name_cmd: %s[%4s]%s", RED, name_cmd, RESET);                                  \
        PRINT_INFO("%s[%2d]%s\n", MAGENTA, num, RESET);                                           \
                                                                                                  \
        sscanf(src_arr_ptr + src_pc, "%d \n%n", &num_cmd, &ncr);                                  \
                                                                                                  \
        if (ORG_NOT_CORRECT(num_cmd, pc + 1))                                                     \
        {                                                                                         \
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_ORG_DIR]);                                   \
            return ERROR_ORG_DIR;                                                                 \
        }                                                                                         \
                                                                                                  \
        for (int i = (int)pc + 1; i < num_cmd; i++)                                               \
            res_arr_ptr[i] = 0;                                                                   \
                                                                                                  \
        PRINT_LOG("PRM<%-9d> CMD_ID <%s>\n", num_cmd, "---");                                     \
                                                                                                  \
        pc = (size_t)num_cmd;                                                                     \
        src_pc += ncr;                                                                            \
    }