//
// Created by jonathan on 23-10-18.
//

#include "state.h"


int get_state_size(int *arr)
{
    return arr[0];
}


int get_x(int *arr)
{
    int size = get_state_size(arr);
    return arr[size + 1];
}


int get_y(int *arr)
{
    int size = get_state_size(arr);
    return arr[size + 2];
}


int get_g_cost(int *arr)
{
    int size = get_state_size(arr);
    return arr[size + 3];
}


int get_h_cost(int *arr)
{
    int size = get_state_size(arr);
    return arr[size + 4];
}


bool is_visited(int *arr)
{
    int size = get_state_size(arr);
    return arr[size + 5] == 1 ? true : false;
}
