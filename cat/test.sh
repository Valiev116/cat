#!/bin/bash

# Проверка количества аргументов
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <file_path> [flags]"
    exit 1
fi

# Путь к файлу
file_path="$1"
shift

# Запуск вашей программы s21_cat с заданными флагами и запись вывода в файл
./s21_cat "$@" "$file_path" > s21_cat_output.txt

# Запуск утилиты cat с теми же флагами и запись вывода в файл
cat "$@" "$file_path" > cat_output.txt

# Сравнение вывода программ
if diff -q s21_cat_output.txt cat_output.txt > /dev/null; then
    echo "Output of s21_cat is the same as cat"
else
	diff s21_cat_output.txt cat_output.txt
	echo "Output of s21_cat differs from cat"
fi

# Удаление временных файлов
rm s21_cat_output.txt cat_output.txt