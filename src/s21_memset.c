#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *p = str;
  unsigned int v = (unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) {
    p[i] = v;
  }
  return str;
}