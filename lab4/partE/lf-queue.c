#include <stdio.h>
#include <stdlib.h>
#include "lf-queue.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void lf_queue_init(lf_queue_t* queue) {
    // node_t* node = (node_t*)calloc(1, sizeof(*node));
    atomic_store(&queue->front, NULL);
    atomic_store(&queue->rear, NULL);
    atomic_store(&queue->size, 0);
}


void lf_queue_enqueue(lf_queue_t* queue, int value) {
    // Exercise 2: fill in your code:
    // Add your code here:
    node_t* new = calloc(1, sizeof(*new));
    new->value = value;
    new->next = NULL;
    while (1) {
        node_t* old_rear = atomic_load(&queue->rear);
        if (atomic_compare_exchange_strong(&queue->rear, &old_rear, new)) {
            atomic_fetch_add(&queue->size, 1);
            if (old_rear == NULL) {
                while (!atomic_compare_exchange_strong(&queue->front, &old_rear, new)) {}
            } else { old_rear->next = new; }
            break;
        }
    }
}

int lf_queue_dequeue(lf_queue_t* queue) {
    // Exercise 2: fill in your code:
    // Add your code here:
    node_t* old_front;
    node_t* old_next;
    while (1) {
        old_front = atomic_load(&queue->front);
        if (old_front == NULL)
            return -1;
        old_next = old_front->next;
        if (atomic_compare_exchange_strong(&(queue->front), &old_front, old_next)) {
            atomic_fetch_add(&queue->size, -1);
            break;
        }
    }
    int val = old_front->value;
    free(old_front);
    return val;
}

int lf_queue_size(lf_queue_t* queue) {
    // Exercise 2: fill in your code:
    // Add your code here:
    return atomic_load(&queue->size);
}
