//
// Created by jonathan on 10/26/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "utils.h"
#include "puzzle.h"
#include "queue.h"


unsigned long get_hash(int *record, int m)
{
    return hash(record, DJB2, m);
}


int main(int argc, char *argv[])
{
    const char *fp = "Programa/tests/lunatic/test5.txt";
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

    int *start_record = create_record(size, start);
    int max, n_slots;
    double load;

    max = 10;
    n_slots = 5;
    load = 0.75;

    HashBackedPriorityQueue *queue = pq_init(max, n_slots, load, get_f_cost, get_hash);

    printf("Start state:\n");
    record_print(start_record);
    board_print(start_record, size);
    printf("\n");

    int *end_record = astar(start_record, size, queue);

    if (end_record != NULL)
    {
        record_print(end_record);
        printf("End record ordered: %d\n", record_state_ordered(end_record));
        board_print(end_record, size);
        printf("\n");
    }
    else
    {
        printf("End record is NULL.\n");
    }

    printf("No. queued states: %d\n\n", pq_size(queue));

    int steps = get_g_cost(end_record);
    int **solution = get_solution(end_record, size, queue, record_state_equal);

    for (int i = 0; i < steps; ++i)
    {
        printf("\n");
        printf("Step %d: (%d, %d)\n", i, solution[i][0], solution[i][1]);
        board_print(start_record, size);
        shift_tile(solution[i][0], solution[i][1], start_record, size);
        printf("\n");
        board_print(start_record, size);
        printf("\n");
    }

    printf("Solution: %d steps\n", steps);
    for (int i = 0; i < steps; ++i)
    {
        printf("%d: ", i);
        int_array_println(solution[i], 2);
        free(solution[i]);
    }
    free(solution);
    free(start);
    pq_destroy(queue, true);

    return 0;
}