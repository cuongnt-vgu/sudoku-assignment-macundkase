#pragma once

#include "sudoku.h"

typedef struct NakedPairs_impl
{
    Cell **p_cells;
    int value1;
    int value2;
} NakedPairs;

typedef struct SolvedPairs_impl
{
    Cell *p_cell1;
    Cell *p_cell2;
} SolvedPairs;

int check_naked_pairs(Cell **p_cells, int a, int b);
int find_naked_pair_values(Cell **p_cells, int *probable_naked_pairs);
void find_naked_pair(Cell **p_cells, NakedPairs *p_naked_pairs,
                        int *p_counter);
int naked_pairs(SudokuBoard *p_board);
