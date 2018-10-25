//
// Created by jonathan on 22-10-18.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


bool int_array_equal(int *a0, int *a1, int sz)
{
    bool equal = true;

    for (int i = 0; i < sz; ++i)
    {
        equal = equal && (a0[i] == a1[i]);
    }

    return equal;
}


void int_array_print(int *arr, int sz)
{
    printf("[");

    for (int i = 0; i < sz; ++i)
    {
        if (i > 0)
        {
            printf(", ");
        }

        printf("%d", arr[i]);
    }

    printf("]");
}


void int_array_println(int *arr, int sz)
{
    int_array_print(arr, sz);
    printf("\n");
}


int * index_to_xy(int index, int size)
{
    int *xy = malloc(2 * sizeof(int));
    xy[0] = index % size;
    xy[1] = index / size;
    return xy;
}


int xy_to_index(int x, int y, int size)
{
    return y * size + x;
}


int get_adj_num(int i, int dir, int size)
{
    int adj = i + dir;

    if (adj < 0)
    {
        adj = size - 1;
    }
    else if (adj >= size)
    {
        adj = 0;
    }

    return adj;
}


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
