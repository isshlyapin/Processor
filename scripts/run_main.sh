CXXFLAGS="-D _DEBUG -ggdb3 -g -std=c++23 -O0 -Wall -Wextra -Weffc++ \
-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations  \
-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 \
-Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros \
-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
-fsized-deallocation -fstack-protector -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=49152 \
-Wstack-usage=8192 -fPIE -Werror=vla -fsanitize=address,\
alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,\
returns-nonnull-attribute,shift,signed-integer-overflow,undefined,\
unreachable,vla-bound,vptr"

ASM_START_FILE="program-txt/src/ram_test.txt program-txt/res/res_asm.txt log/log_test.txt"
PROC_START_FILE="program-txt/res/res_asm.txt"

PATH_ASM_SRC="assembler/src/asm.cpp"
PATH_ASM_BIN="bin/asm"

cd ~/GIT/processor

if [[ ! $1 =~ "f" ]]; then
    CXXFLAGS=""
fi

if [[ $1 =~ "i" ]]; then
    CXXFLAGS+=" -DINFO"
    LIB_MYSTACK="-lmystack_with_info"
else
    LIB_MYSTACK="-lmystack_without_info"
fi

if [[ $1 =~ "l" ]]; then
    CXXFLAGS+=" -DLOG"
    PATH_ASM_SRC+=" assembler/src/create_log.cpp"
fi
 
FILE=asm
if test -f "$FILE"; then
    bin/asm $ASM_START_FILE
else
    g++ $CXXFLAGS $PATH_ASM_SRC -o bin/asm && bin/asm $ASM_START_FILE
fi

FILE=program-txt/res/res_asm.txt
if [ ! -f "$FILE" ]; then
	echo "Ошибка: Файл res_asm.txt не найден"
    exit 1
fi

g++ $CXXFLAGS processor/src/*.cpp -o bin/main $LIB_MYSTACK

FILE=bin/main
if [ ! -f "$FILE" ]; then
    echo "Ошибка: Файлы main.cpp и processor.cpp не были скомпилированы"
	exit 1
else
	bin/main $PROC_START_FILE 
fi