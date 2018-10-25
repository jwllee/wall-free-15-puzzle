//
// Created by jonathan on 10/20/18.
//


#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "hashtable.h"


bool is_equal(int *a0, int *a1)
{
    return int_array_equal(a0, a1, 2);
}


unsigned long hash(int *arr, int m)
{
    return sum(&arr[1], 1, m);
}


int main(int argc, char *argv[])
{
    printf("Testing hashtable...\n");

    printf("Read in data...\n");

    const char *fp = "Programa/tests/data/hash0.txt";

    FILE *file = fopen(fp, "r");

    if (!file)
    {
        printf("Error opening the file!\n");
        return 2;
    }

    int m, n_data;
    fscanf(file, "%d %d", &m, &n_data);

    printf("Hashtable size: %d\nNo. of data: %d\n", m, n_data);

    int **data = malloc(n_data * sizeof(int *));

    int n_datapoint, arr_sz = 0, *arr;
    for (int i = 0; i < n_data; ++i)
    {
        fscanf(file, " %d ", &n_datapoint);
        arr_sz = n_datapoint + 1;
        arr = malloc(arr_sz * sizeof(int));

        arr[0] = n_datapoint;
        // printf("Array size: %d\n", n_datapoint);

        for (int j = 0; j < n_datapoint; ++j)
        {
            fscanf(file, " %d ", &arr[j + 1]);
        }

        data[i] = arr;
        printf("Read in: ");
        int_array_println(arr, arr_sz);
    }

    HashTable *table = init_hashtable(100, m, 1.0, is_equal, hash);

    printf("\nEmpty hashtable:\n");
    hashtable_print(table);

    int *item;
    Node *node;
    for (int i = 0; i < n_data; ++i)
    {
        item = data[i];
        hashtable_insert(item, table);
        hashtable_assert(table);
    }

    printf("\nFilled hashtable:\n");
    hashtable_print(table);

    // delete 10
    hashtable_delete(data[1], table);
    printf("\nDelete 10\n");
    hashtable_print(table);
    free(data[1]);

    // delete 3
    hashtable_delete(data[4], table);
    printf("\nDelete 3\n");
    hashtable_print(table);
    free(data[4]);

    hashtable_assert(table);

    // free memory
    hashtable_destroy(table, true);
    free(data);
    fclose(file);

    return 0;
}
