// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+
//
using namespace std;

#include "find.hh"

#include <immintrin.h>
#include <stdint.h>

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *i = b;

    __m256i q = _mm256_set1_epi8(c);

    __m256i x = _mm256_lddqu_si256(
        reinterpret_cast<const __m256i*>(i));
    __m256i r = _mm256_cmpeq_epi8(x, q);
    int z = _mm256_movemask_epi8(r);
    if (z) {
      const char *r = i + __builtin_ffs(z) - 1;
      return r < e ? r : e;
    }
    const void *a = b + 32;
    uintptr_t ai = reinterpret_cast<uintptr_t>(a);
    ai &= ~uintptr_t(0b11111u);
    a = reinterpret_cast<const void*>(ai);
    i = static_cast<const char*>(a);
    for (; i < e; i+=32) {
      __m256i x = _mm256_lddqu_si256(
          reinterpret_cast<const __m256i*>(i));
      __m256i r = _mm256_cmpeq_epi8(x, q);
      int z = _mm256_movemask_epi8(r);
      if (z) {
        const char *r = i + __builtin_ffs(z) - 1;
        return r < e ? r : e;
      }
    }

    return e;
  }
}
