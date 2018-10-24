//
// Created by jonathan on 10/20/18.
//

#include "hashtable.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


unsigned long sum(int *arr, int sz, int m)
{
    unsigned long hash = 0;

    for (int i = 0; i < sz; ++i)
    {
        hash = (hash + arr[i]);

        if (m > 0)
        {
            hash = hash % m;
        }
    }

    return hash;
}


unsigned long djb2(int *arr, int sz, int m)
{
    unsigned long hash = 5381;

    for (int i = 0; i < sz; ++i)
    {
        // hash * 33 + arr[i]
        hash = ((hash << 5) + hash) + arr[i];

        if (m > 0)
        {
            hash = hash % m;
        }
    }

    return hash;
}


const char * hash_func_to_str(HashFunction func)
{
    static const char *strings[] = { "djb2", "sum" };
    return strings[func];
}


Node * init_node(int *item)
{
    Node *node = malloc(sizeof(Node));
    node->prev = NULL;
    node->next = NULL;
    node->item = item;
    return node;
}


HashTable * init_hashtable(int max, int nb_slots, double load, HashFunction hash, int arr_sz)
{
    HashTable *table = malloc(sizeof(HashTable));
    table->nb_slots = nb_slots;
    table->max = max;
    table->size = 0;
    table->load_factor = load;
    table->hash = hash;
    table->nodes = malloc(nb_slots * sizeof(Node *));
    table->slot_counter = malloc(nb_slots * sizeof(int));
    table->empty = init_node(NULL);
    table->arr_sz = arr_sz;

    for (int i = 0; i < nb_slots; ++i)
    {
        table->nodes[i] = NULL;
        table->slot_counter[i] = 0;

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
    free(table->slot_counter);
    free(table);
}


void hashtable_rehash(HashTable *table)
{
    int new_max = table->max << 2;
    int new_nb_slots = table->nb_slots << 2;

    HashTable *old_table = table;

    // point to new table
    table = init_hashtable(new_max, new_nb_slots, table->load_factor, table->hash, table->arr_sz);

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


unsigned long hash(int *item, HashFunction func, int m)
{
    unsigned long hash;
    switch (func)
    {
        case DJB2:
            // the number of item's elements to be used for computing the hashcode
            // is stored at the first index
            hash = djb2(&item[1], item[0], m);
            break;
        case SUM:
            hash = sum(&item[1], item[0], m);
            break;
        default:
            printf("Do not recognize hash function %s.\n", hash_func_to_str(func));
            exit(1);
    }

    return hash;
}


void hashtable_insert(int *item, HashTable *table)
{
    unsigned long hcode = hash(item, table->hash, table->nb_slots);
    // unsigned long index = hcode % table->nb_slots;

    Node *new_node = table->empty, *node;

    node = table->nodes[hcode];
    table->empty = table->empty->next;

    table->nodes[hcode] = new_node;
    new_node->item = item;
    new_node->next = node;

    if (node != NULL)
    {
        node->prev = new_node;
    }

    ++table->size;
    ++table->slot_counter[hcode];

    int limit = (int) (table->max * table->load_factor);
    if (table->size >= limit)
    {
        hashtable_rehash(table);
    }
}


Node * hashtable_get(unsigned long key, HashTable *table)
{
    unsigned long index = key % table->nb_slots;

    Node *node = table->nodes[index];
    return node;
}


void hashtable_delete(int *item, HashTable *table)
{
    unsigned long hcode = hash(item, table->hash, table->nb_slots);
    Node *node = hashtable_get(hcode, table);

    bool equal;

    int index = 0;
    while (node != NULL)
    {
        equal = int_array_equal(item, node->item, table->arr_sz);

        if (equal)
        {
            // unchain node and put to empty
            if (index == 0)
            {
                table->nodes[hcode] = node->next;
            }
            else
            {
                node->prev->next = node->next;

                if (node->next != NULL)
                {
                    node->next->prev = node->prev;
                }
            }

            // remove item from node
            node->item = NULL;
            // add empty node back to table's empty nodes
            node->prev = NULL;
            node->next = table->empty;
            table->empty->prev = node;
            table->empty = node;
            --table->size;
            --table->slot_counter[hcode];
            break;
        }
        else
        {
            node = node->next;
            ++index;
        }
    }
}


void hashtable_print(HashTable *table)
{
    unsigned long hcode;
    Node *node;

    for (int i = 0; i < table->nb_slots; ++i)
    {
        node = table->nodes[i];

        printf("[%i]: ", i);

        int cnt = 0;
        while (node != NULL)
        {
           if (cnt > 0)
           {
              printf(" -> ");
           }

           hcode = hash(node->item, table->hash, -1);
           printf("%ld", hcode);

           node = node->next;
           ++cnt;
        }

        printf("\n");
    }
}


void hashtable_assert(HashTable *table)
{
    // To check:
    // - size
    // - hashcodes
    // - under load factor
    // - slot counter

    int cnt = 0, slot_cnt, slot_i;
    Node *node;
    int *item;
    unsigned long hcode;

    for (int i = 0; i < table->nb_slots; ++i)
    {
        node = table->nodes[i];
        slot_cnt = 0;
        slot_i = table->slot_counter[i];

        while (node != NULL)
        {
            item = node->item;
            hcode = hash(item, table->hash, table->nb_slots);

            if (hcode != i)
            {
                printf("Hashcode %ld does not match slot index %d.\n", hcode, i);
                exit(1);
            }

            node = node->next;
            ++cnt;
            ++slot_cnt;
        }

        if (slot_cnt != slot_i)
        {
            printf("Slot counter at slot %d: %d != %d (expected).\n", i, slot_i, slot_cnt);
            exit(1);
        }
    }

    if (cnt != table->size)
    {
        printf("Hashtable size: %d != number of items: %d.\n", table->size, cnt);
        exit(1);
    }

    double cur_load = table->size * 1.0 / table->max;

    if (cur_load > table->load_factor)
    {
        printf("Hashtable current load %.2f exceeds load factor %.2f.\n", cur_load, table->load_factor);
    }
}
