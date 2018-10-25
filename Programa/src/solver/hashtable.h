//
// Created by jonathan on 10/20/18.
//

#pragma once

#include <stdbool.h>

typedef struct node Node;
typedef struct hashtable HashTable;

// const int INIT_SIZE = 2 << 5;
// const double LOAD_FACTOR = 0.75;


typedef enum
{
    DJB2,
    SUM
} HashFunction;


struct node
{
    Node *prev;
    Node *next;
    int *item;
};


struct hashtable
{
    int max;
    int nb_slots;
    int size;
    double load_factor;
    HashFunction hash;
    // list of node pointers
    Node **nodes;
    Node *empty;
    int *slot_counter;
    int arr_sz;
};

const char * hash_func_to_str(HashFunction func);

Node * init_node(int *item);
HashTable * init_hashtable(int max, int nb_slots, double load, HashFunction hash, int arr_sz);

void node_destroy(Node *node, bool destroy_item);
void hashtable_destroy(HashTable *table, bool destroy_item);

unsigned long hash(int *item, HashFunction func, int m);
void hashtable_insert(int *item, HashTable *table);
Node * hashtable_get(unsigned long key, HashTable *table);
void hashtable_delete(int *item, HashTable *table);
void hashtable_print(HashTable *table);

void hashtable_assert(HashTable *table);
