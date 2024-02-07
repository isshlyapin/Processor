CFLAGS="-D _DEBUG -ggdb3 -g -O0 -Wall -Wextra \
-Waggressive-loop-optimizations -Wmissing-declarations  \
-Wcast-align -Wcast-qual -Wchar-subscripts \
-Wconversion -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 \
-Winline -Wlogical-op -Wopenmp-simd \
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
-Wsign-conversion -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wunreachable-code -Wunused -Wvariadic-macros \
-Wno-missing-field-initializers -Wno-narrowing \
-Wno-varargs -Wstack-protector -fcheck-new \
-fstack-protector -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=49152 \
-Wstack-usage=8192 -fPIE -Werror=vla -fsanitize=address,\
alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,\
returns-nonnull-attribute,shift,signed-integer-overflow,undefined,\
unreachable,vla-bound,vptr"

ASM_SRC_FILE="ram_test.txt"
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
