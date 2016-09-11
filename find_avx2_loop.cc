// 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

#include <stdexcept>
#include <immintrin.h>

#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

namespace x {
  int open(const char *f, int flags) { auto r = ::open(f, flags); if (r == -1) throw runtime_error(strerror(errno)); return r; }
  ssize_t read(int fd, void *b, size_t n) { auto r = ::read(fd, b, n); if (r == -1) throw runtime_error(strerror(errno)); return r; }
  off_t lseek(int fd, off_t off, int w) { auto r = ::lseek(fd, off, w); if (r == (off_t)-1) throw runtime_error(strerror(errno)); return r; }
}

class Matcher {
  private:
    __m256i q_;
    int mask_;
  public:
    enum { INC = 32 };
    Matcher(char c)
      :
        q_(_mm256_set1_epi8(c))
    {
    }
    inline void match(const __m256i &x)
    {
      __m256i r = _mm256_cmpeq_epi8(x, q_);
      mask_ = _mm256_movemask_epi8(r);
    }
    unsigned next()
    {
      if (mask_) {
        auto r =  __builtin_ffs(mask_) - 1;
        mask_ ^= 1u << r;
        //mask_ &= ~(1u << r);
        return r;
      } else {
        return 32u;
      }
    }
};

int main(int argc, char **argv)
{
  if (argc < 3)
    return 1;
  unsigned n = atoi(argv[1]);
  int fd = x::open(argv[2], O_RDONLY);
  enum { N = 128 * 1024 };
  char buffer[N] __attribute__((aligned(32)));
  for (unsigned i = 0; i < n; ++i) {
    size_t off = 0;
    size_t last = 0;
    if (i)
      x::lseek(fd, 0, SEEK_SET);
    for (;;) {
      auto r = x::read(fd, buffer, N);
      const char *b = buffer;
      auto e = b + r;
      if (r != N)
        memset(buffer+r, 0, min(32u, unsigned(N-r)));
      Matcher dl('|');
      Matcher nl('\n');
      for (const char *i = b; i<e; i+=32) {
        __m256i x = _mm256_lddqu_si256(
        //__m256i x = _mm256_load_si256(
             reinterpret_cast<const __m256i*>(i));
        dl.match(x);
        nl.match(x);
        for (;;) {
          auto x = dl.next();
          auto y = nl.next();
          if (x == y)
            break;
          if (y < x)
            last = off+y+1;
          if (x < 32)
            printf("%zu\n", off+x-last);
          if (y < 32)
            last = off+y+1;
        }
        off += 32;
      }
      if (r != N)
        break;
    }
  }
  return 0;
}
