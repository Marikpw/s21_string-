#include "s21_string.h"

int s21_memcmp(const void* ptr1, const void* ptr2, s21_size_t n) {
  const unsigned char *p1 = ptr1, *p2 = ptr2;
  int result = 0;
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] - p2[i];
    }
  }
  return result;
}
