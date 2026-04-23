#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ***data;
    int rows;
    int cols;
    int max_width;
} Table;

void free_table(Table *t) {
    for (int r = 0; r < t->rows; r++) {
        for (int c = 0; c < t->cols; c++) {
            free(t->data[r][c]);
        }
        free(t->data[r]);
    }
    free(t->data);
}

// Функция для чтения таблицы из файла
int read_csv(const char *filename, Table *t) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        printf("Нет файла %s\n", filename);
        return -1;
    }

    t->rows = 0;
    t->cols = 0;
    t->max_width = 0;

    size_t line_size = 0;
    char *line = NULL;

    // Выделение памяти для указателей
    size_t capacity = 10; // начальная емкость
    t->data = malloc(capacity * sizeof(char **));

    while (getline(&line, &line_size, in) != -1) {
        line[strcspn(line, "\r\n")] = 0; // удаление символов переноса строки
        if ((size_t)t->rows >= capacity) {
            capacity *= 2;
            t->data = realloc(t->data, capacity * sizeof(char **));
        }

        t->data[t->rows] = NULL;
        int c = 0;
        size_t col_capacity = 10;
        t->data[t->rows] = malloc(col_capacity * sizeof(char *));
        char *token = strtok(line, ",");
        while (token) {
            if ((size_t)c >= col_capacity) {
                col_capacity *= 2;
                t->data[t->rows] = realloc(t->data[t->rows], col_capacity * sizeof(char *));
            }
            t->data[t->rows][c] = strdup(token);
            int len = strlen(token);
            if (len > t->max_width) t->max_width = len;
            token = strtok(NULL, ",");
            c++;
        }
        if (c > t->cols) t->cols = c;
        t->rows++;
    }

    free(line);
    fclose(in);
    return 0;
}

// Функция для вывода таблицы в файл
void write_table(const char *filename, const Table *t) {
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Не удалось открыть файл для записи.\n");
        return;
    }

    for (int r = 0; r < t->rows; r++) {
        char fill = (r <= 1) ? '=' : '-';
        fprintf(out, "+");
        for (int i = 0; i < t->cols; i++) {
            for (int j = 0; j < t->max_width + 2; j++) {
                fputc(fill, out);
            }
            fprintf(out, "+");
        }
        fprintf(out, "\n");

        for (int c = 0; c < t->cols; c++) {
            const char *cell = (c < t->rows ? t->data[r][c] : "");
            if (atof(cell) != 0 || strcmp(cell, "0") == 0) {
                fprintf(out, "| %*s ", t->max_width, cell);
            } else {
                fprintf(out, "| %-*s ", t->max_width, cell);
            }
        }
        fprintf(out, "|\n");
    }

    // Создаем нижнюю рамку
    fprintf(out, "+");
    for (int i = 0; i < t->cols; i++) {
        for (int j = 0; j < t->max_width + 2; j++) {
            fputc('-', out);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");

    fclose(out);
}

int main() {
    Table table = {0};
    if (read_csv("input.csv", &table) != 0) {
        return 1; // ошибка чтения
    }

    write_table("output.txt", &table);
    printf("Таблица готова в output.txt!\n");

    free_table(&table);
    return 0;
}
