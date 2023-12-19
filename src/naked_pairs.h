#pragma once

#include "sudoku.h"

typedef struct NakedPairs_impl
{
    Cell **p_cells;
    int value1;
    int value2;
} NakedPairs;

int check_naked_pairs(Cell **p_cells, int a, int b, int check_box);
int find_naked_pair_values(Cell **p_cells, int *probable_naked_pairs);
void find_naked_pair(Cell **p_cells, NakedPairs *p_naked_pairs,
                        int *p_counter, int check_box);
int naked_pairs(SudokuBoard *p_board);
