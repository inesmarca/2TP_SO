/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>
#include <shell.h>

static int fd[MAX_PROCESS] = {STDIN, STDOUT, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int main() {
	printf("Initialize shell");
	if (createForeground("shell", shell, 0, fd, 0, 0) == -1)
		printf("Esta fallando la creacion del shell");
	return -1;
}