clang++ -c Hello.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so Hello.o `llvm-config --ldflags`
opt -load ./pass.so -hello <hello.bc> /dev/null

