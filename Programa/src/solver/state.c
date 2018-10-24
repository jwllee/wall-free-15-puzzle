//
// Created by jonathan on 23-10-18.
//

#include "state.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

#define max(x, y) ((x) >= (y)) ? (x) : (y)


int required_moves(int pos, int val, int size)
{
    if (val <= 0)
    {
        return 0;
    }

    int cur_row, cur_col, moves = 0, req_row, req_col;

    cur_row = pos / size;
    cur_col = pos % size;

    // empty tile has value 0 and is at the last row and column
    req_row = val > 0 ? (val - 1) / size : size - 1;
    req_col = val > 0 ? (val - 1) % size : size - 1;

    int move_row, move_col;

    move_row = abs(req_row - cur_row);
    move_col = abs(req_col - cur_col);

    // can move through walls, so moving to other side is just 1 move
    move_row = move_row == size - 1 ? 1 : move_row;
    move_col = move_col == size - 1 ? 1 : move_col;

    moves = move_row + move_col;

    return moves;
}


/**
 * Heuristic cost is the sum of distance from required position of all tiles.
 * The empty tile (0) does not count as a tile.
 *
 * This cost function is a lower bound because in order to move each tile
 * to its correct place, the tile needs to be moved at least the direct
 * distance from current to required position.
 *
 * @param size: square board width
 * @param record: state record
 * @return: heuristic cost
 */
int compute_h_cost(int size, int *record)
{
    int cost = 0;

    int req_moves, val, state_size = size * size;
    for (int i = 0; i < state_size; ++i)
    {
        val = record[i + 1];
        req_moves = required_moves(i, val, size);
        cost += req_moves;
    }

    return cost;
}


int * create_record(int size, int *state)
{
    int state_size = size * size;
    int *record = malloc((state_size + 6) * sizeof(int));

    record[0] = state_size;
    memcpy(&record[1], state, state_size * sizeof(int));
    set_x(-1, record);
    set_y(-1, record);
    set_g_cost(0, record);
    set_h_cost(compute_h_cost(size, record), record);
    set_visited(false, record);

    return record;
}


void set_x(int x, int *record)
{
    int size = get_state_size(record);
    record[size + 1] = x;
}


void set_y(int y, int *record)
{
    int size = get_state_size(record);
    record[size + 2] = y;
}


void set_g_cost(int g, int *record)
{
    int size = get_state_size(record);
    record[size + 3] = g;
}


void set_h_cost(int h, int *record)
{
    int size = get_state_size(record);
    record[size + 4] = h;
}


void set_visited(bool visited, int *record)
{
    int size = get_state_size(record);
    record[size + 5] = visited ? 1: 0;
}


int get_state_size(int *record)
{
    return record[0];
}


int get_x(int *record)
{
    int size = get_state_size(record);
    return record[size + 1];
}


int get_y(int *record)
{
    int size = get_state_size(record);
    return record[size + 2];
}


int get_g_cost(int *record)
{
    int size = get_state_size(record);
    return record[size + 3];
}


int get_h_cost(int *record)
{
    int size = get_state_size(record);
    return record[size + 4];
}


bool is_visited(int *record)
{
    int size = get_state_size(record);
    return record[size + 5] == 1 ? true : false;
}


void record_print(int *record)
{
    printf("size: %d, state: ", get_state_size(record));
    int_array_print(&record[1], get_state_size(record));
    printf(", x: %d, y: %d, g: %d, h: %d\n",
            get_x(record),
            get_y(record),
            get_g_cost(record),
            get_h_cost(record));
}
