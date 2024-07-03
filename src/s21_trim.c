#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL || trim_chars == S21_NULL) return S21_NULL;
  s21_size_t src_len = s21_strlen(src);
  s21_size_t start_index = 0;
  s21_size_t end_index = src_len - 1;
  // Находим индекс первого символа, не являющегося trim_char, в начале строки
  for (; start_index < src_len &&
         s21_strchr(trim_chars, src[start_index]) != S21_NULL;
       start_index++)
    ;
  // Если вся строка состоит из trim_chars, возвращаем пустую строку
  if (start_index == src_len) {
    char *empty_str = (char *)malloc(sizeof(char));
    *empty_str = '\0';
    return empty_str;
  }
  // Находим индекс последнего символа, не являющегося trim_char, в конце строки
  for (; end_index > start_index &&
         s21_strchr(trim_chars, src[end_index]) != S21_NULL;
       end_index--)
    ;
  // Вычисляем новую длину строки после обрезки
  s21_size_t trimmed_len = end_index - start_index + 1;
  // Выделяем память для новой строки
  char *trimmed_str = (char *)malloc((trimmed_len + 1) * sizeof(char));
  // Копируем в нее обрезанную часть исходной строки
  s21_strncpy(trimmed_str, src + start_index, trimmed_len);
  trimmed_str[trimmed_len] = '\0';
  return trimmed_str;
}