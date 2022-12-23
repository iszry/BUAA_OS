#include<stdio.h>
#include<pthread.h>
char msg[]="Hello world!\n";
int count;
int main(){
	printf("Hello world!\n");
	printf("msg:%X\n",msg);
	printf("count:%X\n",&count);
	printf("main:%X\n",main);
	return 0;
}
