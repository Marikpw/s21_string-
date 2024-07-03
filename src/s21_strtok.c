#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last;
  char *ref;
  int func_work = 1;

  if ((ref = str) == S21_NULL && (ref = last) == S21_NULL) func_work = 0;

  for (int i = 0; func_work && *ref && delim[i]; i++) {
    if (*ref == delim[i]) {
      ref++;
      i = -1;
    }
  }

  if (func_work && *ref == '\0') {
    ref = S21_NULL;
    last = S21_NULL;
    func_work = 0;
  }
  str = ref;

  while (func_work && *ref) {
    for (s21_size_t i = 0; delim[i]; i++) {
      if (*ref == delim[i]) {
        func_work = 0;
        *ref = '\0';
        ref++;
        if (*ref == '\0') ref = S21_NULL;
        last = ref;
        break;
      }
    }

    if (func_work) {
      ref++;

      if (*ref == '\0') {
        last = S21_NULL;
      }
    }
  }
  return str;
}
