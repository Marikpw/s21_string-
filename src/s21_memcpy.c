#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *tmp_dest = dest;
  const unsigned char *tmp_src = src;

  for (s21_size_t i = 0; i < n; i++) {
    tmp_dest[i] = tmp_src[i];
  }
  return dest;
}
