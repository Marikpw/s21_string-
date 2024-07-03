#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *new_str = S21_NULL;
  if (str == S21_NULL && src != S21_NULL) {
    new_str = (char *)calloc((s21_strlen(src) + 1), sizeof(char));
    s21_strncpy(new_str, src, s21_strlen(src));
  }
  if (src != S21_NULL && str != S21_NULL && start_index <= s21_strlen(src)) {
    new_str =
        (char *)calloc((s21_strlen(src) + s21_strlen(str) + 1), sizeof(char));
    s21_strncpy(new_str, src, start_index);
    s21_strncat(new_str, str, s21_strlen(str));
    s21_strncat(new_str, src + start_index, s21_strlen(src) - start_index);
  }
  return new_str;
}
