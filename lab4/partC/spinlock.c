#include<pthread.h>
#include<stdio.h>
#include<stdatomic.h>
#include <assert.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




#include "spinlock.h"

void spinlock_init(spinlock_t* lock) {
    // Exercise 2:
    // Add your code here:
    atomic_init(&lock->n, 0);
}

void spinlock_lock(spinlock_t* lock) {
    // Exercise 2:
    // Add your code here:
    int n = 0;
    while (!atomic_compare_exchange_strong(&lock->n, &n, 1)) {
        n = 0;
    }
}

void spinlock_unlock(spinlock_t* lock) {
    // Exercise 2:
    // Add your code here:
    atomic_store(&lock->n, 0);
}

typedef struct {
    spinlock_t lock;
    int num;
} counter_t;

counter_t counter;

void* start() {
    for (int i = 0; i < 10000; i++) {
        // Exercise 2:
        // Add your code here:
        spinlock_lock(&counter.lock);
        counter.num++;
        spinlock_unlock(&counter.lock);
    }
    pthread_exit(0);
}

int main() {
    pthread_t pids[10];

    spinlock_init(&counter.lock);

    for (int i = 0; i < 10; i++) {
        pthread_create(&pids[i], NULL, start, NULL);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(pids[i], NULL);
    }

    printf("Counter: %d\n", counter.num);

    assert(counter.num == 100000);
    printf("Testing spinlock: PASS");
    return 0;
}