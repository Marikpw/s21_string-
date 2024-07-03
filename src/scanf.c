#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "s21_string.h"

long int char_int(const char *bufer, int base) {
  long int rez = 0;
  int znak = 1;
  int len = s21_strlen(bufer);
  for (int i = 0; i < len; i++) {
    long int digit = 0;
    if (bufer[i] == '-') {
      i++;
      znak = -1;
    } else if (bufer[i] == '+')
      i++;
    if (bufer[i] >= '0' && bufer[i] <= '9')
      digit = bufer[i] - '0';
    else if (bufer[i] >= 'a' && bufer[i] <= 'f')
      digit = bufer[i] - 'a' + 10;
    else if (bufer[i] >= 'A' && bufer[i] <= 'F')
      digit = bufer[i] - 'A' + 10;
    rez = rez * base + digit;
  }
  return rez * znak;
}

int s21_sscanf(const char *str, const char *format, ...) {
  int count = 0;
  va_list args;
  va_start(args, format);

  while (*format != '\0' && *str != '\0') {
    if (isspace(*format)) {
      // Пропускаем пробельные символы в формате
      while (isspace(*format)) {
        format++;
      }
    } else if (*format == '%') {
      // Найден спецификатор формата
      format++;
      int width = 0;
      while (isdigit(*format)) {
        width = width * 10 + (*format - '0');
        format++;
      }
      bool is_david = false;
      if (*format == '*') {
        is_david = true;
        format++;
        // str++;
        va_arg(args, int);  // Пропускаем аргумент
      }
      bool is_long = false;
      // bool is_long_double = false;
      bool is_short = false;
      while (*format == 'l' || *format == 'h' || *format == 'L') {
        if (*format == 'l') {
          is_long = true;
        } else if (*format == 'h') {
          is_short = true;
        } else if (*format == 'L') {
          // is_long_double = true;
        }
        format++;
      }
      if (*format == '%') {
        // Это двойной процент - просто пропускаем
        if (*format == *str) {
          format++;
          str++;
        } else {
          break;  // Проценты не совпадают - прерываем цикл
        }
      } else if (*format == 'c') {
        // Спецификатор для символа
        if (!is_david) {
          char *ch = va_arg(args, char *);
          *ch = *str;
          count++;
        }
        format++;
        str++;
      } else if (*format == 'd' || *format == 'i') {
        // Спецификатор для целых чисел
        if (!is_david) {
          int *n = va_arg(args, int *);
          *n = (is_long) ? strtol(str, (char **)&str, 10)
                         : (int)strtol(str, (char **)&str, 10);
          count++;
        }
        // while (*str != '\0' && (isdigit(*str) || *str == '-' || *str == '+'))
        // {
        //   str++;
        // }
        format++;
      } else if (*format == 'u' || *format == 'o' || *format == 'x' ||
                 *format == 'X') {
        // Спецификаторы для беззнаковых целых чисел
        // if (is_short) {
        //   unsigned short int *n = va_arg(args, unsigned short int *);
        // } else
        //   //unsigned int *n = va_arg(args, unsigned int *);
        if (*format == 'o') {
          while (*str == '0') {
            str++;
          }
        }
        if (!is_david) {
          if (is_short) {
            *va_arg(args, unsigned short int *) =
                (is_long)
                    ? strtoul(str, (char **)&str, (*format == 'o') ? 8 : 10)
                    : (unsigned int)strtoul(str, (char **)&str,
                                            (*format == 'o') ? 8 : 10);
            // } else if (is_long) {
            //   *va_arg(args, unsigned long int *) =
            //       strtoul(str, (char **)&str, (*format == 'o') ? 8 : 10);

          } else
            *va_arg(args, unsigned int *) =
                strtoul(str, (char **)&str, (*format == 'o') ? 8 : 10);
          // (is_long) ? strtoul(str, (char **)&str, (*format == 'o') ? 8 : 10)
          //           : (unsigned int)strtoul(str, (char **)&str,
          //                                   (*format == 'o') ? 8 : 10);
          count++;
        }
        while (*str != '\0' && isdigit(*str)) {
          str++;
        }
        format++;
      } else if (*format == 'f' || *format == 'e' || *format == 'E' ||
                 *format == 'g' || *format == 'G') {
        // Спецификаторы для чисел с плавающей точкой
        float *f = va_arg(args, float *);
        *f = (float)strtod(str, (char **)&str);
        count++;
        while (*str != '\0' && (isdigit(*str) || *str == '.' || *str == 'e' ||
                                *str == 'E' || *str == '+' || *str == '-')) {
          str++;
        }
        format++;
      } else if (*format == 's') {
        // Спецификатор для строки
        char *s = va_arg(args, char *);
        while (isspace(*str))
          // efefef
          str++;
        while (*str != '\0' && !isspace(*str)) {
          *s = *str;
          s++;
          str++;
        }
        *s = '\0';
        count++;
        while (*str != '\0' && isspace(*str)) {
          str++;
        }
        format++;
      } else if (*format == 'p') {
        // Спецификатор для адреса указателя
        unsigned long int *ptr = va_arg(args, void *);
        *ptr = char_int(str, 16);
        count++;
        format++;
        str++;
      } else if (*format == 'n') {
        // Спецификатор для количества символов
        int *num_chars = va_arg(args, int *);
        *num_chars = str - va_arg(args, const char *);
        count++;
        format++;
      } else {
        // Неизвестный спецификатор - прерываем цикл
        break;
      }
    } else {
      // Символы в формате и в строке не совпадают - прерываем цикл
      if (*format != *str) {
        break;
      }
      format++;
      str++;
    }
  }

  va_end(args);
  return count;
}
