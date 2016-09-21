// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include "find.hh"

namespace g {

// only GCC supports this pragma/attribute
// (as of clang 3.9, icc 13)
// when given, GCC (e.g. 5.3.1, 6.2) does unroll the loop 8 times

// alternativ to using the attribute:
// #pragma GCC push_options
// #pragma GCC optimize ("unroll-loops")

  __attribute__((optimize("unroll-loops")))
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    for (const char *i = b; i < e; ++i)
      if (*i == c)
        return i;
    return e;
  }

// #pragma GCC pop_options

}
