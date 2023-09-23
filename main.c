#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./bin/hello.out", "hello.out", 0, 0);
	} else if (pid > 0) {
		sleep(2);
		printf("parent process\n");
	} else {
		exit(1);
	}

	return 0;
}
