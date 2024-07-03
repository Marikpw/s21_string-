#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (str1[i] > str2[i]) {
      return 1;
    }
    if (str1[i] < str2[i]) {
      return -1;
    }
    if (str1[i] == '\0' && str2[i] == '\0') {
      return 0;
    }
  }
  return 0;
}
