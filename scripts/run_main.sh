#!/bin/bash

. ./flags

# ASM_SRC_FILE="discriminant.txt"
# ASM_SRC_FILE="endless_cycle.txt"
# ASM_SRC_FILE="factorial_label.txt"
# ASM_SRC_FILE="factorial_org.txt"
# ASM_SRC_FILE="loop_condition.txt"
# ASM_SRC_FILE="num_fib.txt"
# ASM_SRC_FILE="ram_test.txt"
ASM_SRC_FILE="solve_square.txt"

ASM_LOG_FILE="log_file.txt"

ASM_START_STR="program-txt/src/$ASM_SRC_FILE program-txt/res/res_asm.bin log/$ASM_LOG_FILE"
PROC_START_FILE="program-txt/res/res_asm.bin"

PATH_ASM_SRC="assembler/src/asm.c"
PATH_ASM_BIN="bin/asm"

cd ~/GIT/processor

if [[ ! $1 =~ "f" ]]; then
    CFLAGS=""
fi

if [[ $1 =~ "i" ]]; then
    CFLAGS+=" -DINFO"
    LIB_MYSTACK="-lmystack_with_info"
else
    LIB_MYSTACK="-lmystack_without_info"
fi

if [[ $1 =~ "l" ]]; then
    CFLAGS+=" -DLOG"
    PATH_ASM_SRC+=" assembler/src/create_log.c"
fi
 
FILE=asm
if test -f "$FILE"; then
    bin/asm $ASM_START_STR
else
    gcc $CFLAGS $PATH_ASM_SRC -o bin/asm;
    bin/asm $ASM_START_STR
fi

FILE=program-txt/res/res_asm.bin
if [ ! -f "$FILE" ]; then
	echo "Ошибка: Файл res_asm.bin не найден"
    exit 1
fi

gcc $CFLAGS processor/src/*.c -o bin/main -lm $LIB_MYSTACK

FILE=bin/main
if [ ! -f "$FILE" ]; then
    echo "Ошибка: Файлы main.c и processor.c не были скомпилированы"
	exit 1
else
	bin/main $PROC_START_FILE 
fi
