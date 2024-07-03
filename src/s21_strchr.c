#include "s21_string.h"

char *s21_strchr(const char *s, int c) {
  char *answ = S21_NULL;
  int flag = 0;
  do {
    if (*s == c) {
      answ = (char *)s;
      flag = 1;
    }
  } while (*s++ != '\0' && flag != 1);
  if (flag == 0) {
    answ = S21_NULL;
  }
  return answ;
}
