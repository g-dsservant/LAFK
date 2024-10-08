#ifndef UTIL
#define UTIL

#include "Matrix.h"

#define VERT 5
#define HORZ 6

double Get_element(Matrix* A, int row, int col);
void Set_element(Matrix* A, int row, int col, double value);
Matrix* Get_row(Matrix* A, int row);
Matrix* Get_col(Matrix* A, int col);
void Set_row(Matrix* A, int row, Matrix* B);
void Set_col(Matrix* A, int col, Matrix* B);
void Swap_rows(Matrix* A, int row1, int row2);
void Swap_cols(Matrix* A, int col1, int col2);
void Matrix_copy(Matrix* A, Matrix* B);
Matrix* Matrix_conc(Matrix* A, Matrix* B, int Mode);

#endif
