//
// Created by jonathan on 10/26/18.
//

#include "puzzle.h"
#include "state.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int * astar(int *start, int size, HashBackedPriorityQueue *queue)
{
    int *record = NULL;

    // 1. put start state into queue
    // 2. start while loop, end when queue is empty
    // while loop:
    // 3. get state with minimum cost using priority queue
    //    - if state is goal, break loop and return this state
    // 4. get its neighbors, for each neighbor
    //    - if neighbor is in closed set, do nothing
    //    - else if neighbor is in open set:
    //      - if neighbor is in queue, check and update g cost if neighbor g cost is lower
    //      - else add neighbor as new state in queue

    pq_add(start, queue);

    int **neighbors, *neighbor, *aux;

    while (pq_size(queue) > 0)
    {
        record = pq_poll(queue);

        // put into closed set
        set_visited(true, record);

        // check if record contains end state
        if (record_state_ordered(record))
        {
            break;
        }

        neighbors = get_neighbor_states(record, size);

        for (int i = 0; i < 4; ++i)
        {
            neighbor = neighbors[i];

            if (pq_contains(neighbor, queue, record_state_equal))
            {
                // get the contained one
                aux = pq_get(neighbor, queue, record_state_equal);

                // check if the state is not in the closed set
                // if it is not, update contained record's g cost if necessary
                if (!is_visited(aux))
                {
                    int g_cost0 = get_g_cost(aux);
                    int g_cost1 = get_g_cost(neighbor);

                    // check that h cost is the same since they have the same state
                    int h_cost0 = get_h_cost(aux);
                    int h_cost1 = get_h_cost(neighbor);

                    if (h_cost0 != h_cost1)
                    {
                        printf("h cost 0: %d != %d: h cost 1\n", h_cost0, h_cost1);
                        record_print(aux);
                        record_print(neighbor);
                        exit(1);
                    }

                    if (g_cost1 < g_cost0)
                    {
                        // printf("Updated g cost from %d to %d.\n", g_cost0, g_cost1);
                        set_g_cost(g_cost1, aux);
                        pq_update_priority(queue);
                    }
                }

                // neighbor is not used anymore
                free(neighbor);
            }
            // queue does not have this neighbor
            else
            {
                pq_add(neighbor, queue);
            }
        }

        free(neighbors);
    }

    return record;
}
