//
// Created by jonathan on 10/20/18.
//

#pragma once

#include <stdbool.h>
#include "utils.h"

typedef struct node Node;
typedef struct hashtable HashTable;

// const int INIT_SIZE = 2 << 5;
// const double LOAD_FACTOR = 0.75;


struct node
{
    Node *prev;
    Node *next;
    int *item;
};


struct hashtable
{
    int max;
    int n_slots;
    int size;
    double load_factor;
    // list of node pointers
    Node **nodes;
    Node *empty;
    int *slot_counter;

    bool(*compar)(int *, int *);
    unsigned long(*hash)(int *, int);
};


Node * init_node(int *item);
HashTable * init_hashtable(int max, int n_slots, double load,
        bool(*compar)(int *, int *), unsigned long(*hash)(int *, int));

void node_destroy(Node *node, bool destroy_item);
void hashtable_destroy(HashTable *table, bool destroy_item);

void hashtable_insert(int *item, HashTable *table);
bool hashtable_contains(int *item, HashTable *table);
Node * hashtable_get(unsigned long key, HashTable *table);
void hashtable_delete(int *item, HashTable *table);
void hashtable_print(HashTable *table);

void hashtable_assert(HashTable *table);
