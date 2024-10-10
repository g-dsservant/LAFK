
#include "Util.h"
#include "Matrix.h"
#include "Matrix_private.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<time.h>


void Swap(double *a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}


double Get_element(Matrix* A, int row, int col) {

  CHECK(A);
  assert(row > 0 && row <= A->rows);
  assert(col > 0 && col <= A->cols);
  --row;
  --col;

  return MAT_POS(A, row, col);
}


void Set_element(Matrix* A, int row, int col, double value) {

  CHECK(A);
  assert(row > 0 && row <= A->rows);
  assert(col > 0 && col <= A->cols);

  --row;
  --col;
  MAT_POS(A, row, col) = value;
}



void Matrix_apply(Matrix *A, double (*f)(double)) {
  CHECK(A);

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->cols; ++j) {
      MAT_POS(A, i, j) = f(MAT_POS(A, i, j));
    }
  }
}



Matrix* Get_row(Matrix* A, int row) {
  CHECK(A);
  assert(row > 0 && row <= A->rows);

  Matrix* B = Matrix_slice(A, row, row, 1, A->cols);
  return B;
}


Matrix* Get_col(Matrix* A, int col) {
  CHECK(A);
  assert(col > 0 && col <= A->cols);

  Matrix* B = Matrix_slice(A, 1, A->rows, col, col);
  return B;
}


void Set_row(Matrix* A, int row, Matrix* B) {
  CHECK(A);
  CHECK(B);

  assert(row > 0 && row <= A->rows);
  assert(A->cols == B->cols);
  assert(B->rows == 1);

  --row;
  for (int i = 0; i < A->cols; ++i) {
    MAT_POS(A, row, i) = MAT_POS(B, 0, i);
  }
}


void Set_col(Matrix* A, int col, Matrix* B) {
  CHECK(A);
  CHECK(B);

  assert(col > 0 && col <= A->cols);
  assert(A->rows == B->rows);
  assert(B->cols == 1);

  --col;
  for (int i = 0; i < A->rows; ++i) {
    MAT_POS(A, i, col) = MAT_POS(B, i, 0);
  }
}



void Swap_rows(Matrix* A, int row1, int row2) {

  CHECK(A);

  assert(row1 > 0 && row1 <= A->rows);
  assert(row2 > 0 && row2 <= A->rows);

  if (row1 == row2) {
    return;
  }

  --row1;
  --row2;
  for (int i = 0; i < A->cols; ++i) {
      Swap(&MAT_POS(A, row1, i), &MAT_POS(A, row2, i));
  }
}



void Swap_cols(Matrix* A, int col1, int col2) {
  CHECK(A);

  assert(col1 > 0 && col1 <= A->cols);
  assert(col2 > 0 && col2 <= A->cols);

  if (col1 == col2) {
    return;
  }

  --col1;
  --col2;
  for (int i = 0; i < A->rows; ++i) {
    Swap(&MAT_POS(A, i, col1), &MAT_POS(A, i, col2));
  }
}


void Matrix_copy(Matrix* Des, Matrix* Src) {

  CHECK(Src);
  CHECK(Des);
  assert(Src->rows == Des->rows);
  assert(Src->cols == Des->cols);

  for (int i = 0; i < Des->rows; ++i) {
    for (int j = 0; j < Des->cols; ++j) {
      MAT_POS(Des, i, j) = MAT_POS(Src, i, j);
    }
  }
  return;

}


Matrix* Matrix_conc(Matrix* A, Matrix* B, int Mode) {
  CHECK(A);
  CHECK(B);

  assert(Mode == VERT || Mode == HORZ);

  if (Mode == VERT) {

    assert(A->cols == B->cols);

    Matrix *C = Matrix_resize(A, A->rows + B->rows, B->cols);

    for (int i = A->rows; i < C->rows; ++i) {
      for (int j = 0; j < C->cols; ++j) {
        int temp = i - A->rows;
        MAT_POS(C, i, j) = MAT_POS(B, temp, j);
      }
    }

    return C;
  }

  else if (Mode == HORZ) {

    assert(A->rows == B->rows);

    Matrix *C = Matrix_resize(A, A->rows, A->cols + B->cols);

    for (int i = A->cols; i < C->cols; ++i) {
      for (int j = 0; j < C->rows; ++j) {
        int temp = i - A->cols;
        MAT_POS(C, j, i) = MAT_POS(B, j, temp);
      }
    }

    return C;
  }

  return NULL;
}


