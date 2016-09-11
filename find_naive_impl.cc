// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include "find.hh"

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    for (const char *i = b; i < e; ++i)
      if (*i == c)
        return i;
    return e;
  }
}
