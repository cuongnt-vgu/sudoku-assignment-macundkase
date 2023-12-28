#include <stdlib.h>
#include <stdio.h>

#include "naked_pairs.h"
#include "sudoku.h"

int check_naked_pairs(Cell **p_cells, int a, int b)
{
    for (int i = 0; i < BOARD_SIZE-1; i++)
    {
        for (int j = i+1; j < BOARD_SIZE; j++)
        {
            if ((is_candidate(p_cells[i], a) && is_candidate(p_cells[i], b) && p_cells[i]->num_candidates == 2) &&
                (is_candidate(p_cells[j], a) && is_candidate(p_cells[j], b) && p_cells[j]->num_candidates == 2))
            {
                return 1;
            }
        }
    }
    return 0;
}

int find_naked_pair_values(Cell **p_cells, int *probable_naked_pairs)
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
        if (p_cells[i]->num_candidates == 1) continue;
        
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
            probable_naked_pairs[probable_count] = i + 1; 
            probable_count++;
        }
    }
    return probable_count;
}


void find_naked_pair(Cell **p_cells, NakedPairs *p_naked_pairs,
                        int *p_counter)
/*
Find cells containing naked values
*/
{
    int probable_naked_pairs[BOARD_SIZE];
    int probable_count = find_naked_pair_values(p_cells, probable_naked_pairs);

    for(int i = 0; i < probable_count - 1; i++)
    {
        for(int j = i + 1; j < probable_count; j++)
        {
            if (check_naked_pairs(p_cells, probable_naked_pairs[i], probable_naked_pairs[j]))
            {
                NakedPairs new_naked_pair;
                new_naked_pair.p_cells = p_cells;
                new_naked_pair.value1 = probable_naked_pairs[i];
                new_naked_pair.value2 = probable_naked_pairs[j];
                p_naked_pairs[*p_counter] = new_naked_pair;
                *p_counter += 1;
            }            
        }
    }
}

int naked_pairs(SudokuBoard *p_board)
/*
Output the number of naked pairs in the board
*/
{
    int naked_pair_counter = 0;
    NakedPairs naked_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_pair(p_board->p_rows[i], naked_pairs,
                           &naked_pair_counter);
        find_naked_pair(p_board->p_cols[i], naked_pairs,
                           &naked_pair_counter);
        find_naked_pair(p_board->p_boxes[i], naked_pairs,
                           &naked_pair_counter);
    }

    int overlap = 0;
    int solved_count = 0;
    SolvedPairs solved_pairs[naked_pair_counter];
    for (int i = 0; i < naked_pair_counter; i++)
    {
        Cell **p_cells = naked_pairs[i].p_cells;
        int value1 = naked_pairs[i].value1;
        int value2 = naked_pairs[i].value2;

        int count = 0;
        int is_overlap = 0;
        Cell* naked_p_cells[2];
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(!(is_candidate(p_cells[j], value1) && is_candidate(p_cells[j], value2) && p_cells[j]->num_candidates == 2))
            {
                if (is_candidate(p_cells[j], value1))
                {
                    unset_candidate(p_cells[j], value1);
                }
                if (is_candidate(p_cells[j], value2))
                {
                    unset_candidate(p_cells[j], value2);
                }
            } 
            else naked_p_cells[count++] = p_cells[j];
        }
        for (int k = 0; k < solved_count; k++)
        {
            if (solved_pairs[k].p_cell1 == naked_p_cells[0] && solved_pairs[k].p_cell2 == naked_p_cells[1]) is_overlap = 1;
        }
        if (!is_overlap)
        {
            SolvedPairs new_solved_pair;
            new_solved_pair.p_cell1 = naked_p_cells[0];
            new_solved_pair.p_cell2 = naked_p_cells[1];
            solved_pairs[solved_count++] = new_solved_pair;
        }
        else overlap++;
    }

    return naked_pair_counter-overlap;
}

