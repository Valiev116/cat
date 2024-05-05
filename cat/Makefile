CC = gcc
FLAG = -Wall -Wextra -Werror -pedantic -Ofast -std=c11
CFILE = s21_cat.c
FNAME = s21_cat

all: s21_cat

s21_cat: s21_cat.o catFlags.o
	$(CC) $(FLAG) s21_cat.o catFlags.o -o $(FNAME)
	rm -f s21_cat.o catFlags.o

s21_cat.o: s21_cat.c
	$(CC) $(FLAG) -IIncludes -c $(CFILE) -o s21_cat.o

catFlags.o: catFlags.c
	$(CC) $(FLAG) -IIncludes -c catFlags.c -o catFlags.o

rebuild: clean all

clean:
	rm -f $(FNAME)