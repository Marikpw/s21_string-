#include <stdio.h>

#include "s21_string.h"

void* s21_memchr(const void* ptr, int value, s21_size_t n) {
  const unsigned char* p = ptr;
  for (s21_size_t i = 0; i < n; i++) {
    if (p[i] == (unsigned char)value) {
      return (void*)(p + i);
    }
  }
  return S21_NULL;
}
