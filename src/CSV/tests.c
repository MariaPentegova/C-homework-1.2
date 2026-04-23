#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "main.c" 

// Вспомогательная функция для сравнения содержимого файла с ожидаемым выводом
void check_full_output(const char *filename, const char *expected_output) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Не удалось открыть файл %s для проверки\n", filename);
        return;
    }
    char buffer[4096] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, f);
    fclose(f);
    assert(strcmp(buffer, expected_output) == 0 && "Вывод таблицы не соответствует ожидаемому");
}

void test_boundary_cases() {
    // Тест 1: минимальные значения, пустые поля
    FILE *f = fopen("test_input1.csv", "w");
    fprintf(f, "ID,Name,Score\n");
    fprintf(f, "1,,\n");
    fprintf(f, "0,Empty,0\n");
    fclose(f);

    Table t = {0};
    read_csv("test_input1.csv", &t);
    write_table("test_output1.txt", &t);

    const char *expected_output1 =
"+=======+=======+=======+\n"
"| ID    | Name  | Score |\n"
"+=======+=======+=======+\n"
"|     1 |       |       |\n"
"+-------+-------+-------+\n"
"|     0 | Empty |     0 |\n"
"+-------+-------+-------+\n";

    check_full_output("test_output1.txt", expected_output1);
    free_table(&t);
    printf("Граничный случай 1 пройден!\n");

    // Тест 2: очень длинные строки/числа, чтобы проверить ширины
    f = fopen("test_input2.csv", "w");
    fprintf(f, "ID,Name,Score\n");
    fprintf(f, "123456789,VeryLongNameIndeed,98765\n");
    fclose(f);

    read_csv("test_input2.csv", &t);
    write_table("test_output2.txt", &t);

    const char *expected_output2 =
"+===========+====================+=======+\n"
"| ID        | Name               | Score |\n"
"+===========+====================+=======+\n"
"| 123456789 | VeryLongNameIndeed |   765 |\n"
"+-----------+--------------------+-------+\n";

    check_full_output("test_output2.txt", expected_output2);
    free_table(&t);
    printf("Граничный случай 2 пройден!\n");

    // Тест 3: числа с отрицательными значениями, нулями
    f = fopen("test_input3.csv", "w");
    fprintf(f, "ID,Name,Score\n");
    fprintf(f, "-1,Negative, -999\n");
    fprintf(f, "0,Zero,0\n");
    fclose(f);

    read_csv("test_input3.csv", &t);
    write_table("test_output3.txt", &t);

    const char *expected_output3 =
"+=======+==========+=======+\n"
"| ID    | Name     | Score |\n"
"+=======+==========+=======+\n"
"|    -1 | Negative |  -999 |\n"
"+-------+----------+-------+\n"
"|     0 | Zero     |     0 |\n"
"+-------+----------+-------+\n";

    check_full_output("test_output3.txt", expected_output3);
    free_table(&t);
    printf("Граничный случай 3 пройден!\n");
}

int main() {
    printf("Запуск тестов с граничными случаями...\n");
    test_boundary_cases();
    printf("Все тесты успешно пройдены!\n");
    return 0;
}
