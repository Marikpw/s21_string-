#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, size_t n) {
  //Проверка на условие, не является ли первая строка нулевой
  if (dest == S21_NULL) {
    return S21_NULL;
  }

  //Проверка на условие, не является ли вторая строка нулевой
  if (src != S21_NULL) {
    // check - длина строки src
    size_t check = 0;
    while (src[check]) {
      check++;
    }
    //Проверка количества копируемых символов
    if (n > 0) {
      size_t i;
      //Сравнение количества символов, больше или меньше размера копируемой
      //строки
      if (n <= check) {
        for (i = 0; i < n; i++) {
          dest[i] = src[i];
        }
      } else {
        for (i = 0; i < check; i++) {
          dest[i] = src[i];
        }
        for (; i < n; i++) {
          dest[i] = '\0';
        }
      }
    }
  }
  return dest;
}
