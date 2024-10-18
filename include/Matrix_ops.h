
#ifndef MATRIX_OPS
#define MATRIX_OPS

#include "Matrix.h"

void PLUDecomposition(Matrix* A, Matrix** P, Matrix** L, Matrix** U);
void REF(Matrix *A);
void RREF(Matrix* A);
double Determinant(Matrix* A);
int Rank(Matrix* A);

#endif


