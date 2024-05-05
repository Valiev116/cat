CC = gcc
FLAG = -Wall -Wextra -Werror -pedantic -Ofast -std=c11
CFILE = s21_cat.c
FNAME = s21_grep

all: s21_grep

s21_grep: s21_grep.c
	$(CC) $(FLAG) s21_grep.c -o $(FNAME)

rebuild: clean all

clean:
	rm -f $(FNAME)