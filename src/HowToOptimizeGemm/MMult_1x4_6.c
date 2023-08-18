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

  for ( i=0; i<m; i++ ){        /* Loop over the rows of C */
    for ( j=0; j<n; j+=4 ){        /* Loop over the columns of C */
      /* Update C(i, j), C(i, j+1), C(i, j+2), C(i, j+3)*/
      AddDot1x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
    }
  }
}

void AddDot1x4(int k, double *a, int lda, double *b, int ldb, double *c, int ldc)
{
  int p;
  register double c_00_reg, c_01_reg, c_02_reg, c_03_reg,
      a_0p_reg;
  c_00_reg = 0.0;
  c_01_reg = 0.0;
  c_02_reg = 0.0;
  c_03_reg = 0.0;

  for (p = 0; p < k; p++)
  {
    a_0p_reg = A(0, p);
    c_00_reg += a_0p_reg * B(p, 0);
    c_01_reg += a_0p_reg * B(p, 1);
    c_02_reg += a_0p_reg * B(p, 2);
    c_03_reg += a_0p_reg * B(p, 3);
  }
  C(0, 0) += c_00_reg;
  C(0, 1) += c_01_reg;
  C(0, 2) += c_02_reg;
  C(0, 3) += c_03_reg;
}
