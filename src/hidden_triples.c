#include <stdlib.h>
#include <stdio.h>

#include "hidden_triples.h"
#include "sudoku.h"

int check_hidden_triples(Cell **p_cells, int a, int b, int c)
{
    int count = 0; // Counter for the cells containing a, b, or c

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        if (is_candidate(p_cells[i], a) || is_candidate(p_cells[i], b) || is_candidate(p_cells[i], c)) 
        {
            count++;
            // If the count exceeds 3, it's not a hidden triple
            if (count > 3) 
            {
                return 0; // Not a hidden triple
            }
        }
    }
    return (count == 3); // Return 1 only if exactly 3 cells contain a, b, or c
}

int find_hidden_triple_values(Cell **p_cells, int *probable_hidden_triples)
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
        if (candidates_count[i] == 2 || candidates_count[i] == 3)
        {
            probable_hidden_triples[probable_count] = i + 1; 
            probable_count++;
        }
    }
    return probable_count;
}


void find_hidden_triple(Cell **p_cells, HiddenTriples *p_hidden_triples,
                        int *p_counter)
/*
Find cells containing hidden values
*/
{
    int probable_hidden_triples[BOARD_SIZE];
    int probable_count = find_hidden_triple_values(p_cells, probable_hidden_triples);

    for(int i = 0; i < probable_count - 2; i++)
    {
        for(int j = i + 1; j < probable_count - 1; j++)
        {
            for(int k = j + 1; k < probable_count; k++)
            {
                if (check_hidden_triples(p_cells, probable_hidden_triples[i], probable_hidden_triples[j], probable_hidden_triples[k]))
                {
                    
                    HiddenTriples new_hidden_triple;
                    new_hidden_triple.p_cells = p_cells;
                    new_hidden_triple.value1 = probable_hidden_triples[i];
                    new_hidden_triple.value2 = probable_hidden_triples[j];
                    new_hidden_triple.value3 = probable_hidden_triples[k];
                    p_hidden_triples[*p_counter] = new_hidden_triple;
                    *p_counter += 1;                    
                }
            }
        }
    }
}
int hidden_triples(SudokuBoard *p_board)
/*
Output the number of hidden triples in the board
*/
{
    int hidden_triple_counter = 0;
    HiddenTriples hidden_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_triple(p_board->p_rows[i], hidden_triples,
                           &hidden_triple_counter);
        find_hidden_triple(p_board->p_cols[i], hidden_triples,
                           &hidden_triple_counter);
        find_hidden_triple(p_board->p_boxes[i], hidden_triples,
                           &hidden_triple_counter);
    }

    int over = 0;
    for (int i = 0; i < hidden_triple_counter; i++)
    {
        int is_over = 1;
        Cell **p_cells = hidden_triples[i].p_cells;
        int value1 = hidden_triples[i].value1;
        int value2 = hidden_triples[i].value2;
        int value3 = hidden_triples[i].value3;
        for(int m = 0; m < BOARD_SIZE; m++)
        {
            if(is_candidate(p_cells[m], value1) || is_candidate(p_cells[m], value2) || is_candidate(p_cells[m], value3) ) 
            {
                int* candidate = get_candidates(p_cells[m]);
                int num_candidates = p_cells[m]->num_candidates;
                for(int j = 0; j < num_candidates; j++)
                {
                    if (candidate[j] != value1 && candidate[j] != value2 && candidate[j] != value3)
                    {
                        unset_candidate(p_cells[m], candidate[j]);
                        is_over = 0;
                    }
                }
                free(candidate);
            }
        }
        over += is_over;
    }
    return hidden_triple_counter - over;
}

