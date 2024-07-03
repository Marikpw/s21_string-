#include "s21_string.h"

char *s21_strpbrk(const char *s1, const char *s2) {
  if (s1 == S21_NULL || s2 == S21_NULL) {
    return S21_NULL;
  }
  for (; *s1; s1++) {
    if (s21_strchr(s2, *s1) != S21_NULL) {
      return (char *)s1;
    }
  }
  return S21_NULL;
}
