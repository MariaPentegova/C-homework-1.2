#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *table[100][100]; 
int rows = 0, cols = 0;

int main() {
    FILE *in = fopen("input.csv", "r");
    if (!in) { 
        printf("Нет файла input.csv\n"); 
        return 1; 
    }

    char line[1024];
    int max_w = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\r\n")] = 0; 
        
        int c = 0;
        char *token = strtok(line, ",");
        while (token) {
            table[rows][c] = strdup(token); 
            int len = strlen(token);
            if (len > max_w) {
                max_w = len;   
                }
            
            token = strtok(NULL, ",");
            c++;
        }
        if (rows == 0) {
            cols = c;
            }
        rows++;
    }
    fclose(in);

    FILE *out = fopen("output.txt", "w");

    for (int r = 0; r < rows; r++) {
        char fill = (r <= 1) ? '=' : '-';
        fprintf(out, "+");
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < max_w + 2; j++) {
                fputc(fill, out);
                }
            fprintf(out, "+");
            }
        fprintf(out, "\n");

        for (int c = 0; c < cols; c++) {
            if (atof(table[r][c]) != 0 || strcmp(table[r][c], "0") == 0) {
                fprintf(out, "| %*s ", max_w, table[r][c]);
            } 
            else {
                fprintf(out, "| %-*s ", max_w, table[r][c]);
            }
        }
        fprintf(out, "|\n");
    }

    fprintf(out, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < max_w + 2; j++) {
            fputc('-', out);
            }
        fprintf(out, "+");
        }
    fprintf(out, "\n");

    fclose(out);
    printf("Таблица готова в output.txt!\n");
    return 0;
}
