// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include "find.hh"

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

extern "C" {
void *memchr_diet(const void *src, int c, size_t n);
}

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *r = static_cast<const char*>(memchr_diet(b, c, e-b));
    return r ? r : e;
  }
}
