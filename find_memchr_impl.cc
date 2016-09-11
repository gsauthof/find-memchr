// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include <string.h>
using namespace std;

#include "find.hh"

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *r = static_cast<const char*>(memchr(b, c, e-b));
    return r ? r : e;
  }
}
