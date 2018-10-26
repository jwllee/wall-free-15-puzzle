//
// Created by jonathan on 10/25/18.
//

#include "queue.h"
#include "state.h"
#include "hashtable.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


HashBackedPriorityQueue * pq_init(int max, int n_slots, double load,
        int(*get_priority)(int *), unsigned long(*hash)(int *, int))
{
    HashBackedPriorityQueue *pq = malloc(sizeof(HashBackedPriorityQueue));
    pq->items = init_hashtable(max, n_slots, load, hash);
    pq->arr_size = max;
    pq->heap_size = 0;
    pq->heap = malloc(max * sizeof(int *));
    pq->get_priority = get_priority;
    return pq;
}


void pq_destroy(HashBackedPriorityQueue *pq_p, bool destroy_item)
{
    hashtable_destroy(pq_p->items, destroy_item);
    free(pq_p->heap);
    free(pq_p);
}


// min heap functions
void heap_swap(int i, int j, int **heap)
{
    int *tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}


void heap_sift_up(int k, int **heap_p, int(*get_key)(int *))
{
    int *item, parent_ind, *parent;
    while (k > 0)
    {
        parent_ind = k / 2;
        item = heap_p[k];
        parent = heap_p[parent_ind];

        // is min heap property satisfied?
        if (get_key(item) >= get_key(parent))
        {
            break;
        }

        heap_swap(k, parent_ind, heap_p);
        k = parent_ind;
    }
}


void heap_sift_down(int k, int **heap_p, int heap_size, int(*get_key)(int *))
{
    int *item, child_ind, *child;
    while (k * 2 < heap_size)
    {
        child_ind = k * 2;
        item = heap_p[k];
        child = heap_p[child_ind];

        // right child is smaller?
        if (get_key(heap_p[child_ind + 1]) < get_key(child))
        {
            ++child_ind;
            child = heap_p[child_ind];
        }

        // is min heap property satisfied?
        if (get_key(item) <= get_key(child))
        {
            break;
        }

        heap_swap(k, child_ind, heap_p);
        k = child_ind;
    }
}


int * heap_min(int **heap_p, int heap_size)
{
    if (heap_size <= 0)
    {
        printf("Heap empty.\n");
        exit(1);
    }

    return heap_p[0];
}


void heap_resize(int ***heap_pp, int heap_size, int *arr_size)
{
    int **heap_p = *heap_pp;
    // double array size
    *arr_size = *arr_size * 2;
    int **new_heap = malloc(*arr_size * sizeof(int *));

    // copy old data over
    memcpy(new_heap, heap_p, heap_size * sizeof(int *));

    // in place resizing
    free(heap_p);
    *heap_pp = new_heap;
}


void heap_add(int *item, int ***heap_pp, int *heap_size, int *arr_size, int(*get_key)(int *))
{
    if (*heap_size == *arr_size)
    {
        // resize
        heap_resize(heap_pp, *heap_size, arr_size);
    }

    int **heap_p = *heap_pp;
    heap_p[*heap_size] = item;
    heap_sift_up(*heap_size, heap_p, get_key);
    ++*heap_size;
}


int * heap_delete(int k, int **heap_p, int *heap_size, int(*get_key)(int *))
{
    if (k < 0)
    {
        printf("Cannot delete item with negative index %d.\n", k);
        exit(1);
    }

    if (k >= *heap_size)
    {
        printf("Cannot delete item %d with heap size %d.\n", k, *heap_size);
        exit(1);
    }

    int *deleted = heap_p[k];
    --*heap_size;
    // swap the last heap item with kth item and sift down the new kth item to its correct position
    heap_swap(k, *heap_size, heap_p);
    heap_sift_down(k, heap_p, *heap_size, get_key);

    return deleted;
}


void heap_heapify(int **heap, int heap_size, int(*get_key)(int *))
{
    // heap[size / 2 + 1] onwards are leaves
    for (int i = heap_size / 2; i > 0; --i)
    {
        heap_sift_down(i, heap, heap_size, get_key);
    }
}


void pq_add(int *item, HashBackedPriorityQueue *pq_p)
{
    heap_add(item, &pq_p->heap, &pq_p->heap_size, &pq_p->arr_size, pq_p->get_priority);
    hashtable_insert(item, &pq_p->items);
}


int * pq_get(int *item, HashBackedPriorityQueue *pq_p, bool(*compar)(int *, int *))
{
    Node *node = hashtable_get(item, pq_p->items, compar);
    return node == NULL ? NULL : node->item;
}


bool pq_contains(int *item, HashBackedPriorityQueue *pq_p, bool(*compar)(int *, int *))
{
    return hashtable_contains(item, pq_p->items, compar);
}


int * pq_peek(HashBackedPriorityQueue *pq_p)
{
    return heap_min(pq_p->heap, pq_p->heap_size);
}


int * pq_poll(HashBackedPriorityQueue *pq_p)
{
    return heap_delete(0, pq_p->heap, &pq_p->heap_size, pq_p->get_priority);
}


int pq_size(HashBackedPriorityQueue *pq_p)
{
    return pq_p->heap_size;
}


void pq_update_priority(HashBackedPriorityQueue *pq_p)
{
    // can improve complexity from O(n) to O(lg n) if we store the item positions in hashtable
    heap_heapify(pq_p->heap, pq_p->heap_size, pq_p->get_priority);
}
