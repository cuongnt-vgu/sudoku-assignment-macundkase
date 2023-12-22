#pragma once

#include "sudoku.h"

typedef struct HiddenPairs_impl
{
    Cell *p_cell_1;
    Cell *p_cell_2;
    int value1;
    int value2;
} HiddenPairs;

int check_hidden_pairs(Cell **p_cells, int a, int b);
int find_hidden_pair_values(Cell **p_cells, int *probable_hidden_pairs);
void find_hidden_pair(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                        int *p_counter);
void unset_non_hidden_candidates(Cell* p_cell, int value1, int value2, int* is_over);
int hidden_pairs(SudokuBoard *p_board);
