/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void AddDot(int, double*, int, double*, double*);
void AddDot1x4(int, double*, int, double*, int, double*, int);
void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j;

  for ( j=0; j<n; j+=4 ){    /* Loop over the columns of C */
    for (i = 0; i < m; i++) { /* Loop over the rows of C */
      /* Update C(i, j), C(i, j+1), C(i, j+2), C(i, j+3)*/
      AddDot1x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
    }
  }
}

void AddDot1x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc) {
  /* B(0, 0...3)是相对于传入的 b 数组进行的偏移 */
  AddDot(k, &A(0, 0), lda, &B(0, 0), &C(0, 0));
  AddDot(k, &A(0, 0), lda, &B(0, 1), &C(0, 1));
  AddDot(k, &A(0, 0), lda, &B(0, 2), &C(0, 2));
  AddDot(k, &A(0, 0), lda, &B(0, 3), &C(0, 3));
}

#define X(i) x[(i)*incx]

void AddDot(int k, double* x, int incx, double* y, double* gamma) {
  /* 此函数的计算的内容表示如下：
    gamma := x * y + gamma
    即乘加运算。
  */
  int p;
  for (p = 0; p < k; p++) {
    *gamma += X(p) * y[p];
  }
}
