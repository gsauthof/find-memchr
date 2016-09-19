# 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

CXXFLAGS = -g -Wall -std=c++11 -O3
CXXFLAGS += -march=native
CXXFLAGS_NOZERO =
CXXFLAGS += $(CXXFLAGS_NOZERO)
CXXFLAGS += -Wno-unused-function -Wno-parentheses
CXXFLAGS += -fstrict-aliasing -Wstrict-aliasing

#CXXFLAGS += -march=skylake-avx512

.PHONY: all all-mini all-x86 all-sse all-avx
all: all-mini all-x86 all-sse all-avx
all-mini: find_find find_memchr \
     find_naive find_musl find_uclibc
all-x86: find_uclibcx86
all-sse: find_diet find_sse
all-avx: find_avx2_overflow \
     find_avx2 find_avx2_overflow_ext find_avx2_align \
     find_avx2_align2 find_avx2_more find_avx2_loop \
     find_avx2_memcpy

LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

define mk-main
$1: $2
	$$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@

TEMP += $1 $2
endef

$(eval $(call mk-main,find_naive,find_naive_impl.o find_main.o))
$(eval $(call mk-main,find_musl,find_musl_impl.o find_main.o))
$(eval $(call mk-main,find_uclibc,find_uclibc_impl.o find_main.o))
$(eval $(call mk-main,find_diet,find_diet_impl.o dietlibc_memchr.o find_main.o))
$(eval $(call mk-main,find_uclibcx86,find_uclibcx86_impl.o find_main.o))
$(eval $(call mk-main,find_avx2,find_avx2_impl.o find_main.o))
$(eval $(call mk-main,find_sse,find_sse_impl.o find_main.o))
$(eval $(call mk-main,find_avx2_overflow_ext,find_avx2_overflow_impl.o find_main.o))
$(eval $(call mk-main,find_avx2_align,find_avx2_align_impl.o find_main.o))
$(eval $(call mk-main,find_avx2_align2,find_avx2_align2_impl.o find_main.o))
$(eval $(call mk-main,find_avx2_more,find_avx2_more_impl.o find_main.o))
$(eval $(call mk-main,find_avx2_memcpy,find_avx2_memcpy_impl.o find_main.o))

TEMP += dietlibc_memchr.o
TEMP += find_main.o

find_find.cc find_memchr.cc find_avx2_overflow.cc: find_main.cc
	cp $< $@

TEMP += find_find.cc find_memchr.cc find_avx2_overflow.cc

find_find.o: CPPFLAGS += -DFIND_INCLUDE='"find_find_impl.cc"' \
  -DFIND_INLINE=static
find_find.o: find_find_impl.cc
TEMP += find_find.o find_find

find_memchr.o: CPPFLAGS += -DFIND_INCLUDE='"find_memchr_impl.cc"' \
  -DFIND_INLINE=static
find_memchr.o: find_memchr_impl.cc
TEMP += find_memchr.o find_memchr


find_avx2_overflow.o: CPPFLAGS += -DFIND_INCLUDE='"find_avx2_overflow_impl.cc"' \
  -DFIND_INLINE=static
find_avx2_overflow.o: find_avx2_overflow_impl.cc
TEMP += find_avx2_overflow.o find_avx2_overflow

TEMP += find_avx2_loop


.PHONY: clean
clean:
	rm -f $(TEMP)
