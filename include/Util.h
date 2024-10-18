
#ifndef UTIL
#define UTIL

#include "Matrix.h"

#define VERT 5
#define HORZ 6

void Matrix_copy(Matrix* Des, Matrix* Src);

double Get_element(Matrix* A, int row, int col);
void Set_element(Matrix* A, int row, int col, double value);

void Matrix_apply(Matrix *A, double (*f)(double));

Matrix* Get_row(Matrix* A, int row);
Matrix* Get_row_new(Matrix* A, int row);
Matrix* Get_col(Matrix* A, int col);
Matrix* Get_col_new(Matrix* A, int col);

void Matrix_add_row(Matrix* A, Matrix* B, int row);
void Matrix_add_col(Matrix* A, Matrix* B, int col);

void Set_row(Matrix* A, int row, Matrix* B);
void Set_col(Matrix* A, int col, Matrix* B);

void Swap_rows(Matrix* A, int row1, int row2);
void Swap_cols(Matrix* A, int col1, int col2);

Matrix* Matrix_conc(Matrix* A, Matrix* B, int Mode);

Matrix* Matrix_identity(int size);

double Matrix_average(Matrix* A);



#endif



