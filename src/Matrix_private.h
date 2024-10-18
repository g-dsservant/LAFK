
#ifndef MATRIX_PRIVATE
#define MATRIX_PRIVATE

#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define MAT_POS(m, i, j) (m->elements)[(i-1)*(m->row_stride) + (j-1)*(m->col_stride)]
#define FLAT_INDEX(m, i, j) ((i-1)*(m->row_stride) + (j-1)*(m->col_stride))


struct Matrix {
  int rows;
  int cols;
  int row_stride;
  int col_stride;
  double *elements;
  int memfree_flag;
};


void Swap(void* a, void* b, size_t size);


static inline void CHECK(int condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "%s", message);
    exit(1);
  }
}


#endif
