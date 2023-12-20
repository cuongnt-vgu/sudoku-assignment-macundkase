#pragma once

#include "sudoku.h"

typedef struct NakedTriples_impl
{
    Cell **p_cells;
    int value1;
    int value2;
    int value3;
} NakedTriples;

int check_naked_triples(Cell **p_cells, int a, int b, int c, int check_box);
int find_naked_triple_values(Cell **p_cells, int *probable_naked_triples);
void find_naked_triple(Cell **p_cells, NakedTriples *p_naked_triples,
                        int *p_counter, int check_box);
int naked_triples(SudokuBoard *p_board);
