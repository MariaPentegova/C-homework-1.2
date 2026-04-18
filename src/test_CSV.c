#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void); 

// Вспомогательная функция для чтения всего файла в строку
char *read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    assert(f != NULL);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    assert(buffer != NULL);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

// Тест 1: простая таблица
void test_simple_csv() {
    FILE *f = fopen("input.csv", "w");
    assert(f != NULL);
    fprintf(f, "Name,Value\nAlice,10\nBob,20\n");
    fclose(f);

    main(); // вызов вашей программы

    char *output = read_file("output.txt");
    const char *expected =
        "+====+======+\n"
        "|Name|Value |\n"
        "+====+======+\n"
        "|Alice|    10 |\n"
        "+----+------+\n"
        "|Bob  |    20 |\n"
        "+----+------+\n";

    // Проверка наличия ключевых подстрок
    assert(strstr(output, "Alice") != NULL);
    assert(strstr(output, "Bob") != NULL);
    assert(strstr(output, "Value") != NULL);

    free(output);
}

// Тест 2: числа с плавающей точкой и текст
void test_mixed_csv() {
    FILE *f = fopen("input.csv", "w");
    assert(f != NULL);
    fprintf(f, "Item,Price\nBook,12.5\nPen,1.0\nNote,0\n");
    fclose(f);

    main();

    char *output = read_file("output.txt");
    assert(strstr(output, "12.5") != NULL);
    assert(strstr(output, "1.0") != NULL);
    assert(strstr(output, "Note") != NULL);

    free(output);
}

int main_tests() {
    test_simple_csv();
    test_mixed_csv();
    printf("Все тесты пройдены!\n");
    return 0;
}
