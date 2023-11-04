#include <stdio.h>
#include <stdlib.h>
#include "concur-stack.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void concur_stack_init(concur_stack_t* stack) {
    pthread_mutex_init(&stack->mu, NULL);
    stack->top = (node_t*)malloc(sizeof(node_t));
    stack->top->next = NULL;
    stack->size = 0;
    return;
}

void concur_stack_push(concur_stack_t* stack, int value) {
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    node_t* new_top = (node_t*)malloc(sizeof(node_t));
    new_top->value = value;
    new_top->next = stack->top;
    stack->top = new_top;
    stack->size++;
    pthread_mutex_unlock(&stack->mu);
}

int concur_stack_pop(concur_stack_t* stack) {
    int value = -1;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    if (stack->top->next == NULL) {
        return value;
    }
    value = stack->top->value;
    stack->top = stack->top->next;
    stack->size--;
    pthread_mutex_unlock(&stack->mu);
    return value;
}

int concur_stack_size(concur_stack_t* stack) {
    int size = 0;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    size = stack->size;
    pthread_mutex_unlock(&stack->mu);
    return size;
}
