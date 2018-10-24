//
// Created by jonathan on 23-10-18.
//

#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    const char *fp = "Programa/tests/3x3/test0.txt";
    FILE *file = fopen(fp, "r");

    int size;
    fscanf(file, "%d", &size);

    int *start = malloc(size * size * sizeof(int));

    int ind;
    for (int i = 0; i < size; ++i)
    {
        ind = i * size;
        fscanf(file, "%d", &start[ind]);

        for (int j = 1; j < size; ++j)
        {
            ind = i * size + j;
            fscanf(file, ",%d", &start[ind]);
        }
    }

    int_array_println(start, size * size);

    int *record = create_record(size, start);

    record_print(record);

    // free memories
    fclose(file);
    free(start);

    return 0;
}

