#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *cfind = S21_NULL;
  if (str != S21_NULL) {
    do {
      if (*str == (char)c) {
        cfind = str;
      }
    } while (*str++);
  }
  return (char *)cfind;
}
