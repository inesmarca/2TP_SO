/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>

void test1(int argc, char ** argv) {
	printf("Hello World %d", argc);
}

void test2(int argc, char ** argv) {
	printf("Hello World %d", argc);
}

int main() {
	create(test1, 2, 0);
	printf("No deberia verse");
	while(1);
	return 0;
}