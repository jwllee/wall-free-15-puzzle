//
// Created by jonathan on 10/26/18.
//

#pragma once

#include "queue.h"


int * astar(int *start, int size, HashBackedPriorityQueue *queue);
int ** get_solution(int *end_record, int size, HashBackedPriorityQueue *queue, bool(*compar)(int *, int *));