#pragma once

#include "sudoku.h"

typedef struct HiddenTriples_impl
{
    Cell **p_cells;
    int value1;
    int value2;
    int value3;
} HiddenTriples;

int check_hidden_triples(Cell **p_cells, int a, int b, int c);
int find_hidden_triple_values(Cell **p_cells, int *probable_hidden_triples);
void find_hidden_triple(Cell **p_cells, HiddenTriples *p_hidden_triples,
                        int *p_counter);
int hidden_triples(SudokuBoard *p_board);
