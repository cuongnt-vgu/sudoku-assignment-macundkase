#include <stdlib.h>
#include <stdio.h>

#include "hidden_pairs.h"
#include "sudoku.h"   

int check_hidden_pairs(Cell **p_cells, int a, int b)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if(is_candidate(p_cells[i], a) && !is_candidate(p_cells[i], b)) return 0;
        if(is_candidate(p_cells[i], b) && !is_candidate(p_cells[i], a)) return 0;
    }
    return 1; 
}

int find_hidden_pair_values(Cell **p_cells, int *probable_hidden_pairs)
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
        if (candidates_count[i] == 2)
        {
            probable_hidden_pairs[probable_count] = i + 1; 
            probable_count++;
        }
    }
    return probable_count;
}


void find_hidden_pair(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                        int *p_counter)
/*
Find cells containing hidden values
*/
{
    int probable_hidden_pairs[BOARD_SIZE];
    int probable_count = find_hidden_pair_values(p_cells, probable_hidden_pairs);

    for(int i = 0; i < probable_count - 1; i++)
    {
        for(int j = i + 1; j < probable_count; j++)
        {
            if (check_hidden_pairs(p_cells, probable_hidden_pairs[i], probable_hidden_pairs[j]))
            {
                for(int k = 0; k < BOARD_SIZE-1; k++)
                {
                    for (int l = k+1; l < BOARD_SIZE; l++)
                    {
                        if(is_candidate(p_cells[k], probable_hidden_pairs[i]) && is_candidate(p_cells[l], probable_hidden_pairs[i])) 
                        {
                            HiddenPairs new_hidden_pair;
                            new_hidden_pair.p_cell_1 = p_cells[k];
                            new_hidden_pair.p_cell_2 = p_cells[l];
                            new_hidden_pair.value1 = probable_hidden_pairs[i];
                            new_hidden_pair.value2 = probable_hidden_pairs[j];
                            p_hidden_pairs[*p_counter] = new_hidden_pair;
                            *p_counter += 1;
                        }
                    }
                }
            }
        }
    }
}

void unset_non_hidden_candidates(Cell* p_cell, int value1, int value2, int* is_over)
{
    int* candidate = get_candidates(p_cell);
    int num_candidates = p_cell->num_candidates;
    for(int j = 0; j < num_candidates; j++)
    {
        if (candidate[j] != value1 && candidate[j] != value2)
        {
            unset_candidate(p_cell, candidate[j]);
            *is_over = 0;
        }
    }
    free(candidate);
}

int hidden_pairs(SudokuBoard *p_board)
/*
Output the number of hidden pairs in the board
*/
{
    int hidden_pair_counter = 0;
    HiddenPairs hidden_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_pair(p_board->p_rows[i], hidden_pairs,
                           &hidden_pair_counter);
        find_hidden_pair(p_board->p_cols[i], hidden_pairs,
                           &hidden_pair_counter);
        find_hidden_pair(p_board->p_boxes[i], hidden_pairs,
                           &hidden_pair_counter);
    }

    int overlap = 0;
    for (int i = 0; i < hidden_pair_counter; i++)
    {
        Cell *p_cell_1 = hidden_pairs[i].p_cell_1;
        Cell *p_cell_2 = hidden_pairs[i].p_cell_2;
        int value1 = hidden_pairs[i].value1;
        int value2 = hidden_pairs[i].value2;

        int is_over = 1;
        
        unset_non_hidden_candidates(p_cell_1, value1, value2, &is_over);
        unset_non_hidden_candidates(p_cell_2, value1, value2, &is_over);
        overlap += is_over;
    }

    return hidden_pair_counter-overlap;
}

