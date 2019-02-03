// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include <stdexcept>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

namespace x {
  int open(const char *f, int flags)
  {
    auto r = ::open(f, flags);
    if (r == -1)
        throw runtime_error(strerror(errno));
    return r;
  }
  ssize_t read(int fd, void *b, size_t n)
  {
    auto r = ::read(fd, b, n);
    if (r == -1)
        throw runtime_error(strerror(errno));
    return r;
  }
  off_t lseek(int fd, off_t off, int w)
  {
    auto r = ::lseek(fd, off, w);
    if (r == (off_t)-1) throw runtime_error(strerror(errno));
    return r;
  }
}

#ifdef FIND_INCLUDE
  #include FIND_INCLUDE
#else
  #include "find.hh"
#endif


int main(int argc, char **argv)
{
  if (argc < 3)
    return 1;
  unsigned n = atoi(argv[1]);
  int fd = x::open(argv[2], O_RDONLY);
  enum { N = 128 * 1024 };
  // char buffer[N];
  // char buffer[N+32];
  char buffer[N+64];
  for (unsigned i = 0; i < n; ++i) {
    size_t off = 0;
    if (i)
      x::lseek(fd, 0, SEEK_SET);
    for (;;) {
      auto r = x::read(fd, buffer, N);
      const char *b = buffer;
      auto e = b + r;
      for (;;) {
        auto x = g::find(b, e, '\n');
        auto y = g::find(b, x, '|');
        off += y - b;
        if (y != x)
          printf("%zu\n", off);
        if (x == e)
          break;
        off = 0;
        b = x + 1;
      }
      if (r != N)
        break;
    }
  }
  return 0;
}
