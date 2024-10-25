
#ifndef MINI_LIB_H
#define MINI_LIB_H

//#include <time.h>

struct timespec {
    long long tv_sec;
    long long tv_nsec;
};

int alarm(unsigned int seconds);
void exit(int status);
int printf(char *s, ...);
int puts(char *s);
int scanf(char *buf);
int sleep(struct timespec *, struct timespec *);
int strlen(char *s);


#endif


