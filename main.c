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


void swap_rows(int row1, int row2){
    int column;
    double temp;
    for (column = 0; column < columns; column++){
        temp = G[row1][column];
        G[row1][column] = G[row2][column];
        G[row2][column] = temp;
    }
}

void test1(){
    int i;
    #pragma omp parallel num_threads(2) private(i)
    {
        for (i = 0; i < 5; i++){
            printf("%d, %d\n", omp_get_thread_num(), i);
        }
    }
}

void test2(){
    int i;
    #pragma omp parallel num_threads(2) private(i)
    {
        for (i = 0; i < 5; i++){
            #pragma omp single
            {
                printf("%d, %d\n", omp_get_thread_num(), i);
            }
        }
    }
}

void test3(){
    int i;
    #pragma omp parallel for num_threads(5) private(i)
    for (i = 0; i < 5; i++){
        printf("%d, %d\n", omp_get_thread_num(), i);
    }
}

void gaussian_elimination(){
    int row, under_row, column, max_row;
    double temp;
    #pragma omp parallel num_threads(2) private(row, under_row, column, max_row) shared(temp)
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
                swap_rows(max_row, row);
            }
        }       

        for (under_row = row + 1; under_row < rows; under_row++){ // iterate over all under rows. Parallelizable
            #pragma omp single
            temp = G[under_row][row] / G[row][row];

            #pragma omp for
            for (column = row; column < columns; column++){ // iterate over the non-zero columns.
                G[under_row][column] -= temp * G[row][column];
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
    gaussian_elimination();
    jordan_elimination();
    // test1();
    // test2();
    // test3();
    GET_TIME(end);
    store_answer();
    Lab3SaveOutput(x, rows, end-start);
    return 0;
}