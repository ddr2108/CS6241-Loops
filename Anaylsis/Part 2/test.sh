clang++ -c p32.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p32.o `llvm-config --ldflags`
opt -load ./pass.so -p32 <../../Test/hello.bc> /dev/null

