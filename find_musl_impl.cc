#include "find.hh"

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

// imported from
// https://git.musl-libc.org/cgit/musl/tree/src/string/memchr.c
// MIT license
// and modified to compile as C++


#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

static void *memchr_musl(const void *src, int c, size_t n)
{
  const unsigned char *s = (const unsigned char*) src;
  c = (unsigned char)c;
  for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
  if (n && *s != c) {
    const size_t *w;
    size_t k = ONES * c;
    for (w = (const size_t *)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
    for (s = (const unsigned char*)w; n && *s != c; s++, n--);
  }
  return n ? (void *)s : 0;
}

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *r = static_cast<const char*>(memchr_musl(b, c, e-b));
    return r ? r : e;
  }
}
