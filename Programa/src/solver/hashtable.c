//
// Created by jonathan on 10/20/18.
//

#include "hashtable.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


Node * init_node(int *item)
{
    Node *node = malloc(sizeof(Node));
    node->prev = NULL;
    node->next = NULL;
    node->item = item;
    return node;
}


HashTable * init_hashtable(int max, int n_slots, double load, unsigned long(*hash)(int *, int))
{
    HashTable *table_p = malloc(sizeof(HashTable));
    table_p->n_slots = n_slots;
    table_p->max = max;
    table_p->size = 0;
    table_p->n_rehash = 0;
    table_p->load_factor = load;
    table_p->hash = hash;
    table_p->nodes = malloc(n_slots * sizeof(Node *));
    table_p->slot_counter = malloc(n_slots * sizeof(int));
    table_p->empty = init_node(NULL);

    for (int i = 0; i < n_slots; ++i)
    {
        table_p->nodes[i] = NULL;
        table_p->slot_counter[i] = 0;

    }

    Node *cur = table_p->empty;
    int limit = (int) (max * load);
    for (int i = 0; i < limit; ++i)
    {
        cur->next = init_node(NULL);
        cur = cur->next;
    }

    return table_p;
}


void node_destroy(Node *node, bool destroy_item)
{
    if (node->item != NULL && destroy_item)
        free(node->item);
    free(node);
}


void hashtable_destroy(HashTable *table_p, bool destroy_item)
{
    Node *node, *tmp;
    for (int i = 0; i < table_p->n_slots; ++i)
    {
        node = table_p->nodes[i];
        while (node != NULL)
        {
            tmp = node;
            node = node->next;

            node_destroy(tmp, destroy_item);
        }
    }

    int empty_slots = (int) (table_p->max * table_p->load_factor) - table_p->size + 1;
    while (empty_slots-- > 0)
    {
        tmp = table_p->empty;
        table_p->empty = tmp->next;

        node_destroy(tmp, destroy_item);
    }

    free(table_p->nodes);
    free(table_p->slot_counter);
    free(table_p);
}


void hashtable_rehash(HashTable **table_pp)
{
    HashTable *table_p = *table_pp;
    int new_max = table_p->max << 1;
    int new_n_slots = table_p->n_slots << 1;

    HashTable *old_table = table_p;

    // point to new table
    *table_pp = init_hashtable(new_max, new_n_slots, table_p->load_factor, table_p->hash);
    (*table_pp)->n_rehash = old_table->n_rehash + 1;

    Node *node;
    for (int i = 0; i < old_table->n_slots; ++i)
    {
        node = old_table->nodes[i];
        while (node != NULL)
        {
            hashtable_insert(node->item, table_pp);
            node->item = NULL;
            node = node->next;
        }
    }

    hashtable_destroy(old_table, false);
}


void hashtable_insert(int *item, HashTable **table_pp)
{
    HashTable *table_p = *table_pp;
    int limit = (int) (table_p->max * table_p->load_factor);
    if (table_p->size >= limit)
    {
        hashtable_rehash(table_pp);
        table_p = *table_pp;
    }

    unsigned long hcode = table_p->hash(item, table_p->n_slots);
    // unsigned long index = hcode % table->n_slots;

    Node *new_node = table_p->empty, *node;

    node = table_p->nodes[hcode];
    table_p->empty = table_p->empty->next;

    table_p->nodes[hcode] = new_node;
    new_node->item = item;
    new_node->next = node;

    if (node != NULL)
    {
        node->prev = new_node;
    }

    ++table_p->size;
    ++table_p->slot_counter[hcode];
}


bool hashtable_contains(int *item, HashTable *table_p, bool(*compar)(int *, int *))
{
    unsigned long index = table_p->hash(item, table_p->n_slots);

    Node *node = table_p->nodes[index];
    bool contains = false;

    // loop invariant:
    // - have not found an item equal to item
    // - node contains something and is not NULL
    // At termination:
    // We either have found the item or we have looked through all items in the linked list.
    while (!contains && node != NULL)
    {
        contains = compar(item, node->item);
        node = node->next;
    }

    return contains;
}


Node * hashtable_get(int *item, HashTable *table_p, bool(*compar)(int *, int *))
{
    unsigned long index = table_p->hash(item, table_p->n_slots);

    Node *node = table_p->nodes[index];
    bool equal;

    while (node != NULL)
    {
        equal = compar(item, node->item);

        if (equal)
        {
            break;
        }

        node = node->next;
    }

    return node;
}


void hashtable_delete(int *item, HashTable *table_p, bool(*compar)(int *, int *))
{
    unsigned long hcode = table_p->hash(item, table_p->n_slots);
    Node *node = hashtable_get(item, table_p, compar);

    if (node == NULL)
    {
        printf("Item not in hashtable.\n");
        exit(1);
    }

    // first node in linked list
    if (node->prev == NULL)
    {
        table_p->nodes[hcode] = node->next;
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
    // add node as empty node back at table's empty nodes
    node->prev = NULL;
    node->next = table_p->empty;
    table_p->empty->prev = node;
    table_p->empty = node;

    --table_p->size;
    --table_p->slot_counter[hcode];
}


void hashtable_print(HashTable *table_p)
{
    unsigned long hcode;
    Node *node;

    for (int i = 0; i < table_p->n_slots; ++i)
    {
        node = table_p->nodes[i];

        printf("[%i]: ", i);

        int cnt = 0;
        while (node != NULL)
        {
           if (cnt > 0)
           {
              printf(" -> ");
           }

           hcode = table_p->hash(node->item, -1);
           printf("%ld", hcode);

           node = node->next;
           ++cnt;
        }

        printf("\n");
    }
}


void hashtable_assert(HashTable *table_p)
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

    for (int i = 0; i < table_p->n_slots; ++i)
    {
        node = table_p->nodes[i];
        slot_cnt = 0;
        slot_i = table_p->slot_counter[i];

        while (node != NULL)
        {
            item = node->item;
            hcode = table_p->hash(item, table_p->n_slots);

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

    if (cnt != table_p->size)
    {
        printf("Hashtable size: %d != number of items: %d.\n", table_p->size, cnt);
        exit(1);
    }

    double cur_load = table_p->size * 1.0 / table_p->max;

    if (cur_load > table_p->load_factor)
    {
        printf("Hashtable current load %.2f exceeds load factor %.2f.\n", cur_load, table_p->load_factor);
    }
}
