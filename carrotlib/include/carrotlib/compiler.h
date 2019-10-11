
#pragma once
#include <stddef.h>


#define CARROT_COMPILER_GCC             (1)



#if !defined(CARROT_CONFIG_COMPILER)
#  error "Give configuration value of CARROT_CONFIG_COMPILER"
#elif  (CARROT_CONFIG_COMPILER == CARROT_COMPILER_GCC)

#  include <sys/cdefs.h>

#  define __forceinline  __attribute__((always_inline)) inline
//#  define __nonnull(X)   __attribute__((nonnull X))
#  define __nodiscard    __attribute__((warn_unused_result))
#  define __noreturn     __attribute__((noreturn))
//#  define __unused       __attribute__((unused))
//#  define __pure         __attribute__((pure))
//#  define __const        __attribute__((const))
#  define __constrant_p(X)  __builtin_constant_p(X)
#  define __pack         __attribute__((packed))
#  define __align(X)     __attribute__((aligned(X)))
#  define __weak         __attribure__((weak))
#  if __GNUC__ >= 7
#    define __fallthrough  __attribute__((fallthrough))
#  else
#    define __fallthrough
#  endif
//#  define __unreachable()  do { __builtin_unreachable(); } while (0)

#  define __max(a, b) \
  ({ __typeof__(a) _a = (a); \
     __typeof__(b) _b = (b); \
     _a > _b? _a : _b })
 #  define __min(a, b) \
  ({ __typeof__(a) _a = (a); \
     __typeof__(b) _b = (b); \
     _a < _b? _a : _b })

//#define offsetof(typ, mbr) __builtin_offsetof(typ, mbr)
#define container_of(ptr, typ, mbr) \
  ((typ*)(char*)(ptr) - offsetof(type, mbr))


#else
#  error "Unknown compiler"
#endif




