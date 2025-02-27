/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]
#define min(x, y) (((x) < (y)) ? (x) : (y))

/* Define block size */
#define mc 256
#define kc 128

/* Routine for computing C = A * B + C */

void AddDot4x4(int, double*, int, double*, int, double*, int);
void MY_MMult(int, int, int, double *, int, double *, int, double *, int);
void InnerMMult(int, int, int, double *, int, double *, int, double *, int);
void PackMatrixB(int, double *, int, double *);
void PackMatrixA(int, double *, int, double *);

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, p, pb, ib;

  for (p = 0; p < k; p += kc)
  { // Loop over the column of A per kc gap.
    pb = min(k - p, kc);
    for (i = 0; i < m; i += mc)
    { // Loop over the row of A per mc gap.
      ib = min(m - i, mc);
      InnerMMult(ib, n, pb, &A(i, p), lda, &B(p, 0), ldb, &C(i, 0), ldc);
    }
  }
}

void InnerMMult(int m, int n, int k, double *a, int lda,
                                    double *b, int ldb,
                                    double *c, int ldc)
{
  /* Do the inner block multiplication. */
  int i, j;
  double packedA[m * k], packedB[k*n];

  for ( j=0; j<n; j+=4 ){        /* Loop over the columns of C */
    PackMatrixB(k, &B(0, j), ldb, &packedB[j*k]);
    for ( i=0; i<m; i+=4 ){        /* Loop over the rows of C */
      /* Update a 4x4 block of matrix C
      C(i  , j), C(i  , j+1), C(i  , j+2), C(i  , j+3)
      C(i+1, j), C(i+1, j+1), C(i+1, j+2), C(i+1, j+3)
      C(i+2, j), C(i+2, j+1), C(i+2, j+2), C(i+2, j+3)
      C(i+3, j), C(i+3, j+1), C(i+3, j+2), C(i+3, j+3)
      */
      if (j == 0) PackMatrixA(k, &A(i, 0), lda, &packedA[i*k]);
      AddDot4x4(k, &packedA[i*k], 4, &packedB[j*k], k, &C(i, j), ldc);
    }
  }
}

void PackMatrixB(int k, double *b, int ldb, double *b_to) {
  int i;
  double *b_i0_ptr = &B(0, 0), // Unlike A, which use a single
         *b_i1_ptr = &B(0, 1), // a_ij_ptr is enough.
         *b_i2_ptr = &B(0, 2),
         *b_i3_ptr = &B(0, 3);
  for (i = 0; i < k; i++) {
    *b_to++ = *(b_i0_ptr++);
    *b_to++ = *(b_i1_ptr++);
    *b_to++ = *(b_i2_ptr++);
    *b_to++ = *(b_i3_ptr++);
  }
}

void PackMatrixA(int k, double *a, int lda, double *a_to) {
  int j;

  for (j = 0; j < k; j++) { // Loop over columns of A
    double *a_ij_ptr = &A(0, j);
    *a_to++ = *a_ij_ptr;
    *a_to++ = *(a_ij_ptr+1);
    *a_to++ = *(a_ij_ptr+2);
    *a_to++ = *(a_ij_ptr+3);
  }
}

#include <mmintrin.h>
#include <xmmintrin.h> // SSE
#include <pmmintrin.h> // SSE2
#include <emmintrin.h> // SSE3

typedef union
{
  __m128d v;
  double d[2];
} v2df_t;

void AddDot4x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc) {
  int p;
  v2df_t c_00_vreg, c_01_vreg, c_02_vreg, c_03_vreg, 
         
         c_20_vreg, c_21_vreg, c_22_vreg, c_23_vreg, 

         a_0p_vreg,            a_2p_vreg,
         b_p0_vreg, b_p1_vreg, b_p2_vreg, b_p3_vreg;

  // c_00_reg = 0.0; c_01_reg = 0.0; c_02_reg = 0.0; c_03_reg = 0.0; 
  // c_10_reg = 0.0; c_11_reg = 0.0; c_12_reg = 0.0; c_13_reg = 0.0; 
  // c_20_reg = 0.0; c_21_reg = 0.0; c_22_reg = 0.0; c_23_reg = 0.0; 
  // c_30_reg = 0.0; c_31_reg = 0.0; c_32_reg = 0.0; c_33_reg = 0.0;
  c_00_vreg.v = _mm_setzero_pd(); // pd: packed double-precision floating-point, in
                                  // SSE instruction set, it is stored in 128-bits reg.
  c_01_vreg.v = _mm_setzero_pd();
  c_02_vreg.v = _mm_setzero_pd();
  c_03_vreg.v = _mm_setzero_pd();
  c_20_vreg.v = _mm_setzero_pd();
  c_21_vreg.v = _mm_setzero_pd();
  c_22_vreg.v = _mm_setzero_pd();
  c_23_vreg.v = _mm_setzero_pd();

  for (p = 0; p < k; p++) {
    a_0p_vreg.v = _mm_load_pd((double *)a); // load 128-bits from mem_addr to m128d_dst
    a_2p_vreg.v = _mm_load_pd((double *)(a + 2));
    a += 4;

    // b_p0_reg = *b_p0_ptr++;
    // b_p1_reg = *b_p1_ptr++;
    // b_p2_reg = *b_p2_ptr++;
    // b_p3_reg = *b_p3_ptr++;
    b_p0_vreg.v = _mm_loaddup_pd((double *) b); // load and duplicate
    b_p1_vreg.v = _mm_loaddup_pd((double *) (b + 1));
    b_p2_vreg.v = _mm_loaddup_pd((double *) (b + 2));
    b_p3_vreg.v = _mm_loaddup_pd((double *) (b + 3));
    b += 4;

    /* First row and second rows */
    // c_00_reg += a_0p_reg * b_p0_reg;
    // c_10_reg += a_1p_reg * b_p0_reg;

    // c_01_reg += a_0p_reg * b_p1_reg;
    // c_11_reg += a_1p_reg * b_p1_reg;

    // c_02_reg += a_0p_reg * b_p2_reg;
    // c_12_reg += a_1p_reg * b_p2_reg;

    // c_03_reg += a_0p_reg * b_p3_reg;
    // c_13_reg += a_1p_reg * b_p3_reg;
    c_00_vreg.v += a_0p_vreg.v * b_p0_vreg.v;
    c_01_vreg.v += a_0p_vreg.v * b_p1_vreg.v;
    c_02_vreg.v += a_0p_vreg.v * b_p2_vreg.v;
    c_03_vreg.v += a_0p_vreg.v * b_p3_vreg.v;

    /* Third and fourth rows */
    // c_20_reg += a_2p_reg * b_p0_reg;
    // c_30_reg += a_3p_reg * b_p0_reg;

    // c_21_reg += a_2p_reg * b_p1_reg;
    // c_31_reg += a_3p_reg * b_p1_reg;

    // c_22_reg += a_2p_reg * b_p2_reg;
    // c_32_reg += a_3p_reg * b_p2_reg;

    // c_23_reg += a_2p_reg * b_p3_reg;
    // c_33_reg += a_3p_reg * b_p3_reg;
    c_20_vreg.v += a_2p_vreg.v * b_p0_vreg.v;
    c_21_vreg.v += a_2p_vreg.v * b_p1_vreg.v;
    c_22_vreg.v += a_2p_vreg.v * b_p2_vreg.v;
    c_23_vreg.v += a_2p_vreg.v * b_p3_vreg.v;
  }
  C( 0, 0 ) += c_00_vreg.d[0];  C( 0, 1 ) += c_01_vreg.d[0];  
  C( 0, 2 ) += c_02_vreg.d[0];  C( 0, 3 ) += c_03_vreg.d[0]; 

  C( 1, 0 ) += c_00_vreg.d[1];  C( 1, 1 ) += c_01_vreg.d[1];  
  C( 1, 2 ) += c_02_vreg.d[1];  C( 1, 3 ) += c_03_vreg.d[1]; 

  C( 2, 0 ) += c_20_vreg.d[0];  C( 2, 1 ) += c_21_vreg.d[0];  
  C( 2, 2 ) += c_22_vreg.d[0];  C( 2, 3 ) += c_23_vreg.d[0]; 

  C( 3, 0 ) += c_20_vreg.d[1];  C( 3, 1 ) += c_21_vreg.d[1];  
  C( 3, 2 ) += c_22_vreg.d[1];  C( 3, 3 ) += c_23_vreg.d[1]; 
}
