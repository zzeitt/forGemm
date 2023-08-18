/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void AddDot(int, double*, int, double*, double*);

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j;

  for ( i=0; i<m; i++ ){        /* Loop over the rows of C */
    for ( j=0; j<n; j++ ){        /* Loop over the columns of C */
      /* Update C( i,j ) with the inner
				       product of the ith row of A and
				       the jth column of B */
      AddDot(k, &A(i, 0), lda, &B(0, j), &C(i, j));
    }
  }
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
