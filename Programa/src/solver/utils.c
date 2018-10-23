//
// Created by jonathan on 22-10-18.
//

#include "utils.h"
#include <stdio.h>


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
    for (int i = 0; i < sz; ++i)
    {
        if (i > 0)
        {
            printf(", ");
        }

        printf("%d", arr[i]);
    }

    printf("\n");
}
