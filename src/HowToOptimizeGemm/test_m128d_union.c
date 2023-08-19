#include <mmintrin.h>
#include <xmmintrin.h>

typedef union
{
    __m128d v;
    double d[2];
} xt;

int main()
{
    xt m_r;
    m_r.v = _mm_set_pd((double)1.0, (double)2.0);

    return 0;
}