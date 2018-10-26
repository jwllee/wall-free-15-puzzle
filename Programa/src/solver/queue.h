//
// Created by jonathan on 10/25/18.
//

#pragma once

#include "hashtable.h"
#include <stdbool.h>


typedef struct hash_backed_priority_queue HashBackedPriorityQueue;

struct hash_backed_priority_queue
{
    HashTable *items;
    int **heap;
    int arr_size;
    int heap_size;

    int(*get_priority)(int *);
};


HashBackedPriorityQueue * pq_init(int max, int n_slots, double load,
        int(*get_priority)(int *),
        unsigned long(*hash)(int *, int));
void pq_destroy(HashBackedPriorityQueue *pq_p, bool destroy_item);

void pq_add(int *item, HashBackedPriorityQueue *pq_p);
bool pq_contains(int *item, HashBackedPriorityQueue *pq_p, bool(*compar)(int *, int *));
int * pq_peek(HashBackedPriorityQueue *pq_p);
int * pq_poll(HashBackedPriorityQueue *pq_p);
int pq_size(HashBackedPriorityQueue *pq_p);
void pq_update_priority(HashBackedPriorityQueue *pq_p);
