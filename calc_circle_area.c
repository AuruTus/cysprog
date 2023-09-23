#include <stdio.h>
#include <stdlib.h>

const double PI = 3.1415926;

int main(int argc, char* arg[]) {
	if (argc != 2) {
		printf("calc_circle_area needs at least one radius arg");
		return 1;
	}

	double rd;
	double res;

	for (int i = 1; i <= argc; ++i) {
		char* _;
		rd = strtod(arg[i], &_);
		res = PI * rd * rd;
		printf("circle_area(rd=%.3f) = %.3f\n", rd, res);
	}

	return 0;
}
