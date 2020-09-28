/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>

void test1() {
	printf("Hello World");
}

void test2() {
	printf("Hello World");
}

int main() {
	create(programHandler, 0, 0);
	while(1);
	return 0;
}