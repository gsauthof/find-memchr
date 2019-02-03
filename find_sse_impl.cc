// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

using namespace std;

#include "find.hh"

#include <immintrin.h>

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    size_t n = e-b;
    const char *mid = b + n/16 * 16;

    __m128i q = _mm_set1_epi8(c);

    for (const char *i = b; i < mid; i+=16) {
      // this cast is defined-behaviour because __m128i is
      // typedef'fed as may alias
      __m128i x = _mm_lddqu_si128((const __m128i*)i);
      __m128i r = _mm_cmpeq_epi8(x, q);
      int z = _mm_movemask_epi8(r);
      if (z)
        return i + __builtin_ffs(z) - 1;
    }

    for (const char *i = mid; i < e; ++i)
      if (*i == c)
        return i;

    return e;
  }
}
