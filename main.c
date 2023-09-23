#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid = fork();
	if (pid == 0) {
		// execlp("./bin/hello.out", "hello.out", NULL);
		execlp("./bin/calc_circle_area.out", "calc_circle_area.out", "1.", NULL);
	} else if (pid > 0) {
		// sleep(2);
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			printf("parent process: %d\n",
				WEXITSTATUS(status));
		}
	} else {
		printf("for error occurred.\n");
		exit(-1);
	}

	return 0;
}
