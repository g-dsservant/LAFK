#include "Matrix.h"
#include "Matrix_private.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<stdarg.h>


Matrix* Matrix_create(int rows, int cols) {

  assert(rows > 0);
  assert(cols > 0);

  Matrix* m = (Matrix *)malloc(sizeof(Matrix));
  m->rows = rows;
  m->cols = cols;
  m->row_stride = cols;
  m->col_stride = 1;
  m->elements = (double *)malloc(sizeof(double)*rows*cols);
  m->memfree_flag = 1;
  return m;
}


void Matrix_free(Matrix **A) {
  if ((*A)->memfree_flag) {
    free((*A)->elements);
  }
  (*A)->elements = NULL;
  free(*A);
  *A = NULL;
}


void Matrix_freeN(int n, ...) {

  va_list Matrices;
  va_start(Matrices, n);

  for (int i = 0; i < n; ++i) {
    Matrix **Temp = va_arg(Matrices, Matrix **);
    Matrix_free(Temp);
  }

  va_end(Matrices);

}


void Matrix_print(Matrix *A, const char* name) {

  CHECK(A);

  printf("\n\n rows = %d, cols = %d\n", A->rows, A->cols);
  printf("\n\n %s = [ ", name);
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      printf("%0.10f   ", MAT_POS(A, i, j));
    }
    printf("\n");
  }
  printf("]\n\n");

}


void Matrix_fill(Matrix *A, double num) {

  CHECK(A);

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) = num;
    }
  }
}


void Matrix_rand(Matrix *A, int low, int high) {

  CHECK(A);

  static int seed_set = 0;
  if (!seed_set) {
    srand(time(NULL));
    seed_set = 1;
  }

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) = low + ((double)rand()/RAND_MAX)*(high-low+1);
    }
  }
}


void Matrix_sum(Matrix *A, Matrix *B) {

  CHECK(A);
  CHECK(B);

  assert(A->rows == B->rows);
  assert(A->cols == B->cols);


  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) += MAT_POS(B, i, j);
    }
  }
}


void Matrix_multiply(Matrix *Result, Matrix *A, Matrix *B) {

  CHECK(Result);
  CHECK(A);
  CHECK(B);

  assert(A->cols == B->rows);
  assert(Result->rows == A->rows);
  assert(Result->cols == B->cols);

  int inner = A->cols;

  Matrix_fill(Result, 0);

  for (int i = 0; i < Result->rows; ++i) {
    for (int j = 0; j < Result->cols; ++j) {
      for (int k = 0; k < inner; ++k) {
        MAT_POS(Result, i, j) += MAT_POS(A, i, k)*MAT_POS(B, k, j);
      }
    }
  }
}


void Matrix_difference(Matrix *A, Matrix *B) {

  CHECK(A);
  CHECK(B);

  assert(A->rows == B->rows);
  assert(A->cols == B->cols);


  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) -= MAT_POS(B, i, j);
    }
  }
}


void Matrix_haddamard(Matrix *A, Matrix *B) {

  CHECK(A);
  CHECK(B);

  assert(A->rows == B->rows);
  assert(A->cols == B->cols);


  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) *= MAT_POS(B, i, j);
    }
  }
}


void Matrix_scalarprod(Matrix *A, double B) {
  CHECK(A);

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) *= B;
    }
  }
}


void Matrix_transpose(Matrix *A) {

  CHECK(A);

  for (int i = 0; i < A->rows; ++i) {
    for (int j = i + 1; j < A->cols; ++j) {
      Swap(&MAT_POS(A, i, j), &MAT_POS(A, j, i));
    }
  }
}


Matrix* Matrix_submatrix(Matrix *A, int row_stride, int col_stride) {
  CHECK(A);
  assert(row_stride > 0 && row_stride <= A->rows);
  assert(col_stride > 0 && col_stride <= A->cols);

  int rows = ((A->rows + row_stride - 1) / row_stride);
  int cols = ((A->cols + col_stride - 1) / col_stride);

  Matrix* B = (Matrix *)malloc(sizeof(Matrix));

  B->rows = rows;
  B->cols = cols;
  B->row_stride = A->row_stride * row_stride;
  B->col_stride = A->col_stride * col_stride;
  B->elements = A->elements;
  B->memfree_flag = 0;

  return B;
}


Matrix* Matrix_slice(Matrix* A, int row_start, int row_end, int col_start, int col_end) {
  CHECK(A);

  assert(row_start > 0 && row_start <= row_end);
  assert(row_end > 0 && row_end <= A->rows);
  assert(col_start > 0 && col_start <= col_end);
  assert(col_end > 0 && col_end <= A->cols);

  int rows = row_end - row_start + 1;
  int cols = col_end - col_start + 1;

  Matrix *B = (Matrix *)malloc(sizeof(Matrix));

  B->rows = rows;
  B->cols = cols;
  B->row_stride = A->row_stride;
  B->col_stride = A->col_stride;
  --row_start;
  --col_start;
  B->elements = A->elements + FLAT_INDEX(A, row_start, col_start);
  B->memfree_flag = 0;

  return B;
}



Matrix* Matrix_resize(Matrix *A, int rows, int cols) {
  CHECK(A);

  assert (rows > 0 && cols > 0);

  Matrix *B = (Matrix *)malloc(sizeof(Matrix));

  B->rows = rows;
  B->cols = cols;
  B->row_stride = cols;
  B->col_stride = 1;
  B->memfree_flag = 1;

  B->elements = (double *)malloc(sizeof(double)*rows*cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (i < A->rows && j < A->cols) {
        MAT_POS(B, i, j) = MAT_POS(A, i, j);
      }
      else {
        MAT_POS(B, i, j) = 0;
      }
    }
  }
  return B;
}
