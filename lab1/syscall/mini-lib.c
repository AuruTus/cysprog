#include "mini-lib.h"

// see:
//https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

// syscall number for x86-64, in rax
#define SYS_WRITE 1
#define SYS_SLEEP 35
#define SYS_ALARM 37
#define SYS_EXIT 60

// arguments: rdi, rsi, rdx, r10, r8, r9


#define CALL2(n) "movq $"#n", %%rax\n"
#define CALL(n) CALL2(n)

int strlen(char* s) {
    int n = 0;
    while (*s++)
        n++;
    return n;
}

int printf(char* s, ...) {
    // Your code here:
    // TODO();
    return 0;
}

int write(int fd, char* buf, int size) {
    long r;
    asm(CALL(SYS_WRITE)
        "movq %1, %%rdi\n"
        "movq %2, %%rsi\n"
        "movq %3, %%rdx\n"
        "syscall\n"
        "movq %%rax, %0\n"
        : "=r"(r)
        : "r"((long long int)fd), "r"(buf), "r"((long long int)size)
        : "%rax", "%rdi", "%rsi", "%rdx", "memory");
    return (int)r;
}

int puts(char* s) {
    return write(1, s, strlen(s));
}

void exit(int status) {
    // Your code here:
    asm(CALL(SYS_EXIT)
        "movq %0, %%rdi\n"
        "syscall\n"
        : : "r"((long long int)status)
        : "%rax", "%rdi");
}

int alarm(unsigned int seconds) {
    // Your code here:
    asm(CALL(SYS_ALARM)
        "movq %0, %%rdi\n"
        "syscall\n"
        : : "r"((long long int)seconds)
        : "%rax", "%rdi");
}


int sleep(struct timespec* ts, struct timespec* rem) {
    // Your code here:
    // TODO();

}



