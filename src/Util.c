
#include "Util.h"
#include "Matrix.h"
#include "Matrix_private.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


double Get_element(Matrix* A, int row, int col) {

  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");
  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");

  return MAT_POS(A, row, col);
}


void Set_element(Matrix* A, int row, int col, double value) {

  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");
  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");

  MAT_POS(A, row, col) = value;
}



void Matrix_apply(Matrix *A, double (*f)(double)) {
  CHECK(A != NULL, "Matrix cannot be NULL!");

  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) = f(MAT_POS(A, i, j));
    }
  }
}



Matrix* Get_row(Matrix* A, int row) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");

  Matrix* B = Matrix_slice(A, row, row, 1, A->cols);
  return B;
}


Matrix* Get_row_new(Matrix* A, int row) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");

  Matrix* B = Matrix_create(1, A->cols);
  for (int i = 1; i <= A->cols; ++i) {
    MAT_POS(B, 1, i) = MAT_POS(A, row, i);
  }
  return B;
}


Matrix* Get_col(Matrix* A, int col) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");

  Matrix* B = Matrix_slice(A, 1, A->rows, col, col);
  return B;
}


Matrix* Get_col_new(Matrix* A, int col) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");

  Matrix* B = Matrix_create(A->rows, 1);
  for (int i = 1; i <= A->rows; ++i) {
    MAT_POS(B, i, 1) = MAT_POS(A, i, col);
  }
  return B;
}


void Matrix_add_row(Matrix* A, Matrix* B, int row) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(B != NULL, "Matrix cannot be NULL!");

  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");
  CHECK(A->cols == B->cols, "Cols must match!");
  CHECK(B->rows == 1, "B must be a row matrix!");

  for (int i = 1; i <= A->cols; ++i) {
    MAT_POS(A, row, i) += MAT_POS(B, 1, i);
  }
}



void Matrix_add_col(Matrix* A, Matrix* B, int col) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(B != NULL, "Matrix cannot be NULL!");

  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");
  CHECK(A->rows == B->rows, "Rows must match!");
  CHECK(B->cols == 1, "B must be a column matrix!");

  for (int i = 1; i <= A->rows; ++i) {
    MAT_POS(A, i, col) += MAT_POS(B, i, 1);
  }
}



void Set_row(Matrix* A, int row, Matrix* B) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(B != NULL, "Matrix cannot be NULL!");

  CHECK(row > 0 && row <= A->rows, "Row is out of bound!");
  CHECK(A->cols == B->cols, "Cols must match!");
  CHECK(B->rows == 1, "B must be a row matrix!");

  for (int i = 1; i <= A->cols; ++i) {
    MAT_POS(A, row, i) = MAT_POS(B, 1, i);
  }
}


void Set_col(Matrix* A, int col, Matrix* B) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(B != NULL, "Matrix cannot be NULL!");

  CHECK(col > 0 && col <= A->cols, "Col is out of bound!");
  CHECK(A->rows == B->rows, "Rows must match!");
  CHECK(B->cols == 1, "B must be a column matrix!");

  for (int i = 1; i <= A->rows; ++i) {
    MAT_POS(A, i, col) = MAT_POS(B, i, 1);
  }
}



void Swap_rows(Matrix* A, int row1, int row2) {

  CHECK(A != NULL, "Matrix cannot be NULL!");

  CHECK(row1 > 0 && row1 <= A->rows, "Row is out of bound!");
  CHECK(row2 > 0 && row2 <= A->rows, "Row is out of bound!");

  if (row1 == row2) {
    return;
  }

  for (int i = 1; i <= A->cols; ++i) {
      Swap(&MAT_POS(A, row1, i), &MAT_POS(A, row2, i), sizeof(double));
  }
}



void Swap_cols(Matrix* A, int col1, int col2) {

  CHECK(A != NULL, "Matrix cannot be NULL!");

  CHECK(col1 > 0 && col1 <= A->cols, "Col is out of bound!");
  CHECK(col2 > 0 && col2 <= A->cols, "Col is out of bound!");

  if (col1 == col2) {
    return;
  }

  for (int i = 1; i <= A->rows; ++i) {
    Swap(&MAT_POS(A, i, col1), &MAT_POS(A, i, col2), sizeof(double));
  }
}


void Matrix_copy(Matrix* Des, Matrix* Src) {

  CHECK(Src != NULL, "Src Matrix cannot be NULL!");
  CHECK(Des != NULL, "Des Matrix cannot be NULL!");
  CHECK(Src->rows == Des->rows, "Rows must match!");
  CHECK(Src->cols == Des->cols, "Cols must match!");

  for (int i = 1; i <= Des->rows; ++i) {
    for (int j = 1; j <= Des->cols; ++j) {
      MAT_POS(Des, i, j) = MAT_POS(Src, i, j);
    }
  }
  return;

}


Matrix* Matrix_conc(Matrix* A, Matrix* B, int Mode) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(B != NULL, "Matrix cannot be NULL!");

  CHECK(Mode == VERT || Mode == HORZ, "Unrecognized mode for concatenation!");

  if (Mode == VERT) {

    CHECK(A->cols == B->cols, "Cols must match for vertical concatenation!");

    Matrix *C = Matrix_resize(A, A->rows + B->rows, B->cols);

    for (int i = A->rows + 1; i <= C->rows; ++i) {
      for (int j = 1; j <= C->cols; ++j) {
        int temp = i - A->rows;
        MAT_POS(C, i, j) = MAT_POS(B, temp, j);
      }
    }

    return C;
  }

  else if (Mode == HORZ) {

    CHECK(A->rows == B->rows, "Rows must match for horizontal concatenation!");

    Matrix *C = Matrix_resize(A, A->rows, A->cols + B->cols);

    for (int i = A->cols + 1; i <= C->cols; ++i) {
      for (int j = 1; j <= C->rows; ++j) {
        int temp = i - A->cols;
        MAT_POS(C, j, i) = MAT_POS(B, j, temp);
      }
    }

    return C;
  }

  return NULL;
}


Matrix* Matrix_identity(int size) {
  CHECK(size > 0, "Size cannot be less than zero!");
  Matrix *A = Matrix_create(size, size);
  for (int i = 1; i <= size; ++i) {
    MAT_POS(A, i, i) = 1;
  }
  return A;
}


double Matrix_average(Matrix* A) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  double avg = 0.0;
  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      avg += MAT_POS(A, i, j);
    }
  }
  avg /= (A->rows * A->cols);
  return avg;
}

