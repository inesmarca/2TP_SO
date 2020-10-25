/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>
#include <shell.h>

int main() {
	int fd[MAX_PROCESS];
	memset(fd, -1, MAX_PROCESS);
	fd[0] = STDIN;
	fd[1] = STDOUT;

	printf("Initialize shell");
	if (createForeground("shell", shell, 0, fd, 0, 0) == -1)
		printf("Esta fallando la creacion del shell");
	return -1;
}