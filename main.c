#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./bin/hello.out", "hello.out", NULL, NULL);
	} else if (pid > 0) {
		// sleep(2);
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			printf("parent process: %d\n", WEXITSTATUS(status));
		}
	} else {
		exit(1);
	}

	return 0;
}
