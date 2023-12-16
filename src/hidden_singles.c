#include <stdlib.h>

#include "hidden_singles.h"
#include "sudoku.h"

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
/*
Find the number of unique values in a unit
*/
{
    int num_hidden_values = 0;
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

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidates_count[i] == 1)
        {
            hidden_single_values[num_hidden_values++] = i + 1;
        }
    }
    return num_hidden_values;
}

void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles,
                        int *p_counter)
/*
Find cells containing hidden values
*/
{
    int hidden_single_values[BOARD_SIZE];
    int num_hidden_values = find_hidden_single_values(p_cells, hidden_single_values);

    for (int i = 0; i < num_hidden_values; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_cells[j]->num_candidates > 1 && is_candidate(p_cells[j], hidden_single_values[i]))
            {
                p_hidden_singles[(*p_counter)++] = (HiddenSingle){p_cells[j], hidden_single_values[i]};
            }
        }
    }
}

int hidden_singles(SudokuBoard *p_board)
/*
Output the number of hidden singles in the board
*/
{
    int hidden_single_counter = 0;
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles,
                           &hidden_single_counter);
        find_hidden_single(p_board->p_cols[i], hidden_singles,
                           &hidden_single_counter);
        find_hidden_single(p_board->p_boxes[i], hidden_singles,
                           &hidden_single_counter);
    }

    int duplicate = 0;
    for (int i = 0; i < hidden_single_counter; i++)
    {
        if (hidden_singles[i].p_cell->num_candidates == 1)
        {
            duplicate++;
            continue;
        }
        set_candidates(hidden_singles[i].p_cell,
                       (int *)(int[]){hidden_singles[i].value}, 1);
    }

    return hidden_single_counter-duplicate;
}

