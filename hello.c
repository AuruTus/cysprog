#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* arg[]) {
	printf("hello, world\n");
	int cnt = 0;
	while (1) {
		printf("count=%d\n", cnt++);
		sleep(1);
	}
	return 0;
}
