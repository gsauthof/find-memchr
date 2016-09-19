// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

using namespace std;

#include "find.hh"

#include <immintrin.h>
#include <string.h>

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *i = b;

    __m256i q = _mm256_set1_epi8(c);

    for (; i+32 < e; i+=32) {
      // __m256i x = _mm256_lddqu_si256(
      //    reinterpret_cast<const __m256i*>(i));

      // elimination of the reinterpret_cast
      // which might have strict-aliasing interactions,
      // in other contexts

      // GCC and Clang are able to optimize the memcpy
      // away, i.e. the 3 lines just yield:
      // vpcmpeqb -32(%r8), %ymm1, %ymm0
      __m256i x;
      memcpy(&x, i, sizeof(__m256i));
      __m256i r = _mm256_cmpeq_epi8(x, q);
      int z     = _mm256_movemask_epi8(r);
      if (z)
        return i + __builtin_ffs(z) - 1;
    }

    for (; i < e; ++i)
      if (*i == c)
        return i;

    return e;
  }
}
