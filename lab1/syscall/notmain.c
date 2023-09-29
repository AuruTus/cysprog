#include "mini-lib.h"

struct timespec ts = { .tv_sec = 1, .tv_nsec = 0 };

int notmain() {
    alarm(1);

    for (;;) {
        puts("hello, world\n");
        sleep(&ts, 0);
    }

    exit(0);
}


