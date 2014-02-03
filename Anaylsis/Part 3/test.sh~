clang++ -c p33.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p33.o `llvm-config --ldflags`
opt -load ./pass.so -p33 <hello.bc> /dev/null

