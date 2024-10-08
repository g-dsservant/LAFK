#ifndef MATRIX_PRIVATE
#define MATRIX_PRIVATE

#include "Matrix.h"

#define CHECK(A) assert(A != NULL)
#define MAT_POS(m, i, j) (m->elements)[i*(m->row_stride) + j*(m->col_stride)]
#define FLAT_INDEX(m, i, j) (i*(m->row_stride) + j*(m->col_stride))

struct Matrix {
  int rows;
  int cols;
  int row_stride;
  int col_stride;
  double *elements;
  int memfree_flag;
};

static void Swap(double *a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}


#endif
