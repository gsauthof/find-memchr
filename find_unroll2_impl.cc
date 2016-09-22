// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include "find.hh"
#include <stddef.h>

namespace g {

  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    size_t n = (e-b)/3;
    const char *i = b;
    for (size_t x = 0; x<n; ++x) {
      //if (*i == c) return i; ++i;
      //if (*i == c) return i; ++i;
      if (*i == c) return i; ++i;
      if (*i == c) return i; ++i;
      if (*i == c) return i; ++i;
    }
    switch (e-i) {
      //case 4: if (*i == c) return i; ++i;
      //case 3: if (*i == c) return i; ++i;
      case 2: if (*i == c) return i; ++i;
      case 1: if (*i == c) return i; ++i;
      default: return e;
    }
    return e;
  }


}
