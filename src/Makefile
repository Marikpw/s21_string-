UNAME := $(shell uname)
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g
SOURCE = $(wildcard s21_*.c)
OBJECTS = $(patsubst %.c,%.o, ${SOURCE})

ifeq ($(UNAME), Linux)
  	OPEN_CMD = xdg-open
else
	OPEN_CMD = open
endif

all: s21_string.a 

rebuild: clean s21_string.a 

s21_string.a:
	gcc $(CFLAGS) -c $(SOURCE) 
	ar rc $@ $(OBJECTS)
	ranlib $@
	-rm -rf *.o

ifeq ($(UNAME), Linux)
test: s21_string.a
	gcc $(CFLAGS) tests.c s21_string.a -lcheck -lm -pthread -lrt -lsubunit -o unit_tests
	chmod +x unit_tests
	./unit_tests
else
test: s21_string.a
	gcc $(CFLAGS) tests.c s21_string.a -lcheck -lm -pthread -o unit_tests
	chmod +x unit_tests
	./unit_tests
endif

clean:
	rm -rf *.gcno *.o *.a *.gcda *.gcov *.exec *.info *.css *.html unit_tests gcov_report output_valgrind.txt ./report
	
gcov_report: gcov_test
	lcov -t "./gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	$(OPEN_CMD) ./report/index.html
	

ifeq ($(UNAME), Linux)
gcov_test: gcov_s21_string.a
	gcc $(CFLAGS) tests.c gcov_s21_string.a -lcheck -lm -pthread -lrt -lsubunit --coverage -o gcov_report
	chmod +x gcov_report
	./gcov_report
else
gcov_test: gcov_s21_string.a
	gcc $(CFLAGS) tests.c gcov_s21_string.a -lcheck -lm -pthread --coverage -o gcov_report
	chmod +x gcov_report
	./gcov_report
endif

gcov_s21_string.a:
	gcc $(CFLAGS) --coverage -c $(SOURCE) 
	ar rc $@ $(OBJECTS)
	ranlib $@
	-rm -rf *.o

check:
	clang-format -style=Google -n *.c *.h

clang:
	clang-format -style=Google -i *.c *.h

ifeq ($(UNAME), Linux)
leaks: s21_string.a
	gcc $(CFLAGS) tests.c s21_string.a -lcheck -lm -pthread -lrt -lsubunit -o unit_tests
	chmod +x unit_tests
	CK_FORK=no valgrind --tool=memcheck --leak-check=yes --log-file=output_valgrind.txt ./unit_tests
else
leaks: s21_string.a
	gcc $(CFLAGS) tests.c s21_string.a -lcheck -lm -pthread -o unit_tests
	chmod +x unit_tests
	CK_FORK=no leaks -atExit -- ./unit_tests
endif