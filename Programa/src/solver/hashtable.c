//
// Created by jonathan on 10/20/18.
//

#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


unsigned long djb2(int *arr, int sz)
{
    unsigned long hash = 5381;

    for (int i = 0; i < sz; ++i)
    {
        // hash * 33 + arr[i]
        hash = ((hash << 5) + hash) + arr[i];
    }

    return hash;
}


Node * init_node(int *item)
{
    Node *node = malloc(sizeof(Node));
    node->prev = NULL;
    node->next = NULL;
    node->item = item;
    return node;
}


HashTable * init_hashtable(int max, int nb_slots, double load, HashFunction hash)
{
    HashTable *table = malloc(sizeof(HashTable));
    table->nb_slots = nb_slots;
    table->size = 0;
    table->load_factor = load;
    table->hash = hash;
    table->nodes = malloc(nb_slots * sizeof(Node *));
    table->collision_counter = malloc(nb_slots * sizeof(int));
    table->empty = init_node(NULL);

    for (int i = 0; i < nb_slots; ++i)
    {
        table->nodes[i] = NULL;
        table->collision_counter[i] = -1;

    }

    Node *cur = table->empty;
    int limit = (int) (max * load);
    for (int i = 0; i < limit; ++i)
    {
        cur->next = init_node(NULL);
        cur = cur->next;
    }

    return table;
}


void node_destroy(Node *node)
{
    if (node->item != NULL)
        free(node->item);
    free(node);
}


void hashtable_destroy(HashTable *table)
{
    Node *node, *tmp;
    for (int i = 0; i < table->nb_slots; ++i)
    {
        node = table->nodes[i];
        while (node != NULL)
        {
            tmp = node;
            node = node->next;

            node_destroy(tmp);
        }
    }

    int empty_slots = (int) (table->max * table->load_factor) - table->size + 1;
    while (empty_slots-- > 0)
    {
        tmp = table->empty;
        table->empty = tmp->next;

        node_destroy(tmp);
    }

    free(table->nodes);
    free(table->collision_counter);
    free(table);
}


void hashtable_rehash(HashTable *table)
{
    int new_max = table->max << 2;
    int new_nb_slots = table->nb_slots << 2;

    HashTable *old_table = table;

    // point to new table
    table = init_hashtable(new_max, new_nb_slots, table->load_factor, table->hash);

    Node *node;
    for (int i = 0; i < old_table->nb_slots; ++i)
    {
        node = old_table->nodes[i];
        while (node != NULL)
        {
            hashtable_insert(node->item, table);
            node->item = NULL;
            node = node->next;
        }
    }

    hashtable_destroy(old_table);
}


unsigned long hash(int *item, HashFunction func)
{
    unsigned long hash;
    switch (func)
    {
        case DJB2:
            // the number of item's elements to be used for computing the hashcode
            // is stored at the first index
            hash = djb2(&item[1], item[0]);
            break;
        default:
            printf("Do not recognize hash function %s.\n", hash_func_to_str(func));
            exit(1);
    }

    return hash;
}


void hashtable_insert(int *item, HashTable *table)
{
    unsigned long hcode = hash(item, table->hash);
    unsigned long index = hcode % table->nb_slots;

    Node *new_node = table->empty, *node;

    node = table->nodes[index];
    table->empty = new_node->next;
    new_node->item = item;
    new_node->next = node;

    if (node != NULL)
    {
        node->prev = new_node;
    }

    ++table->size;

    int limit = (int) (table->max * table->load_factor);
    if (table->size >= limit)
    {
        hashtable_rehash(table);
    }
}


int * hashtable_get(int *item, HashTable *table)
{

}


void hashtable_delete(int *item, HashTable *table)
{

}

