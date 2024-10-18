
#include "Matrix.h"
#include "Matrix_private.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdarg.h>
#include<string.h>
#include<stddef.h>

void Swap(void* a, void* b, size_t size) {
    if (a == b) {
      return;
    }
    unsigned char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}


Matrix* Matrix_create(int rows, int cols) {

  CHECK(rows > 0, "Rows have to be greater than 0!");
  CHECK(cols > 0, "Cols have to be greater than 0!");

  Matrix* m = (Matrix *)malloc(sizeof(Matrix));
  m->rows = rows;
  m->cols = cols;
  m->row_stride = cols;
  m->col_stride = 1;
  m->elements = (double *)malloc(sizeof(double)*rows*cols);
  m->memfree_flag = 1;
  return m;
}

Matrix* Matrix_fromArray(int rows, int cols, double *arr) {

  CHECK(rows > 0, "Rows have to be greater than 0!");
  CHECK(cols > 0, "Cols have to be greater than 0!");

  Matrix* m = (Matrix *)malloc(sizeof(Matrix));
  m->rows = rows;
  m->cols = cols;
  m->row_stride = cols;
  m->col_stride = 1;
  m->elements = (double *)malloc(sizeof(double)*rows*cols);
  memcpy(m->elements, arr, sizeof(double)*rows*cols);
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

  CHECK(A != NULL, "Cannot print a NULL Matrix!");

  printf("\n\n %s = [ ", name);
  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      printf("%0.17f   ", MAT_POS(A, i, j));
    }
    printf("\n");
  }
  printf("]\n\n");

}


void Matrix_fill(Matrix *A, double num) {

  CHECK(A != NULL, "Cannot fill a NULL Matrix!");

  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) = num;
    }
  }
}


void Matrix_rand(Matrix *A, int low, int high) {

  CHECK(A != NULL, "Cannot randomize a NULL Matrix!");

  static int seed_set = 0;
  if (!seed_set) {
    srand(time(NULL));
    seed_set = 1;
  }

  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) = low + ((double)rand()/RAND_MAX)*(high-low);
    }
  }
}


int colsize(Matrix *A) {
  CHECK(A != NULL, "Cannot get columns of a NULL Matrix!");
  return A->cols;
}


int rowsize(Matrix *A) {
  CHECK(A != NULL, "Cannot get rows of a NULL Matrix!");
  return A->rows;
}


void Matrix_sum(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for sum!");
  CHECK(A->cols == B->cols, "Cols of operands must match for sum!");


  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) += MAT_POS(B, i, j);
    }
  }
}



Matrix* Matrix_sum_new(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for sum!");
  CHECK(A->cols == B->cols, "Cols of operands must match for sum!");

  Matrix *C = (Matrix *)malloc(sizeof(Matrix));
  C->rows = A->rows;
  C->cols = A->cols;
  C->row_stride = C->cols;
  C->col_stride = 1;
  C->memfree_flag = 1;
  C->elements = (double *)malloc(sizeof(double)*C->rows*C->cols);

  for (int i = 1; i <= C->rows; ++i) {
    for (int j = 1; j <= C->cols; ++j) {
      MAT_POS(C, i, j) = MAT_POS(A, i, j) + MAT_POS(B, i, j);
    }
  }

  return C;
}


void Matrix_multiply(Matrix *Result, Matrix *A, Matrix *B) {

  CHECK(Result != NULL, "Result Matrix cannot be NULL!");
  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(A != NULL, "Operands cannot be NULL!");

  CHECK(A->cols == B->rows, "Cols and Rows must match for multiplication!");
  CHECK(Result->rows == A->rows, "Rows of Result must match for multiplication!");
  CHECK(Result->cols == B->cols, "Cols of Result must match for multiplication!");

  int inner_dim = A->cols;
  for (int i = 1; i <= Result->rows; ++i) {
    for (int j = 1; j <= Result->cols; ++j) {
      MAT_POS(Result, i, j) = 0;
      for (int k = 1; k <= inner_dim; ++k) {
        MAT_POS(Result, i, j) += MAT_POS(A, i, k)*MAT_POS(B, k, j);
      }
    }
  }
}


Matrix* Matrix_multiply_new(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->cols == B->rows, "Cols and Rows must match for multiplication!");

  Matrix *C = (Matrix *)malloc(sizeof(Matrix));
  C->rows = A->rows;
  C->cols = B->cols;
  C->row_stride = C->cols;
  C->col_stride = 1;
  C->memfree_flag = 1;
  C->elements = (double *)malloc(sizeof(double)*C->rows*C->cols);

  int inner_dim = A->cols;
  for (int i = 1; i <= C->rows; ++i) {
    for (int j = 1; j <= C->cols; ++j) {
      MAT_POS(C, i, j) = 0;
      for (int k = 1; k <= inner_dim; ++k) {
        MAT_POS(C, i, j) += MAT_POS(A, i, k)*MAT_POS(B, k, j);
      }
    }
  }

  return C;

}


void Matrix_difference(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for difference!");
  CHECK(A->cols == B->cols, "Cols of operands must match for difference!");


  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) -= MAT_POS(B, i, j);
    }
  }
}


Matrix* Matrix_difference_new(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for difference!");
  CHECK(A->cols == B->cols, "Cols of operands must match for difference!");

  Matrix *C = (Matrix *)malloc(sizeof(Matrix));
  C->rows = A->rows;
  C->cols = A->cols;
  C->row_stride = C->cols;
  C->col_stride = 1;
  C->memfree_flag = 1;
  C->elements = (double *)malloc(sizeof(double)*C->rows*C->cols);

  for (int i = 1; i <= C->rows; ++i) {
    for (int j = 1; j <= C->cols; ++j) {
      MAT_POS(C, i, j) =  MAT_POS(A, i, j) - MAT_POS(B, i, j);
    }
  }
  return C;
}


void Matrix_haddamard(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for haddamard product!");
  CHECK(A->cols == B->cols, "Cols of operands must match for haddamard product!");


  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) *= MAT_POS(B, i, j);
    }
  }
}


Matrix* Matrix_haddamard_new(Matrix *A, Matrix *B) {

  CHECK(A != NULL, "Operands cannot be NULL!");
  CHECK(B != NULL, "Operands cannot be NULL!");

  CHECK(A->rows == B->rows, "Rows of operands must match for haddamard product!");
  CHECK(A->cols == B->cols, "Cols of operands must match for haddamard product!");

  Matrix *C = (Matrix *)malloc(sizeof(Matrix));
  C->rows = A->rows;
  C->cols = A->cols;
  C->row_stride = C->cols;
  C->col_stride = 1;
  C->elements = (double *)malloc(sizeof(double)*C->rows*C->cols);
  C->memfree_flag = 1;

  for (int i = 1; i <= C->rows; ++i) {
    for (int j = 1; j <= C->cols; ++j) {
      MAT_POS(C, i, j) = MAT_POS(A, i, j)*MAT_POS(B, i, j);
    }
  }

  return C;
}


void Matrix_scalarprod(Matrix *A, double B) {

  CHECK(A != NULL, "Operands cannot be NULL!");

  for (int i = 1; i <= A->rows; ++i) {
    for (int j = 1; j <= A->cols; ++j) {
      MAT_POS(A, i, j) *= B;
    }
  }

}


Matrix* Matrix_scalarprod_new(Matrix *A, double B) {

  CHECK(A != NULL, "Operands cannot be NULL!");

  Matrix *C = (Matrix *)malloc(sizeof(Matrix));
  C->rows = A->rows;
  C->cols = A->cols;
  C->row_stride = C->cols;
  C->col_stride = 1;
  C->elements = (double *)malloc(sizeof(double)*C->rows*C->cols);
  C->memfree_flag = 1;

  for (int i = 1; i <= C->rows; ++i) {
    for (int j = 1; j <= C->cols; ++j) {
      MAT_POS(C, i, j) = MAT_POS(A, i, j)*B;
    }
  }

  return C;

}




Matrix* Matrix_transpose(Matrix *A) {

  CHECK(A != NULL, "Operands cannot be NULL!");

  Matrix* B = (Matrix *)malloc(sizeof(Matrix));
  B->rows = A->cols;
  B->cols = A->rows;
  B->row_stride = A->col_stride;
  B->col_stride = A->row_stride;
  B->elements = A->elements;
  B->memfree_flag = 0;
  return B;
}


Matrix* Matrix_transpose_new(Matrix *A) {

  CHECK(A != NULL, "Operands cannot be NULL!");

  Matrix* B = (Matrix *)malloc(sizeof(Matrix));
  B->rows = A->cols;
  B->cols = A->rows;
  B->row_stride = B->cols;
  B->col_stride = 1;
  B->memfree_flag = 1;
  B->elements = (double *)malloc(sizeof(double)*B->rows*B->cols);

  Swap(&A->row_stride, &A->col_stride, sizeof(int));

  for (int i = 1; i <= B->rows; ++i) {
    for (int j = 1; j <= B->cols; ++j) {
        MAT_POS(B, i, j) = MAT_POS(A, i, j);
    }
  }

  Swap(&A->row_stride, &A->col_stride, sizeof(int));
  return B;
}


Matrix* Matrix_submatrix(Matrix *A, int row_stride, int col_stride) {

  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(row_stride > 0 && row_stride <= A->rows, "Row stride is out of bound!");
  CHECK(col_stride > 0 && col_stride <= A->cols, "Col stride is out of bound!");

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

  CHECK(A != NULL, "Matrix cannot be NULL!");

  CHECK(row_start > 0 && row_start <= row_end, "Rows are out of bound!");
  CHECK(row_end > 0 && row_end <= A->rows, "Rows are out of bound!");
  CHECK(col_start > 0 && col_start <= col_end, "Cols are out of bound!");
  CHECK(col_end > 0 && col_end <= A->cols, "Cols are out of bound!");

  int rows = row_end - row_start + 1;
  int cols = col_end - col_start + 1;

  Matrix *B = (Matrix *)malloc(sizeof(Matrix));

  B->rows = rows;
  B->cols = cols;
  B->row_stride = A->row_stride;
  B->col_stride = A->col_stride;
  B->elements = A->elements + FLAT_INDEX(A, row_start, col_start);
  B->memfree_flag = 0;

  return B;
}





Matrix* Matrix_resize(Matrix *A, int rows, int cols) {

  CHECK(A != NULL, "Matrix cannot be NULL!");

  CHECK(rows > 0 && cols > 0, "Rows/Cols cannot be less than 0!");

  Matrix *B = (Matrix *)malloc(sizeof(Matrix));

  B->rows = rows;
  B->cols = cols;
  B->row_stride = cols;
  B->col_stride = 1;
  B->memfree_flag = 1;

  B->elements = (double *)malloc(sizeof(double)*rows*cols);

  for (int i = 1; i <= rows; ++i) {
    for (int j = 1; j <= cols; ++j) {
      if (i <= A->rows && j <= A->cols) {
        MAT_POS(B, i, j) = MAT_POS(A, i, j);
      }
      else {
        MAT_POS(B, i, j) = 0;
      }
    }
  }

  return B;

}






