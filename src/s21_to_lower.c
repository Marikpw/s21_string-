#include "s21_string.h"

//Возвращает копию строки (str), преобразованной в нижний регистр. В случае
//какой-либо ошибки следует вернуть значение NULL

void *s21_to_lower(const char *str) {
  char *new_str = S21_NULL;
  if (str != S21_NULL) {
    new_str = (char *)calloc((s21_strlen(str) + 1), sizeof(char));
    s21_strncpy(new_str, str, s21_strlen(str));
    for (int i = 0; new_str[i] != '\0'; i++) {
      if (new_str[i] >= 'A' && new_str[i] <= 'Z')
        new_str[i] = new_str[i] + ('a' - 'A');
    }
  }
  return new_str;
}