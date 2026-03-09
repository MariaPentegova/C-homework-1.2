#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COLUMNS 50
#define MAX_ROWS 100

typedef struct {
    char* data[MAX_ROWS][MAX_COLUMNS];
    int rows;
    int cols;
    int col_widths[MAX_COLUMNS];
    int is_numeric[MAX_COLUMNS]; // 1 - число, 0 - строка
} Table;

// если вернёт 1 - число, 0 - строка
int is_Number(const char *str) {
    if (*str == '\0') {
        return 0;
        }
    const char *p = str;
    if (*p == '+' || *p == '-') {
        p++;
        }
    int hasDigits = 0, hasDot = 0;
    while (*p) {
        if (isdigit(*p)) {
            hasDigits = 1;
            } 
        else if (*p == '.') {
            if (hasDot){
                return 0; 
                }
            hasDot = 1;
            } 
        else {
            return 0;
            }
        p++;
    }
    return hasDigits;
}

int read_csv(const char *filename, Table *table) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return -1;  
        }

    char line[MAX_LINE_LENGTH];
    int row = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0; // удаляем переносы строк
        int col = 0;
        char *ptr = line;

        while (*ptr) {
            char *start = ptr;
            char *comma = strchr(start, ',');
            size_t len = comma ? (size_t)(comma - start) : strlen(start);

            if (row >= MAX_ROWS || col >= MAX_COLUMNS) {
                printf("Превышение допустимых размеров таблицы.\n");
                fclose(f);
                return -1;
                }

            table->data[row][col] = malloc(len + 1);
            strncpy(table->data[row][col], start, len);
            table->data[row][col][len] = '\0';

            col++;
            if (comma) {
                ptr = comma + 1;
                } 
            else {
                break;
                }
            }

        if (row == 0) {
            table->cols = col; 
            } 
        else if (col != table->cols) {
            printf("Row %d имеет разное число столбцов.\n", row + 1);
            //избегание утечки памяти
            for (int r = 0; r <= row; r++) {
                for (int c = 0; c < table->cols; c++) {
                    free(table->data[r][c]);
                    }
                }
            fclose(f);
            return -1;
            }

        row++;
        if (row >= MAX_ROWS) {
            printf("Превышено максимальное число строк.\n");
            break;
            }
        }

    table->rows = row;
    fclose(f);
    return 0; 
    }

void analyze_table(Table *table) {
    for (int col=0; col<table->cols; ++col) {
        int max_width = 0; 
        int numeric = 1; // допустим, что все числа
        for (int row=0; row<table->rows; ++row) {
            int len = strlen(table->data[row][col]);
            if (len > max_width) {
                max_width = len;
                }
            if (!is_Number(table->data[row][col])) {
                numeric = 0;
                }
            }
        table->col_widths[col] = max_width;
        table->is_numeric[col] = numeric;
        }
    }

void print_line(FILE *out, int * widths, int cols, char const * left, char const * mid, char const * right) {
    fputs(left, out);
    for (int i=0; i<cols; ++i) {
        for (int j=0; j< widths[i]+2; ++j) {
            fputc('=', *mid== '=' || *mid=='-' ? *mid : '-');
            }
        if (i<cols-1) {
            fputs(mid, out);
            }
        }
    fputs(right, out);
    fputc('\n', out);
    }

void print_table(Table *table, const char *filename) {
    FILE *out = fopen(filename, "w");
    if (!out) { 
        perror("fopen output"); 
        exit(1); 
        }

    int *w = table->col_widths;
    int c = table->cols;

    // верхняя линия
    print_line(out, w, c, "+", "=", "+");
    // заголовки 
    for (int col=0; col<c; ++col) {
        printf(out, "| %-*s ", w[col], table->data[0][col]);
        }
    fputs("|\n", out);
    // разделитель
    print_line(out, w, c, "+", "=", "+");
    // данные
    for (int row=1; row<table->rows; ++row) {
        for (int col=0; col<c; ++col) {
            if (table->is_numeric[col]) {
                // выравнивание вправо
                printf(out, "| %*s ", w[col], table->data[row][col]);
                } 
            else {
                // влево
                printf(out, "| %-*s ", w[col], table->data[row][col]);
                }
            }
        fputs("|\n", out);
        // ниже каждой строки — разделитель
        if (row == table->rows - 1 || 1) { 
            print_line(out, w, c, "+", "-", "+");
            }
        }
    fclose(out);
    }

void free_table(Table *table) {
    for (int row=0; row<table->rows; ++row) {
        for (int col=0; col<table->cols; ++col) {
            free(table->data[row][col]);
            }
        }
    }

int main() {
    Table table;
    memset(&table, 0, sizeof(table)); //все значения внутри структуры = 0
    read_csv("input.csv", &table);
    analyze_table(&table);
    print_table(&table, "output.txt");
    free_table(&table);
    return 0;
    }   
