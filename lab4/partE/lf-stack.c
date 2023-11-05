#include <stdio.h>
#include <stdlib.h>
#include "lf-stack.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void lf_stack_init(lf_stack_t* stack) {
    atomic_store(&stack->top, 0);
    atomic_store(&stack->size, 0);
    return;
}

void lf_stack_push(lf_stack_t* stack, int value) {
    // Exercise 1: lock-free stack.
    // Add your code here:
    Node_t* new = calloc(1, sizeof(*new));
    new->value = value;
    while (1) {
        void* old_top = atomic_load(&stack->top);
        new->next = old_top;
        if (atomic_compare_exchange_strong(&(stack->top), &old_top, new)) {
            atomic_fetch_add(&stack->size, 1);
            break;
        }
    }
}

int lf_stack_pop(lf_stack_t* stack) {
    // Exercise 1: lock-free stack
    // Add your code here:
    Node_t* old_top;
    Node_t* old_next;
    while (1) {
        old_top = atomic_load(&stack->top);
        if (old_top == 0) // empty stack
            return -1;
        old_next = old_top->next;
        if (atomic_compare_exchange_strong(&(stack->top), &old_top, old_next)) {
            atomic_fetch_add(&stack->size, -1);
            break;
        }
    }
    int val = old_top->value;
    free(old_top);
    return val;
}



int lf_stack_size(lf_stack_t* stack) {
    // Exercise 1: lock-free stack
    // Add your code here:
    return atomic_load(&stack->size);
}

