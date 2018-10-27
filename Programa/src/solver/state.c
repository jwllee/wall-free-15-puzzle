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


void set_state(int *state, int *record)
{
    int size = get_state_size(record);
    int *cur_state = get_state(record);

    for (int i = 0; i < size; ++i)
    {
        cur_state[i] = state[i];
    }
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


int * get_state(int *record)
{
    return &record[1];
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


int get_f_cost(int *record)
{
    return get_g_cost(record) + get_h_cost(record);
}


bool is_visited(int *record)
{
    int size = get_state_size(record);
    return record[size + 5] == 1 ? true : false;
}


int * record_get_parent(int *record, int size)
{
    int x, y, index, zero_index, *state;
    state = get_state(record);
    x = get_x(record);
    y = get_y(record);
    index = xy_to_index(x, y, size);
    zero_index = get_empty_space_index(record);

    int *parent_record = record_copy(record);

    // swap index and zero index value
    parent_record[zero_index] = state[index];
    parent_record[index + 1] = 0;

    return parent_record;
}


bool record_state_equal(int *r0, int *r1)
{
    int s0 = get_state_size(r0);
    int s1 = get_state_size(r1);

    if (s0 != s1)
    {
        printf("State size %d != %d.\n", s0, s1);
        exit(1);
    }

    bool equal = true;

    for (int i = 0; i < s0; ++i)
    {
        equal = equal && (r0[i + 1] == r1[i + 1]);
    }

    return equal;
}


unsigned long hash(int *record, Hash hash, int m)
{
    unsigned long hcode;
    int s = get_state_size(record);

    switch (hash)
    {
        case DJB2:
            hcode = djb2(&record[1], s, m);
            break;
        case SUM:
            hcode = sum(&record[1], s, m);
            break;
        default:
            printf("Do not recognize hash function %s.\n", hash_func_to_str(hash));
            exit(1);
    }

    return hcode;
}


int get_empty_space_index(int *record)
{
    int size = record[0], index = -1;

    for (int i = 0; i < size; ++i)
    {
        index = record[i + 1] == 0 ? i + 1 : index;
    }

    if (index < 0)
    {
        printf("Cannot find index of empty space.\n");
        exit(1);
    }

    return index;
}


void move_tile(int from, int to, int *record)
{
    if (record[to] != 0)
    {
        printf("Shifted tile destination %d is not empty.\n", record[to]);
        exit(1);
    }

    record[to] = record[from];
    record[from] = 0;
}


int * record_copy(int *record)
{
    int state_size = record[0];
    int total_size = state_size + 6;
    int *copy = malloc(total_size * sizeof(int));
    memcpy(copy, record, total_size * sizeof(int));
    return copy;
}


bool record_state_ordered(int *record)
{
    int size = get_state_size(record);
    int *state = get_state(record);
    bool ordered = true;

    for (int i = 0; i < size; ++i)
    {
        if (i < size - 1)
        {
            ordered = ordered && state[i] == i + 1;
        }
        else
        {
            // last tile is empty
            ordered = ordered && state[i] == 0;
        }
    }

    return ordered;
}


int * get_neighbor(int *record, int size, int *empty_xy, Direction dir)
{
    int *from_xy, from_ind, empty_ind, aux;
    int *neighbor;

    empty_ind = xy_to_index(empty_xy[0], empty_xy[1], size);
    from_xy = malloc(2 * sizeof(int));

    switch (dir)
    {
        case TOP:
            aux = get_adj_num(empty_xy[1], -1, size);
            from_xy[0] = empty_xy[0];
            from_xy[1] = aux;
            break;
        case BOTTOM:
            aux = get_adj_num(empty_xy[1], 1, size);
            from_xy[0] = empty_xy[0];
            from_xy[1] = aux;
            break;
        case LEFT:
            aux = get_adj_num(empty_xy[0], -1, size);
            from_xy[0] = aux;
            from_xy[1] = empty_xy[1];
            break;
        case RIGHT:
            aux = get_adj_num(empty_xy[0], 1, size);
            from_xy[0] = aux;
            from_xy[1] = empty_xy[1];
            break;
        default:
            printf("Do not recognize direction enum: %d.\n", dir);
            exit(1);
    }

    // int_array_println(from_xy, 2);

    from_ind = xy_to_index(from_xy[0], from_xy[1], size);
    neighbor = record_copy(record);

    // need to set the neighbor record information
    // get the previous empty tile position so you can work out previous state
    int x = empty_xy[0];
    int y = empty_xy[1];
    set_x(x, neighbor);
    set_y(y, neighbor);

    move_tile(from_ind, empty_ind, &neighbor[1]);

    // update costs
    int g_cost = get_g_cost(record) + 1;
    int h_cost = compute_h_cost(size, neighbor);
    set_g_cost(g_cost, neighbor);
    set_h_cost(h_cost, neighbor);

    free(from_xy);

    return neighbor;
}


int ** get_neighbor_states(int *record, int size)
{
    int ** neighbors = malloc(4 * sizeof(int *));
    int empty_ind = get_empty_space_index(record) - 1;
    int *empty_xy = index_to_xy(empty_ind, size);

    // printf("Empty index: %d\n", empty_ind);
    // printf("Empty xy: ");
    // int_array_println(empty_xy, 2);

    for (int i = 0; i < 4; ++i)
    {
        neighbors[i] = get_neighbor(record, size, empty_xy, TOP + i);
    }

    free(empty_xy);

    return neighbors;
}


void record_print(int *record)
{
    printf("size: %d, state: ", get_state_size(record));
    int_array_print(&record[1], get_state_size(record));
    printf(", x: %d, y: %d, g: %d, h: %d, visited: %d\n",
            get_x(record),
            get_y(record),
            get_g_cost(record),
            get_h_cost(record),
            is_visited(record));
}


void board_print(int *record, int size)
{
    int index, val;

    for (int i = 0; i < size; ++i)
    {
        index = i * size + 1;
        val = record[index];
        printf("%2d", val);

        for (int j = 1; j < size; ++j)
        {
            ++index;
            val = record[index];
            printf(" %2d", val);
        }

        printf("\n");
    }
}


void state_print(int *state, int size)
{
    int index, val;

    for (int i = 0; i < size; ++i)
    {
        index = i * size;
        val = state[index];
        printf("%2d", val);

        for (int j = 1; j < size; ++j)
        {
            ++index;
            val = state[index];
            printf(" %2d", val);
        }

        printf("\n");
    }
}


void assert_is_neighbor(int *r0, int *r1, int size)
{
    if (r0[0] != r1[0])
    {
        printf("Array lengths %d != %d.\n", r0[0], r1[0]);
        exit(1);
    }

    int size0 = get_state_size(r0);
    int size1 = get_state_size(r1);
    if (size0 != size1)
    {
        printf("State size %d != %d.\n", size0, size1);
        exit(1);
    }

    int diff = 0;
    for (int i = 0; i < size0; ++i)
    {
        if (r0[i + 1] != r1[i + 1])
        {
            ++diff;
        }
    }

    if (diff != 2)
    {
        printf("There are %d differences. Should be exactly 2.\n", diff);
        printf("From: \n");
        board_print(r0, size);
        printf("\nTo: \n");
        board_print(r1, size);
        exit(1);
    }

    int empty_ind_0 = get_empty_space_index(r0) - 1;
    int empty_ind_1 = get_empty_space_index(r1) - 1;
    int *empty_xy_0 = index_to_xy(empty_ind_0, size);
    int *empty_xy_1 = index_to_xy(empty_ind_1, size);

    int x = get_x(r1);
    int y = get_y(r1);

    int tile_ind_0 = xy_to_index(empty_xy_1[0], empty_xy_1[1], size);
    int tile_ind_1 = xy_to_index(empty_xy_0[0], empty_xy_0[1], size);

    int tile_0 = r0[tile_ind_0 + 1];
    int tile_1 = r1[tile_ind_1 + 1];

    if (x != empty_xy_0[0] || y != empty_xy_0[1])
    {
        printf("Previous tile position x: %d, y: %d "
               "is wrong in neighbor x: %d, y: %d.\n", empty_xy_0[0], empty_xy_0[1], x, y);
        printf("From: \n");
        board_print(r0, size);
        printf("\nTo: \n");
        board_print(r1, size);
        exit(1);
    }

    if (tile_0 != tile_1)
    {
        printf("Moved tile value should match: %d != %d.\n", tile_0, tile_1);
        printf("From: \n");
        board_print(r0, size);
        printf("\nTo: \n");
        board_print(r1, size);
    }

    int g0 = get_g_cost(r0);
    int g1 = get_g_cost(r1);

    if (g1 != g0 + 1)
    {
        printf("Neighbor g cost: %d != %d + 1.\n", g1, g0);
        exit(1);
    }

    free(empty_xy_0);
    free(empty_xy_1);
}