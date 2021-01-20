make clean
make 
clang-7 -O2 -emit-llvm -c ./tests/loop.c -o ./tests/loop.bc
llvm-dis ./tests/loop.bc -o=./tests/loop.ll
opt-7 -load ./FunctionInfo.so  -function-info ./tests/loop.bc -o ./tests/loopOpt.bc
