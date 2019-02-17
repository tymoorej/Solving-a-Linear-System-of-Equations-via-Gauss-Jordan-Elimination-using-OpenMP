#include <stdio.h>
#include <stdlib.h>
#include "Lab3IO.h"
#include "timer.h"
#include <math.h>
#include <omp.h>

double **G; // Augmented matrix
double *x; // Pointer to output vector
int rows;
int columns;
int num_of_threads;
double *temparr;

void swap_rows_fast(int row1, int row2){
    double *temp;
    temp = G[row1];
    G[row1] = G[row2];
    G[row2] = temp;
}

void gaussian_elimination(){
    int row, under_row, column, max_row;
    double temp;
    int tempidx;
    for (row = 0; row < rows - 1; row++){ // from first row to second last row. Not parallizable 
        #pragma omp single
        {
            max_row = row;
            for (under_row = row + 1; under_row < rows; under_row++){
                if (abs(G[under_row][row]) > abs(G[max_row][row])){
                    max_row = under_row;
                }
            }
            if (max_row != row){
                swap_rows_fast(max_row, row);
            }
        }

        #pragma omp single
        for (tempidx = row + 1; tempidx < rows; tempidx++){
                temparr[tempidx] = G[tempidx][row] / G[row][row];
        }

        #pragma omp for private(temp) schedule(guided)
        for (under_row = row + 1; under_row < rows; under_row++){ // iterate over all under rows. Parallelizable
            for (column = row; column < columns; column++){ // iterate over the non-zero columns.
                G[under_row][column] -= temparr[under_row] * G[row][column];
            }
        }
    }
}

void jordan_elimination(){
    int row, above_row;
    for (row = rows - 1; row > 0; row--){ // not parallizable
        for (above_row = 0; above_row < row; above_row++){ // parallelizable
            G[above_row][columns - 1] -= (G[above_row][row] / G[row][row]) * G[row][columns - 1];
            G[above_row][row] = 0;
        }
    }
}

void store_answer(){
    int row;
    for (row = 0; row < rows; row++){
        x[row] = G[row][columns-1] / G[row][row];
    }
}

void setup(){
    int size;
    Lab3LoadInput(&G, &size);

    rows = size;
    columns = rows + 1;

    x = CreateVec(rows);

    temparr = malloc (rows * sizeof(double));
}

int main(int argc, char * argv[]){
    double start, end;

    if (argc != 2){
        printf("Invalid number of arguments\n");
        exit(0);
    }

    num_of_threads = atoi(argv[1]);
    setup();
    GET_TIME(start);
    #pragma omp parallel num_threads(num_of_threads)
    {
        gaussian_elimination();
    }
    jordan_elimination();
    GET_TIME(end);
    store_answer();
    Lab3SaveOutput(x, rows, end-start);
    return 0;
}