CXXFLAGS="-D _DEBUG -ggdb3 -g -std=c++23 -O0 -Wall -Wextra -Weffc++ \
-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations  \
-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
-fsized-deallocation -fstack-protector -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=16384 \
-Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,\
alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,\
returns-nonnull-attribute,shift,signed-integer-overflow,undefined,\
unreachable,vla-bound,vptr"

ASM_START_FILE="program-txt/src/src-factorial-with_label.txt program-txt/res/res_asm.txt log/log_file.txt"

cd ..

g++ "-DINFO" "-DLOG" $CXXFLAGS assembler/src/asm.cpp -o bin/asm 
bin/asm $ASM_START_FILE