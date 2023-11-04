#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




#include "concur-hash.h"

// hash
/* this is the D. J. Bernstein hash function */
static unsigned int djb_hash(const char* cp) {
    unsigned hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char)*cp++;
    return hash;
}

void hash_init(hash_t* ht, int num_buckets) {
    pthread_mutex_init(&ht->mu, 0);
    ht->buckets = calloc(num_buckets, sizeof(*ht->buckets));
    for (int i = 0; i < num_buckets; i++) {
        ht->buckets[i] = calloc(1, sizeof(node_t));
        ht->buckets[i]->next = NULL;
    }
    ht->num_buckets = num_buckets;
    ht->size = 0;
}

void hash_insert(hash_t* ht, char* key, char* value) {
    // Exercise 2: complete the code below:
    // Add your code here:
    int hv = djb_hash(key); // the hash value
    int n = hv % ht->num_buckets; // the bucket number
    while (n < 0) {
        n += ht->num_buckets;
    }
    pthread_mutex_lock(&ht->mu);
    node_t* tmp = calloc(1, sizeof(node_t));
    tmp->key = key;
    tmp->value = value;
    tmp->next = ht->buckets[n]->next;
    ht->buckets[n]->next = tmp;
    ht->size++;
    pthread_mutex_unlock(&ht->mu);
}

char* hash_lookup(hash_t* ht, char* key) {
    // Exercise 2: complete the code below:
    // Add your code here:
    int hv = djb_hash(key); // the hash value
    int n = hv % ht->num_buckets; // the bucket number
    while (n < 0) {
        n += ht->num_buckets;
    }
    char* val = NULL;
    pthread_mutex_lock(&ht->mu);
    node_t* curr = ht->buckets[n]->next;
    while (curr) {
        if (strcmp(curr->key, key) != 0) {
            curr = curr->next;
            continue;
        }
        val = curr->value;
        break;
    }
    pthread_mutex_unlock(&ht->mu);
    return val;
}

// return 0 for success; 1 for non-exiting "key"
int hash_delete(hash_t* ht, char* key) {
    // Exercise 2: complete the code below:
    // Add your code here:
    int hv = djb_hash(key); // the hash value
    int n = hv % ht->num_buckets; // the bucket number
    while (n < 0) {
        n += ht->num_buckets;
    }
    pthread_mutex_lock(&ht->mu);
    node_t* front = ht->buckets[n];
    node_t* curr = ht->buckets[n]->next;
    while (curr) {
        if (strcmp(curr->key, key) != 0) {
            front = curr;
            curr = curr->next;
            continue;
        }
        front->next = curr->next;
        free(curr);
        ht->size--;
        break;
    }
    pthread_mutex_unlock(&ht->mu);
    return 1;
}

int hash_size(hash_t* ht) {
    int size = 0;
    // Exercise 2: complete the code below:
    // Add your code here:
    pthread_mutex_lock(&ht->mu);
    size = ht->size;
    pthread_mutex_unlock(&ht->mu);
    return size;
}
