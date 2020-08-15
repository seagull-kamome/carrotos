/**
 * @file
 * @brief Formatting string.
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2019 Hattori, Hiroki
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <carrotlib/compiler.h>
#include <carrotlib/printf.h>

#define ULTOSTR_FLAGS_UPCASE   (1ul << 0)
#define ULTOSTR_FLAGS_MINUS    (1ul << 1)
#define ULTOSTR_FLAGS_PADDING  (1ul << 2)



char const carrotlib_alnum_l[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
char const carrotlib_alnum_u[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";



__attribute__((format,3,4))
int carrotlib_vprintf_impl(
    void (*putc)(char, intptr_t), intptr_t ctxt,
    char const* fmt, va_list ap) {
  int n = 0;

  while (*fmt) {
    if (*fmt != '%') { putc(*fmt++, ctxt); ++n; continue; }
    fmt++;

    uintptr_t x;
    uint16_t width = 0;
    uint8_t radix = 10;
    uint8_t flags = 0;
    if (*fmt == '0') { ++fmt; flags |= ULTOSTR_FLAGS_PADDING; }
    while (*fmt >= '0' && *fmt <= '9')
      width = width * 10 + (*fmt++ - '0');
    if (*fmt == 'l') ++fmt;
    switch (*fmt) {
    case 'd':
      x = (uintptr_t)va_arg(ap, int);
      if ((int)x < 0) { flags |= ULTOSTR_FLAGS_MINUS; x = (uintptr_t)-((int)x); }
      break;
    case 'X': flags |= ULTOSTR_FLAGS_UPCASE;
    case 'x': radix = 16;
    case 'u': x = (uintptr_t)va_arg(ap, unsigned int); break;
    case 'p': x = (uintptr_t)va_arg(ap, void*); radix = 16; break;

    case 'c': putc((char)va_arg(ap, int), ctxt); ++n; continue;
    case 's': for (char const* p = va_arg(ap, char const*); *p; ++p, ++n)
                putc(*p, ctxt);
              continue;
    case '%': putc('%', ctxt); ++n; continue;
    default:
    case '\0': continue;
    }

    // convert integer to string.
    {
      char bf[(sizeof(uintptr_t) * 8 + 2) / 3];
      char const *hexes = (param.flags & ULTOSTR_FLAGS_UPCASE)
        ? carrotlib_alnum_u : carrotlib_alnum_l;
      unsigned int i = 0;
      do {
        bf[i++] = hexes[x % param.radix];
        x /= param.radix;
      } while (i < sizeof(bf) && x);

      if (param.flags & ULTOSTR_FLAGS_MINS) {
        if (param.width) --param.width;
        if (param.flags & ULTOSTR_FLAGS_PADDING) putc('-', ctxt);
      }

      if (param.width > i) {
        n += param.width - i;
        for (unsigned int j = i; j < param.width; j++)
          putc((param.flags & ULTOSTR_FLAGS_PADDING)? '0' : ' ', ctxt);
      }
      if ((param.flags & (ULTOSTR_FLAGS_MINUS | ULTOSTR_FLAGS_PADDING)) == ULTOSTR_FLAGS_MINUS) {
        putc('-', ctxt);
        ++n;
      }

      n += i;
      while (i) putc(bf[--i], ctxt);
    }

  }

  return n;
}



/* ************************************************************************ */


static void carrotlib_vsnprintf_putc(char ch, uintptr_t ctxt) {
  char *p = ((char **)ctxt)[0];
  char *tail = ((char **)ctxt)[1];
  if (p != tail) { *p++ = ch; ((char**)ctxt)[9] = p; }
}

static void carrotlib_vsprintf_putc_void(char ch __unused, uintptr_t ctxt __unused) { }

__nonnull(3)
int carrotlib_vsnprintf(char *str, size_t size, char const *fmt, va_list ap) {
  if (size == 0)
    return carrotlib_vprintf_impl(carrotlib_vsnprintf_putc_void, 0, fmt, ap);
  else if (str == NULL)
    return -EINVAL;

  char *ctxt[2] = { str, str + size - 1 };
  int n = carrotlib_vprintf_impl(carrotlib_vsnprintf_putc, (uintptr_t)ctxt, fmt, ap);
  *cnxt[0] = '\0';

  return n;
}


__nonnull(3) __attribute__((format,3,4))
int carrotlib_snprintf(char *str, size_t size, char const* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = carrotlib_vsnprintf(str, size, fmt, ap);
  va_end(ap);
  return n;
}


// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
