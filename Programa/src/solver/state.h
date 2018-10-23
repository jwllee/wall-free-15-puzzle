//
// Created by jonathan on 23-10-18.
//

#pragma once

#include <stdbool.h>

// a state has
// [ size, s_1, s_2, ..., s_size, x, y, g_cost, h_cost, is_visited ]
// size: number of components that make up the state
// x, y: transformation from the empty tile to current state, direction is not needed since you can work this out by
//       looking at the position of the empty tile in current state
// g_cost: number of moves to transform the initial state to current state
// h_cost: heuristic number of moves to transform the current state to final state, needs to be a lower bound of actual
//         number of moves
// is_visited: if this state is in the closed or open set


int get_state_size(int *arr);
int get_x(int *arr);
int get_y(int *arr);
int get_g_cost(int *arr);
int get_h_cost(int *arr);
bool is_visited(int *arr);
