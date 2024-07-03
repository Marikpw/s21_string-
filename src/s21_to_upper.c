#include "s21_string.h"

//Возвращает копию строки (str), преобразованной в верхний регистр. В случае
//какой-либо ошибки следует вернуть значение NULL

void *s21_to_upper(const char *str) {
  char *new_str = S21_NULL;
  if (str != S21_NULL) {
    new_str = (char *)calloc((s21_strlen(str) + 1), sizeof(char));
    s21_strncpy(new_str, str, s21_strlen(str) + 1);
    for (int i = 0; new_str[i] != '\0'; i++) {
      if (new_str[i] >= 'a' && new_str[i] <= 'z')
        new_str[i] = new_str[i] - ('a' - 'A');
    }
  }
  return new_str;
}