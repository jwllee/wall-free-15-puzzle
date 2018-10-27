#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "puzzle.h"


unsigned long get_hash(int *record, int m)
{
    return hash(record, ADDITION, m);
}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Modo de uso: ./solver [test.txt] [output.txt]\n");
        printf("Donde:\n");
        printf("\t[test.txt] es el archivo de input\n");
        printf("\t[output.txt] es el nombre del archivo a escribir con el output\n");
    }

    FILE *file = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (!file || !output)
    {
        printf("Error in opening files!\n");
        return 2;
    }

    int size;
    fscanf(file, "%d", &size);

    int *start = malloc(size * size * sizeof(int));

    int ind;
    for (int i = 0; i < size * size; ++i)
    {
        ind = i * size;
        fscanf(file, " %d ", &start[ind]);

        for (int j = 1; j < size; ++j)
        {
            ind = i * size + j;
            fscanf(file, ",%d", &start[ind]);
        }
    }

    int *start_record = create_record(size, start);

    bool check_solution = false;
    int max, n_slots;
    double load;

    max = 20;
    n_slots = 11;
    load = 0.75;

    HashBackedPriorityQueue *queue = pq_init(max, n_slots, load, get_f_cost, get_hash);
    int *end_record = astar(start_record, size, queue);

    if (end_record == NULL)
    {
        printf("No solution found.\n");
    }

    int steps = get_g_cost(end_record);
    int **solution = get_solution(end_record, size, queue, record_state_equal);

    fprintf(output, "%d\n", steps);
    for (int i = 0; i < steps; ++i)
    {
        fprintf(output, "%d,%d\n", solution[i][0], solution[i][1]);
    }

    if (check_solution)
    {
        for (int i = 0; i < steps; ++i)
        {
            shift_tile(solution[i][0], solution[i][1], start_record, size);
        }

        if (!record_state_ordered(start_record))
        {
            printf("Solution is not valid!\n");
        }
        else{
            printf("Solution is valid.\n");
        }
    }

    // free memories
    fclose(file);
    fclose(output);
    free(start);

    for (int i = 0; i < steps; ++i)
    {
        free(solution[i]);
    }
    
    free(solution);
    pq_destroy(queue, true);

    return 0;
}
