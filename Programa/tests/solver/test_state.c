//
// Created by jonathan on 23-10-18.
//

#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    const char *fp = "Programa/tests/4x4/test0.txt";
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
    printf("\nStart state:\n");
    board_print(record, size);

    int **neighbors = get_neighbor_states(record, size);
    int *parent_state;

    for (int i = 0; i < 4; ++i)
    {
        printf("\nNeighbor %d\n", i);
        // board_print(neighbors[i], size);
        record_print(neighbors[i]);
        assert_is_neighbor(record, neighbors[i], size);

        // printf("Parent state:\n");
        parent_state = get_parent_state(neighbors[i], size);
        // state_print(parent_state, size);

        if (int_array_equal(parent_state, get_state(record), get_state_size(record)))
        {
            printf("Correct parent state.\n");
        }
        else
        {
            printf("Wrong parent state.\n");
        }

        free(parent_state);
    }

    int *ordered_state = malloc(size * size * sizeof(int));
    for (int i = 0; i < size * size; ++i)
    {
        if (i < size * size - 1)
        {
            ordered_state[i] = i + 1;
        }
        else
        {
            ordered_state[i] = 0;
        }
    }

    printf("Ordered state: ");
    int_array_println(ordered_state, size * size);

    printf("\nBefore setting ordered state:\n");
    record_print(record);
    printf("Record state ordered: %d\n", record_state_ordered(record));

    set_state(ordered_state, record);
    printf("\nAfter setting ordered state:\n");
    record_print(record);
    printf("Record state ordered: %d\n", record_state_ordered(record));

    // free memories
    fclose(file);
    free(start);
    free(ordered_state);
    free(record);
    for (int i = 0; i < 4; ++i)
    {
        free(neighbors[i]);
    }
    free(neighbors);

    return 0;
}

