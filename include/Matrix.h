#ifndef MATRIX
#define MATRIX

typedef struct Matrix Matrix;


Matrix* Matrix_create(int rows, int cols);
Matrix* Matrix_fromArray(int rows, int cols, double *arr);
void Matrix_free(Matrix **A);
void Matrix_freeN(int n, ...);

void Matrix_print(Matrix *A, const char *name);

void Matrix_fill(Matrix *A, double num);
void Matrix_rand(Matrix *A, int low, int high);

int colsize(Matrix *A);
int rowsize(Matrix *A);

void Matrix_sum(Matrix *A, Matrix *B);
Matrix* Matrix_sum_new(Matrix *A, Matrix *B);
void Matrix_multiply(Matrix *Result, Matrix *A, Matrix *B);
Matrix* Matrix_multiply_new(Matrix *A, Matrix *B);
void Matrix_difference(Matrix *A, Matrix *B);
Matrix* Matrix_difference_new(Matrix *A, Matrix *B);
void Matrix_haddamard(Matrix *A, Matrix *B);
Matrix* Matrix_haddamard_new(Matrix *A, Matrix *B);
void Matrix_scalarprod(Matrix *A, double B);
Matrix* Matrix_scalarprod_new(Matrix *A, double B);

Matrix* Matrix_transpose(Matrix *A);
Matrix* Matrix_transpose_new(Matrix *A);
Matrix* Matrix_submatrix(Matrix *A, int row_stride, int col_stride);
Matrix* Matrix_slice(Matrix* A, int row_start, int row_end, int col_start, int col_end);

Matrix* Matrix_resize(Matrix *A, int rows, int cols);

#endif
