#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

typedef struct matrix {
    double** m;
    int rows;
    int cols;
} matrix;

matrix* init(int rows, int cols);
matrix* copy(matrix* m);
matrix* add(matrix* a, matrix* b);
matrix* multiply(matrix* a, matrix* b);
matrix* multiply_scalar(matrix* m, double s);
matrix* inverse(matrix* m);
matrix* adj(matrix* m);
matrix* transpose(matrix* m);
matrix* cofactor(matrix* m);
matrix* flatten(matrix* m);
matrix* get(matrix* m, int rMin, int rMax, int cMin, int cMax, int ignoreRow, int ignoreColumn);
matrix* get_from_cmd(void);
double det(matrix* m);
double get_double(char* msg);
int get_int(char* msg);
void set(matrix* a, matrix* b);
void set_matrix(matrix* m, double** n, int nRows, int nCols);
void set_matrix_static(matrix* m, int nRows, int nCols, double n[nRows][nCols], int flipped);
void multiply_scalar_d(matrix* m, double s);
void clear(matrix* m);
void display(matrix* m);
void free_matrix(matrix* m);
void free_matrices(void);

#endif
