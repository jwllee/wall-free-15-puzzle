//
// Created by jonathan on 10/20/18.
//

#pragma once

#include <stdbool.h>

typedef struct node Node;
typedef struct hashtable HashTable;

const int INIT_SIZE = 2 << 5;
const double LOAD_FACTOR = 0.75;


typedef enum
{
    DJB2
} HashFunction;


const char * hash_func_to_str(HashFunction func)
{
    static const char *strings[] = { "djb2" };
    return strings[func];
}


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
    int *collision_counter;
};


Node * init_node(int *item);
HashTable * init_hashtable(int max, int nb_slots, double load, HashFunction hash);

void node_destroy(Node *node);
void hashtable_destroy(HashTable *table);

unsigned long hash(int *item, HashFunction func);
void hashtable_insert(int *item, HashTable *table);
int * hashtable_get(int *item, HashTable *table);
void hashtable_delete(int *item, HashTable *table);
HashTable * hashtable_reha
