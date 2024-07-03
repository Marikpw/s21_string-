#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  char *str_start = str;
  char spec[] = "cdeEfgGosuxXp%";
  va_list(args);
  va_start(args, format);

  while (*format) {
    if (*format == '%') {
      format++;
      specifiers specs = {0};
      specs.num_sys = 10;
      format = set_specs(&specs, format, &args);
      while (!s21_strchr(spec, *format)) format++;
      str = parser(str, str_start, format, &specs, &args);
    } else {
      *str = *format;
      str++;
    }
    format++;
  }

  *str = '\0';
  va_end(args);
  return (str - str_start);
}

const char *set_specs(specifiers *specs, const char *format, va_list *args) {
  format = get_specs(format, specs);
  format = get_width(format, specs, args);
  if (*format == '.') {
    specs->dot = 1;
    format++;
    format = get_precision(format, specs, args);
  }
  if (*format == 'h')
    specs->lenght = 'h';
  else if (*format == 'l')
    specs->lenght = 'l';
  else if (*format == 'L')
    specs->lenght = 'L';
  if (specs->lenght) format++;
  return format;
}

const char *get_specs(const char *format, specifiers *specs) {
  while (format) {
    if (*format == '-')
      specs->minus = 1;
    else if (*format == '+')
      specs->plus = 1;
    else if (*format == ' ')
      specs->space = 1;
    else if (*format == '#')
      specs->hash = 1;
    else if (*format == '0')
      specs->zero = 1;
    else
      break;
    format++;
  }
  specs->space = (specs->space && !specs->plus);  // plus == 1 make space = 0
  specs->zero = (specs->zero && !specs->minus);   // minus == 1 make zero = 0
  return format;
}

const char *get_width(const char *format, specifiers *specs, va_list *args) {
  specs->width = 0;
  if (*format == '*') {
    specs->width = va_arg(*args, int);
    format++;
  }
  while (format) {
    if ('0' <= *format && *format <= '9') {
      specs->width *= 10;
      specs->width += *format - '0';
    } else
      break;
    format++;
  }
  return format;
}

const char *get_precision(const char *format, specifiers *specs,
                          va_list *args) {
  specs->precision = 0;
  if (*format == '*') {
    specs->precision = va_arg(*args, int);
    format++;
  }
  while (format) {
    if ('0' <= *format && *format <= '9') {
      specs->precision *= 10;
      specs->precision += *format - '0';
    } else
      break;
    format++;
  }
  return format;
}

char *parser(char *str, char *str_start, const char *format, specifiers *specs,
             va_list *args) {
  if (*format == 'c') {
    int sym = va_arg(*args, int);
    str = print_c(str, specs, sym);
  } else if (*format == 'd' || *format == 'i') {
    str = print_di(str, specs, args);
  } else if (*format == 'f') {
    str = print_f(str, specs, args);
  } else if (*format == 'e' || *format == 'E' || *format == 'g' ||
             *format == 'G') {
    set_specs_for_ge(specs, format);
    str = print_eg(str, specs, args);
  } else if (*format == 's') {
    str = print_s(str, specs, args);
  } else if (*format == 'p') {
    str = print_p(str, specs, args);
  } else if (*format == 'u' || *format == 'o' || *format == 'x' ||
             *format == 'X') {
    set_num_sys(specs, format);
    str = print_u(str, specs, args);
  } else if (*format == '%') {
    str = print_c(str, specs, '%');
  } else {
    str = S21_NULL;
  }
  if (!str) *str_start = '\0';
  return str;
}

char *print_c(char *str, specifiers *specs, int sym) {
  char *p = S21_NULL;
  while (specs->width - 1 > 0 && !specs->minus) {
    *str = ' ';
    str++;
    specs->width--;
  }
  if (sym <= 127) {
    *str = sym;
    str++;
    while (specs->width - 1 > 0 && specs->minus) {
      *str = ' ';
      str++;
      specs->width--;
    }
    p = str;
  }
  return p;
}

char *print_di(char *str, specifiers *specs, va_list *args) {
  long int num = 0;
  if (specs->lenght == 'l') {
    num = (long int)va_arg(*args, long int);
  } else if (specs->lenght == 'h') {
    num = (short)va_arg(*args, int);
  } else
    num = (int)va_arg(*args, int);
  s21_size_t size_to_d = get_size_of_d(specs, num);  // check size for buffer
  char *str_to_num = calloc(size_to_d, sizeof(char));
  if (str_to_num) {
    int i = decimal_to_string(specs, num, str_to_num, size_to_d);
    for (int j = i - 1; j >= 0; j--) {
      *str = str_to_num[j];
      str++;
    }
    while (i < specs->width) {
      *str = ' ';
      str++;
      i++;
    }
    free(str_to_num);
  }
  return str;
}

s21_size_t get_size_of_d(specifiers *specs, long int num) {
  s21_size_t result = 0;
  long int num_copy = num;
  if (num_copy < 0) {
    if (num_copy == LONG_MIN || num_copy == INT_MIN) {
      num_copy += 1;
      num_copy = -num_copy;
    } else
      num_copy = -num_copy;
  }
  while (num_copy > 0) {
    num_copy /= 10;
    result++;
  }
  if (num_copy == 0 && result == 0 &&
      (specs->precision || specs->width || specs->space)) {
    result++;
  }
  int numbers = result;
  if ((s21_size_t)specs->width > result) result = specs->width;
  if ((s21_size_t)specs->precision > result) result = specs->precision;
  if (specs->space || specs->plus || num < 0) {
    specs->flag_to_size = 1;
    if (numbers >= specs->width || num == 0 || specs->precision >= specs->width)
      result++;
  }
  if (result == 0 && num_copy == 0 && !specs->precision && !specs->width &&
      !specs->space && !specs->dot)
    result++;
  return result;
}

int decimal_to_string(specifiers *specs, long int num, char *str_to_num,
                      s21_size_t size_to_d) {
  int flag = 0, flag_min = 0;
  if (num < 0) {
    flag = 1;
    if (num == LONG_MIN || num == INT_MIN) {
      flag_min = 1;
      num += 1;
      num = -num;
    } else
      num = -num;
  }
  int i = 0;
  long int num_copy = num;
  if ((num_copy == 0 && !specs->dot) ||
      (num_copy == 0 && !specs->precision && !specs->dot)) {
    char sym = num_copy % specs->num_sys + '0';
    str_to_num[i] = sym;
    i++;
    size_to_d--;
  }
  while (num_copy && str_to_num && size_to_d) {
    char sym;
    if (flag_min == 1) {
      sym = get_num_char(num_copy % specs->num_sys + 1, specs->up_case);
      flag_min = 0;
    } else {
      sym = get_num_char(num_copy % specs->num_sys, specs->up_case);
    }
    str_to_num[i] = sym;
    i++;
    size_to_d--;
    num_copy /= 10;
  }
  if (flag) num = -num;
  if (specs->precision - i > 0) {
    specs->precision -= i;
    specs->zero = 1;
  } else
    flag = 1;
  if (size_to_d == 1 && specs->zero == 1 && specs->flag_to_size == 1)
    specs->zero = 0;
  while (specs->zero && str_to_num && (size_to_d - specs->flag_to_size > 0) &&
         (specs->precision || flag)) {
    if (size_to_d == 1 && specs->flag_to_size == 1) break;
    str_to_num[i] = '0';
    i++;
    size_to_d--;
    specs->precision--;
  }
  check_plus_minus_space(num, specs, str_to_num, &i, &size_to_d);
  if (size_to_d > 0 && specs->minus == 0) {
    while ((size_to_d > 0) && str_to_num) {
      str_to_num[i] = ' ';
      i++;
      size_to_d--;
    }
  }
  return i;
}

void check_plus_minus_space(long int num, specifiers *specs, char *str_to_num,
                            int *i, s21_size_t *size_to_d) {
  if (specs->space && num >= 0 && *size_to_d) {
    str_to_num[*i] = ' ';
    *i += 1;
    *size_to_d -= 1;
  }
  if (num < 0 && *size_to_d) {
    str_to_num[*i] = '-';
    *i += 1;
    *size_to_d -= 1;
  }
  if (num >= 0 && specs->plus && *size_to_d) {
    str_to_num[*i] = '+';
    *i += 1;
    *size_to_d -= 1;
  }
}

char get_num_char(int num, int up_case) {
  char sym = '0';
  switch (num) {
    case 10:
      sym = (char)('a' - up_case * 32);
      break;
    case 11:
      sym = (char)('b' - up_case * 32);
      break;
    case 12:
      sym = (char)('c' - up_case * 32);
      break;
    case 13:
      sym = (char)('d' - up_case * 32);
      break;
    case 14:
      sym = (char)('e' - up_case * 32);
      break;
    case 15:
      sym = (char)('f' - up_case * 32);
      break;
  }
  if (0 <= num && num <= 9) sym = (char)(num + 48);
  return sym;
}

void set_specs_for_ge(specifiers *specs, const char *format) {
  if (*format == 'g' || *format == 'G')
    specs->g = 1;
  else if (*format == 'e' || *format == 'E')
    specs->e = 1;
  if (*format == 'G' || *format == 'E') specs->up_case = 1;
}

char *print_f(char *str, specifiers *specs, va_list *args) {
  long double num = 0;
  if (specs->lenght == 'L') {
    num = va_arg(*args, long double);
  } else
    num = va_arg(*args, double);
  // check size for buffer
  s21_size_t size_to_f = get_size_of_f(specs, num);
  char *str_to_double = calloc(size_to_f, sizeof(char));
  if (str_to_double) {
    int i = double_to_string(str_to_double, specs, num, size_to_f, 0);
    for (int j = i - 1; j >= 0; j--) {
      *str = str_to_double[j];
      str++;
    }
    while (i < specs->width) {
      *str = ' ';
      str++;
      i++;
    }
    free(str_to_double);
  }
  return str;
}

s21_size_t get_size_of_f(specifiers *specs, long double num) {
  s21_size_t result = 0;
  long double num_copy = num;
  if (!isnan(num) && !isinf(num)) {
    if (num_copy < 0) num_copy = -num_copy;
    // делим число на целую и дробную часть
    long double integer = 0;
    long double frac = modfl(num_copy, &integer);
    // если не указана точность, то по умолчанию 6
    if (!specs->dot && !specs->precision) {
      specs->precision = 6;
    }
    // точность = количество места для дробной части. И место для точки
    if (specs->precision) {
      result += specs->precision;
      result += 1;
    } else if (specs->hash)
      result += 1;
    if (frac + pow(0.1, specs->precision + 1) >= 1) {
      integer++;
    }
    integer = roundl(integer);
    // выделяем место для записи целой части в строку
    if (integer < 1) result++;
    while (integer >= 1) {
      integer /= 10;
      result++;
    }
  } else {
    result += 3;
    if (result < (s21_size_t)specs->width) result = specs->width;
  }
  // выделяем место для пробела, + или -
  if (specs->space || specs->plus || num < 0) {
    specs->flag_to_size = 1;
    result++;
  }
  // если после всех манипуляций места для записи меньше ширины
  if (specs->e) {
    if ((s21_size_t)specs->width > result + 4) result = specs->width - 4;
  } else if ((s21_size_t)specs->width > result)
    result = specs->width;
  return result;
}

char *print_eg(char *str, specifiers *specs, va_list *args) {
  long double num = 0;
  int e = 0;
  if (specs->lenght == 'L') {
    num = va_arg(*args, long double);
  } else
    num = va_arg(*args, double);
  s21_size_t size = 0;
  if (specs->g) cutter(specs, num);
  if (specs->e && !isnan(num) && !isinf(num)) {
    e = normalize(specs, &num);
    size += 4;
  }
  size += get_size_of_f(specs, num);
  char *str_to_double = calloc(size, sizeof(char));
  if (str_to_double) {
    int i = double_to_string(str_to_double, specs, num, size, e);
    for (int j = i - 1; j >= 0; j--) {
      *str = str_to_double[j];
      str++;
    }
    while (i < specs->width) {
      *str = ' ';
      str++;
      i++;
    }
    free(str_to_double);
  }
  return str;
}

void cutter(specifiers *specs, long double num) {
  long double num_copy = num;
  if (!isnan(num) && !isinf(num)) {
    int e = normalize(specs, &num_copy);
    if (specs->precision == 0 && !specs->dot) specs->precision = 6;
    if ((e <= 4 && specs->e == -1) || (specs->e == 1 && e < 6)) {
      specs->e = 0;
    }
  }
}

int normalize(specifiers *specs, long double *num) {
  int i = 0;
  if (fabsl(*num) > 1) {
    while (fabsl(*num) > 10) {
      *num /= 10;
      i++;
      specs->e = 1;
    }
  } else {
    while (fabsl(*num) < 1) {
      if (*num == 0) {
        specs->e = 1;
        break;
      }
      *num *= 10;
      i++;
      specs->e = -1;
    }
  }
  return i;
}

int double_to_string(char *str_to_double, specifiers *specs, long double num,
                     s21_size_t size, int e) {
  int i = nan_or_inf(str_to_double, num);
  if (!i) {
    if (specs->e) print_e(e, &size, str_to_double, specs, &i);
    int flag = 0;
    if (num < 0) {
      flag = 1;
      num = -num;
    }
    long double integer = 0;
    long double frac = modfl(num, &integer);
    int precision = specs->precision;
    long double frac_copy = frac;
    if (frac + pow(0.1, specs->precision + 1) >= 1) {
      frac = 0;
      integer++;
    }
    for (int j = 0; j < 15; j++) {
      frac_copy *= 10;
      if ((long)frac_copy != 0) break;
    }
    if ((long)frac_copy == 0 && specs->g) precision = 0;
    if (!precision && frac * 10 > 4) integer++;
    while (precision > 0) {
      frac *= 10;
      precision--;
    }
    if (fmodl(fabsl(frac) * 10, 10.0) > 4) frac = roundl(frac);
    if (specs->g) {
      while ((long)frac % 10 == 0 && specs->precision > 0 && (long)frac != 0) {
        frac /= 10;
        precision++;
      }
    }
    add_sym_from_double_to_str(str_to_double, specs, precision, &size, &i, frac,
                               integer);
    if (flag) num = -num;
    check_plus_minus_space_double(num, str_to_double, specs, &i, &size);
    if (size > 0 && specs->minus == 0) {
      while ((size > 0) && str_to_double) {
        if (specs->g && specs->width < specs->precision) break;
        str_to_double[i] = ' ';
        i++;
        size--;
      }
    }
  }
  return i;
}

void check_plus_minus_space_double(long double num, char *str_to_double,
                                   specifiers *specs, int *i,
                                   s21_size_t *size) {
  if (!isnan(num) && !isinf(num)) {
    int flag = 0;
    if (specs->width - *i > 0 && specs->zero) {
      specs->width -= *i;
      specs->zero = 1;
    } else
      flag = 1;
    if (*size == 1 && specs->zero == 1 && specs->flag_to_size == 1)
      specs->zero = 0;
    while (specs->zero && str_to_double && (*size - specs->flag_to_size > 0) &&
           (specs->width || flag)) {
      if (*size == 1 && specs->flag_to_size == 1) break;
      str_to_double[*i] = '0';
      *i += 1;
      *size -= 1;
      specs->width--;
    }
  }
  if (specs->space && num >= 0 && *size) {
    str_to_double[*i] = ' ';
    *i += 1;
    *size -= 1;
  }
  if (num < 0 && *size) {
    str_to_double[*i] = '-';
    *i += 1;
    *size -= 1;
  }
  if (num >= 0 && specs->plus && *size) {
    str_to_double[*i] = '+';
    *i += 1;
    *size -= 1;
  }
}

void add_sym_from_double_to_str(char *str_to_double, specifiers *specs,
                                int precision, s21_size_t *size, int *i,
                                long double frac, long double integer) {
  if (!precision) {
    long int frac_copy = (long int)frac;
    if (frac_copy < 1) {
      for (int j = 0; j < specs->precision; j++) {
        char sym = get_num_char(frac_copy % specs->num_sys, specs->up_case);
        str_to_double[*i] = sym;
        *i += 1;
        *size -= 1;
      }
    }
    while (frac_copy >= 1 && str_to_double && *size && specs->precision) {
      char sym = get_num_char(frac_copy % specs->num_sys, specs->up_case);
      str_to_double[*i] = sym;
      *i += 1;
      *size -= 1;
      frac_copy /= 10;
    }
    if (*i < specs->precision) {
      while (*i != specs->precision) {
        add_sym_to_str(str_to_double, i, '0');
        *size -= 1;
      }
    }
    if (specs->precision || specs->hash) {
      add_sym_to_str(str_to_double, i, '.');
      *size -= 1;
    }
  }
  int integer_copy = (int)integer;
  if (integer_copy < 1 && str_to_double && *size) {
    char sym = get_num_char(integer_copy % specs->num_sys, specs->up_case);
    str_to_double[*i] = sym;
    *i += 1;
    *size -= 1;
  }
  while (integer_copy >= 1 && str_to_double && *size) {
    char sym = get_num_char(integer_copy % specs->num_sys, specs->up_case);
    str_to_double[*i] = sym;
    *i += 1;
    *size -= 1;
    integer_copy /= 10;
  }
}

int nan_or_inf(char *str_to_double, long double num) {
  int i = 0;
  if (isnan(num)) {
    s21_memcpy(str_to_double, "nan", 3);
    i = 3;
  }
  if (isinf(num)) {
    s21_memcpy(str_to_double, "fni", 3);
    i = 3;
  }
  return i;
}

void print_e(int e, s21_size_t *size, char *str_to_double, specifiers *specs,
             int *i) {
  int e_copy = e;
  if (e_copy == 0) {
    *size -= 1;
    add_sym_to_str(str_to_double, i,
                   get_num_char(e_copy % specs->num_sys, specs->up_case));
  }
  while (e_copy) {
    *size -= 1;
    add_sym_to_str(str_to_double, i,
                   get_num_char(e_copy % specs->num_sys, specs->up_case));
    e_copy /= 10;
  }
  if (e < 10) {
    *size -= 1;
    add_sym_to_str(str_to_double + *i, i, '0');
  }
  if (specs->e == 1 || e == 0) {
    *size -= 1;
    add_sym_to_str(str_to_double, i, '+');
  } else {
    *size -= 1;
    add_sym_to_str(str_to_double, i, '-');
  }

  if (specs->up_case) {
    *size -= 1;
    add_sym_to_str(str_to_double, i, 'E');
  } else {
    *size -= 1;
    add_sym_to_str(str_to_double, i, 'e');
  }
}

void add_sym_to_str(char *str, int *i, char c) {
  s21_strncat(str, &c, 1);
  *i += 1;
}

char *print_s(char *str, specifiers *specs, va_list *args) {
  char *p = str;
  char *string = va_arg(*args, char *);
  if (string) {
    int tmp = specs->width;
    // check numbers of spaces
    if ((s21_size_t)specs->width < s21_strlen(string))
      specs->width = s21_strlen(string);
    int blank = specs->width - s21_strlen(string);
    if (!specs->precision && !specs->dot) specs->precision = specs->width;
    if (!specs->precision && specs->dot) blank = specs->width;
    if (specs->precision && specs->precision < tmp)
      blank = tmp - specs->precision;
    // print spaces
    while (blank && !specs->minus) {
      *str = ' ';
      str++;
      blank--;
    }
    while (*string != '\0') {
      if (!specs->precision) break;
      *str = *string;
      str++;
      string++;
      specs->precision--;
    }
    while (blank && specs->minus) {
      *str = ' ';
      str++;
      blank--;
    }
  } else {
    str = s21_memcpy(str, "(null)", 6);
    str += 6;
  }
  if (p) p = str;
  return p;
}

char *print_p(char *str, specifiers *specs, va_list *args) {
  unsigned long int ptr = (unsigned long int)va_arg(*args, unsigned long int);
  specs->num_sys = 16;
  specs->hash = 1;
  specs->up_case = 0;
  specs->zero = 0;
  // check size for buffer
  s21_size_t size_to_unsigned = get_size_of_unsigneg(specs, ptr);
  char *str_to_num = calloc(size_to_unsigned, sizeof(char));
  if (str_to_num) {
    int i = unsigned_to_string(str_to_num, specs, ptr, size_to_unsigned, 1);
    for (int j = i - 1; j >= 0; j--) {
      *str = str_to_num[j];
      str++;
    }
    while (i < specs->width) {
      *str = ' ';
      str++;
      i++;
    }
    free(str_to_num);
  }
  return str;
}

void set_num_sys(specifiers *specs, const char *format) {
  if (*format == 'o')
    specs->num_sys = 8;
  else if (*format == 'x' || *format == 'X')
    specs->num_sys = 16;
  if (*format == 'X') specs->up_case = 1;
}

char *print_u(char *str, specifiers *specs, va_list *args) {
  unsigned long int num = 0;
  if (specs->lenght == 'l') {
    num = (unsigned long int)va_arg(*args, unsigned long int);
  } else if (specs->lenght == 'h') {
    num = (unsigned short)va_arg(*args, unsigned int);
  } else
    num = (unsigned int)va_arg(*args, unsigned int);
  // check size for buffer
  s21_size_t size_to_unsigned = get_size_of_unsigneg(specs, num);
  char *str_to_num = calloc(size_to_unsigned, sizeof(char));
  if (str_to_num) {
    int i = unsigned_to_string(str_to_num, specs, num, size_to_unsigned, 0);
    for (int j = i - 1; j >= 0; j--) {
      *str = str_to_num[j];
      str++;
    }
    while (i < specs->width) {
      *str = ' ';
      str++;
      i++;
    }
    free(str_to_num);
  }
  return str;
}

s21_size_t get_size_of_unsigneg(specifiers *specs, unsigned long int num) {
  s21_size_t result = 0;
  unsigned long int num_copy = num;
  while (num_copy > 0) {
    num_copy /= specs->num_sys;
    result++;
  }
  if (result == 0 && (specs->precision || specs->width)) {
    result++;
  }
  int numbers = result;
  if ((s21_size_t)specs->width > result) result = specs->width;
  if ((s21_size_t)specs->precision > result) result = specs->precision;
  if (result == 0 && !specs->precision && !specs->width && !specs->space &&
      !specs->dot)
    result++;
  if (specs->num_sys == 8 && specs->hash && numbers > specs->width) {
    if (specs->precision >= specs->width) result += 1;
  } else if (specs->num_sys == 16 && specs->hash) {
    if (numbers > specs->width || specs->precision >= specs->width) result += 2;
  }
  return result;
}

int unsigned_to_string(char *str_to_num, specifiers *specs,
                       unsigned long int num, s21_size_t size_to_unsigned,
                       int flag_ptr) {
  int flag = 0;
  int i = 0;
  if ((specs->hash && specs->num_sys == 8 && num != 0))
    specs->flag_to_size = 1;
  else if ((specs->hash && specs->num_sys == 16 && num != 0))
    specs->flag_to_size = 2;
  unsigned long int num_copy = num;
  if ((num_copy == 0 && !specs->dot) ||
      (num_copy == 0 && !specs->precision && !specs->dot)) {
    char sym = num_copy % specs->num_sys + '0';
    str_to_num[i] = sym;
    i++;
    size_to_unsigned--;
  }
  while (num_copy && str_to_num && size_to_unsigned && specs->num_sys) {
    char sym = get_num_char(num_copy % specs->num_sys, specs->up_case);
    str_to_num[i] = sym;
    i++;
    size_to_unsigned--;
    num_copy /= specs->num_sys;
  }
  if (specs->precision - i > 0) {
    specs->precision -= i;
    specs->zero = 1;
  } else
    flag = 1;
  if (size_to_unsigned == 1 && specs->zero == 1 && specs->flag_to_size == 1)
    specs->zero = 0;
  while (specs->zero && str_to_num &&
         (size_to_unsigned - specs->flag_to_size > 0) &&
         (specs->precision || flag)) {
    if (size_to_unsigned == 1 && specs->flag_to_size == 1) break;
    str_to_num[i] = '0';
    i++;
    size_to_unsigned--;
    specs->precision--;
  }
  if (num != 0 || flag_ptr)
    check_hash(specs, str_to_num, &i, &size_to_unsigned);
  if (size_to_unsigned > 0 && specs->minus == 0) {
    while ((size_to_unsigned > 0) && str_to_num) {
      str_to_num[i] = ' ';
      i++;
      size_to_unsigned--;
    }
  }
  return i;
}

void check_hash(specifiers *specs, char *str_to_num, int *i,
                s21_size_t *size_to_unsigned) {
  if (specs->hash && specs->num_sys == 8) {
    str_to_num[*i] = '0';
    *i += 1;
    *size_to_unsigned -= 1;
  } else if (specs->hash && specs->num_sys == 16 && specs->up_case) {
    str_to_num[*i] = 'X';
    *i += 1;
    *size_to_unsigned -= 1;
    str_to_num[*i] = '0';
    *i += 1;
    *size_to_unsigned -= 1;
  } else if (specs->hash && specs->num_sys == 16 && !specs->up_case) {
    str_to_num[*i] = 'x';
    *i += 1;
    *size_to_unsigned -= 1;
    str_to_num[*i] = '0';
    *i += 1;
    *size_to_unsigned -= 1;
  }
}
