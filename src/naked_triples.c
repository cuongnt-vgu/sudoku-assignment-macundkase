#include <stdlib.h>

#include "naked_triples.h"
#include "sudoku.h"

int check_naked_triples_cell(Cell *p_cell, int value1, int value2, int value3)
{
    int *candidates = get_candidates(p_cell);
    for (int i = 0; i < p_cell->num_candidates; i++)
    {
        if (candidates[i] != value1 && candidates[i] != value2 && candidates[i] != value3) 
        {
            free(candidates);
            return 0;
        }
    }
    free(candidates);
    return 1;
}

int check_naked_triples(Cell **p_cells, int a, int b, int c, int check_box) 
{
    int count = 0; // Counter for the cells containing a, b, or c
    int indices[BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        if (check_naked_triples_cell(p_cells[i], a, b, c)) 
        {
            indices[count++] = i;
            // If any cell contains a, b, or c more than 3 times, it's not a naked triple
            if (count > 3) 
            {
                return 0; // Not a naked triple
            }
        }
    }

    if (count == 3)  // Return 1 only if exactly 3 cells contain a, b, or c
    {
        if (check_box)
        {
            int i = indices[0];
            int j = indices[1];
            int k = indices[2];
            if ((p_cells[i]->row_index == p_cells[j]->row_index &&  p_cells[j]->row_index == p_cells[k]->row_index) || 
                (p_cells[i]->col_index == p_cells[j]->col_index && p_cells[j]->col_index == p_cells[k]->col_index) )
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int find_naked_triple_values(Cell **p_cells, int *probable_naked_triples)
/*
Find the number of unique values in a unit
*/
{
    int candidates_count[BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        candidates_count[i] = 0;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        
        int *candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidates_count[candidates[j] - 1] += 1;
        }
        free(candidates);
    }

    int probable_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidates_count[i] >= 2)
        {
            probable_naked_triples[probable_count] = i + 1; 
            probable_count++;
        }
    }
    return probable_count;
}


void find_naked_triple(Cell **p_cells, NakedTriples *p_naked_triples,
                        int *p_counter, int check_box)
/*
Find cells containing naked values
*/
{
    int probable_naked_triples[BOARD_SIZE];
    int probable_count = find_naked_triple_values(p_cells, probable_naked_triples);

    for(int i = 0; i < probable_count - 2; i++)
    {
        for(int j = i + 1; j < probable_count-1; j++)
        {
            for(int k = j + 1; k < probable_count; k++)
            {
                if (check_naked_triples(p_cells, probable_naked_triples[i], probable_naked_triples[j], probable_naked_triples[k], check_box))
                {
                    NakedTriples new_naked_triple;
                    new_naked_triple.p_cells = p_cells;
                    new_naked_triple.value1 = probable_naked_triples[i];
                    new_naked_triple.value2 = probable_naked_triples[j];
                    new_naked_triple.value3 = probable_naked_triples[k];
                    p_naked_triples[*p_counter] = new_naked_triple;
                    *p_counter += 1;
                } 
            }           
        }
    }
}

int naked_triples(SudokuBoard *p_board)
/*
Output the number of naked triples in the board
*/
{
    int naked_triple_counter = 0;
    NakedTriples naked_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triple(p_board->p_rows[i], naked_triples,
                           &naked_triple_counter, 0);
        find_naked_triple(p_board->p_cols[i], naked_triples,
                           &naked_triple_counter, 0);
        find_naked_triple(p_board->p_boxes[i], naked_triples,
                           &naked_triple_counter, 1);
    }

    int overlap = 0;
    int solved_count = 0;
    SolvedTriples solved_triples[naked_triple_counter];
    // int over = 0;
    for (int i = 0; i < naked_triple_counter; i++)
    {
        Cell **p_cells = naked_triples[i].p_cells;
        int value1 = naked_triples[i].value1;
        int value2 = naked_triples[i].value2;
        int value3 = naked_triples[i].value3;

        int count = 0;
        int is_overlap = 0;
        Cell* naked_p_cells[3];

        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if (!check_naked_triples_cell(p_cells[j], value1, value2, value3))
            {
                if (is_candidate(p_cells[j], value1))
                {
                    unset_candidate(p_cells[j], value1);
                }
                if (is_candidate(p_cells[j], value2))
                {
                    unset_candidate(p_cells[j], value2);
                }
                if (is_candidate(p_cells[j], value3))
                {
                    unset_candidate(p_cells[j], value3);
                }
            } 
            else naked_p_cells[count++] = p_cells[j];
        }
        for (int k = 0; k < solved_count; k++)
        {
            if (solved_triples[k].p_cell1 == naked_p_cells[0] && solved_triples[k].p_cell2 == naked_p_cells[1] && 
                solved_triples[k].p_cell3 == naked_p_cells[2]) is_overlap = 1;
        }
        if (!is_overlap)
        {
            SolvedTriples new_solved_triple;
            new_solved_triple.p_cell1 = naked_p_cells[0];
            new_solved_triple.p_cell2 = naked_p_cells[1];
            new_solved_triple.p_cell3 = naked_p_cells[2];
            solved_triples[solved_count++] = new_solved_triple;
        }
        else overlap++;
    }

    return naked_triple_counter;
}

