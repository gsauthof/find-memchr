#include "find.hh"

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

// imported from uclibc 0.9.33.2, lgpl
// libc/string/i386/memchr.c
// cf. http://www.uclibc.org/
// modified for 64 bit

namespace g {
  static void *memchr_uclibcx86(const void *s, int c, size_t count)
  {
          void *rdi;
          long rcx;
          __asm__ __volatile__(
                  "	jecxz	1f\n"
                  "	repne; scasb\n"
                  "	lea	-1(%%rdi), %%rdi\n"
                  "	je	2f\n"
                  "1:\n"
                  "	xor	%%rdi, %%rdi\n" /* NULL */
                  "2:\n"
                  : "=&D" (rdi), "=&c" (rcx)
                  : "a" (c), "0" (s), "1" (count)
                  /* : no clobbers */
          );
          return rdi;
  }
  FIND_INLINE const char *find(const char *b, const char *e,
      char c)
  {
    const char *r = static_cast<const char*>(memchr_uclibcx86(b, c, e-b));
    return r ? r : e;
  }
}
