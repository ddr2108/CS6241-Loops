#include <stdio.h>
#include <stdlib.h>
void third(){
  printf("hello world\n");

	return;

}

void second(){
  printf("hello world\n");

	return;
  
}


int main() {
  second();
int i;
while(rand()%1000!=3);
{
i++;
while(rand()%5!=3){
	if (i==4){
		break;
}
}}

  return 0;
}

//clang hello.c -S -emit-llvm; opt -dot-cfg hello.ll;dot -Tpng cfg.main.dot > main.png
