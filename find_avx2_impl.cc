// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

using namespace std;

#include "find.hh"

#include <immintrin.h>

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    size_t n = e-b;
    const char *mid = b + n/32 * 32;

    __m256i q = _mm256_set1_epi8(c);

    for (const char *i = b; i < mid; i+=32) {
      // this cast is defined-behaviour because __m256i is
      // typedef'fed as may alias
      __m256i x = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(i));
      __m256i r = _mm256_cmpeq_epi8(x, q);
      int z     = _mm256_movemask_epi8(r);
      if (z)
        return i + __builtin_ffs(z) - 1;
    }

    for (const char *i = mid; i < e; ++i)
      if (*i == c)
        return i;

    return e;
  }
}
