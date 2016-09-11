#include <algorithm>
using namespace std;

#include "find.hh"

namespace g {
  FIND_INLINE const char *find(const char *b, const char *e, char c)
  {
    return std::find(b, e, c);
  }
}
