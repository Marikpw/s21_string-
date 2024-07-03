#ifndef S21_STRING_H
#define S21_STRING_H

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_size_t unsigned long long
#define S21_NULL (void *)0

typedef struct {
  int minus;
  int plus;
  int space;
  int hash;
  int zero;
  int width;
  int precision;
  char lenght;
  int num_sys;
  int flag_to_size;
  int dot;
  int up_case;
  int g;
  int e;
} specifiers;

void *s21_memcpy(void *dest, const void *src, s21_size_t n);
s21_size_t s21_strlen(const char *str);
char *s21_strchr(const char *s, int c);
char *s21_strncat(char *dest, const char *src, s21_size_t n);

int s21_sprintf(char *str, const char *format, ...);
const char *set_specs(specifiers *specs, const char *format, va_list *args);
const char *get_specs(const char *format, specifiers *specs);
const char *get_width(const char *format, specifiers *specs, va_list *args);
const char *get_precision(const char *format, specifiers *specs, va_list *args);
char *parser(char *str, char *str_start, const char *format, specifiers *specs,
             va_list *args);
char *print_c(char *str, specifiers *specs, int sym);
char *print_di(char *str, specifiers *specs, va_list *args);
s21_size_t get_size_of_d(specifiers *specs, long int num);
int decimal_to_string(specifiers *specs, long int num, char *str_to_num,
                      s21_size_t size_to_d);
void check_plus_minus_space(long int num, specifiers *specs, char *str_to_num,
                            int *i, s21_size_t *size_to_d);
char get_num_char(int num, int up_case);
void set_specs_for_ge(specifiers *specs, const char *format);
char *print_f(char *str, specifiers *specs, va_list *args);
s21_size_t get_size_of_f(specifiers *specs, long double num);
char *print_eg(char *str, specifiers *specs, va_list *args);
void cutter(specifiers *specs, long double num);
int normalize(specifiers *specs, long double *num);
int double_to_string(char *str_to_double, specifiers *specs, long double num,
                     s21_size_t size, int e);
void check_plus_minus_space_double(long double num, char *str_to_double,
                                   specifiers *specs, int *i, s21_size_t *size);
void add_sym_from_double_to_str(char *str_to_double, specifiers *specs,
                                int precision, s21_size_t *size, int *i,
                                long double frac, long double integer);
int nan_or_inf(char *str_to_double, long double num);
void print_e(int e, s21_size_t *size, char *str_to_double, specifiers *specs,
             int *i);
void add_sym_to_str(char *str, int *i, char c);
char *print_s(char *str, specifiers *specs, va_list *args);
char *print_p(char *str, specifiers *specs, va_list *args);
void set_num_sys(specifiers *specs, const char *format);
char *print_u(char *str, specifiers *specs, va_list *args);
s21_size_t get_size_of_unsigneg(specifiers *specs, unsigned long int num);
int unsigned_to_string(char *str_to_num, specifiers *specs,
                       unsigned long int num, s21_size_t size_to_unsigned,
                       int flag_ptr);
void check_hash(specifiers *specs, char *str_to_num, int *i,
                s21_size_t *size_to_unsigned);

void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);
void *s21_trim(const char *src, const char *trim_chars);

size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
int s21_strncmp(const char *str1, const char *str2, size_t n);
char *s21_strncpy(char *dest, const char *src, size_t n);

void *s21_memchr(const void *ptr, int value, s21_size_t n);
int s21_memcmp(const void *ptr1, const void *ptr2, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);

char *s21_strtok(char *str, const char *delim);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strrchr(const char *str, int c);
char *s21_strpbrk(const char *s1, const char *s2);

#endif  // S21_STRING_H
