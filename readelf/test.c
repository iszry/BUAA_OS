#include<stdio.h>
char msg[]="Hello";
int count;
int count1=1;
int main(){
	printf("Hello world!\n");
	printf("msg:%X\n",msg);
	printf("count:%X\n",&count);
	printf("count1:%X\n",&count1);
	printf("main:%X\n",main);
	printf("addr5661C000:%d\n",*(int*)0x5661C000);
	printf("addr5661C004:%d\n",*(int*)0x5661C004);
	return 0;
}
