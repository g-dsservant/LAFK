

#include "Matrix_ops.h"
#include "Matrix.h"
#include "Matrix_private.h"
#include "Util.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>



void PLUDecomposition(Matrix* A, Matrix** P, Matrix** L, Matrix** U) {
  CHECK(A != NULL, "Matrix cannot be NULL!");
  CHECK(A->rows == A->cols, "Only square Matrices are allowed!");
  CHECK(!(*P) && !(*L) && !(*U), "Initialize the P, L, U matrices to NULL!");

  *P = Matrix_identity(A->rows);
  *L = Matrix_identity(A->rows);
  *U = Matrix_create(A->rows, A->cols);

  Matrix_copy(*U, A);

  for (int i = 1; i <= (*U)->rows; ++i) {

      double max = MAT_POS((*U), i, i) < 0 ? -MAT_POS((*U), i, i) : MAT_POS((*U), i, i);
      int max_row = i;
      for (int j = i + 1; j <= (*U)->rows; ++j) {
        double current = MAT_POS((*U), j, i) < 0 ? -MAT_POS((*U), j, i) : MAT_POS((*U), j, i);
        if (current > max) {
          max = MAT_POS((*U), j, i);
          max_row = j;
        }
      }

      if (!max) {
        *P = NULL;
        *L = NULL;
        *U = NULL;
        return;
      }

      Swap_rows(*U, max_row, i);
      Swap_rows(*P, max_row, i);


      for (int k = 1; k < i && i != max_row; ++k) {
        Swap(&MAT_POS((*L), max_row, k), &MAT_POS((*L), i, k), sizeof(double));
      }

      Matrix* slice_U = Matrix_slice(*U, i, i, i, (*U)->cols);


      for (int j = i + 1; j <= (*U)->rows; ++j) {
        MAT_POS((*L), j, i) = MAT_POS((*U), j, i)/MAT_POS((*U), i, i);
        Matrix *temp_row = Get_row_new(slice_U, i);
        Matrix_scalarprod(temp_row, -MAT_POS((*L), j, i));
        Matrix_add_row((*U), temp_row, j);
        Matrix_free(&temp_row);
      }
  }
}


void REF(Matrix *A) {
  CHECK(A != NULL, "Matrix cannot be NULL!");

  for (int i = 1; i <= A->rows; ++i) {

  }

}









