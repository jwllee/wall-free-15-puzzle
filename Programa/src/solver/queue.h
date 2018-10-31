//
// Created by jonathan on 10/25/18.
//

#pragma once

#include "hashtable.h"
#include <stdbool.h>


typedef struct hash_backed_priority_queue HashBackedPriorityQueue;

/**
 * HashBackedPriorityQueue is a priority queue uses a hashtable to enable O(1) complexity for
 * checking existence of arbitrary element. This would take O(nlgn) without the queue since the
 * heap would have to be sorted and then searched using binary search.
 *
 * This modified data structure is taken from the Java implementation of the same modified data
 * structure found in https://svn.win.tue.nl/trac/prom/browser/Packages/Alignment/Trunk/src/nl/tue/alignment/algorithms/datastructures/HashBackedPriorityQueue.java.
 * However, unlike the Java implementation, update of item priority is not O(1) but O(n) since we
 * do not maintain the queue position of a record. This decision sacrifices time for memory.
 */
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
int * pq_get(int *item, HashBackedPriorityQueue *pq_p, bool(*compar)(int *, int *));
bool pq_contains(int *item, HashBackedPriorityQueue *pq_p, bool(*compar)(int *, int *));
int * pq_peek(HashBackedPriorityQueue *pq_p);
int * pq_poll(HashBackedPriorityQueue *pq_p);
int pq_size(HashBackedPriorityQueue *pq_p);
void pq_update_priority(HashBackedPriorityQueue *pq_p);
