#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура узла дерева
typedef struct Node {
    char code[10];       
    char name[128];      
    struct Node *left;   
    struct Node *right; 
} Node;

// Функция создания нового узла
Node* create_node(char* code, char* name) {
    Node* newNode = malloc(sizeof(Node));
    strcpy(newNode->code, code);
    strcpy(newNode->name, name);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Добавление в дерево (рекурсивно)
Node* insert(Node* root, char* code, char* name) {
    if (root == NULL){
        return create_node(code, name);
    }

    int cmp = strcmp(code, root->code);
    if (cmp < 0) {
        root->left = insert(root->left, code, name);
    }
    else if (cmp > 0) {
        root->right = insert(root->right, code, name);
    }
    
    return root;
}

// Поиск по коду
Node* find(Node* root, char* code) {
    if (root == NULL || strcmp(root->code, code) == 0) {
        return root;
    }
    
    if (strcmp(code, root->code) < 0) {
        return find(root->left, code);
    }
    return find(root->right, code);
}

// Сохранение дерева в файл (обход In-order)
void save_to_file(Node* root, FILE* f) {
    if (root != NULL) {
        save_to_file(root->left, f);
        fprintf(f, "%s:%s\n", root->code, root->name);
        save_to_file(root->right, f);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) { 
        printf("Использование: ./aerosoft airports.txt\n"); 
        return 1; 
    }

    Node* root = NULL;
    char line[256], cmd[20], arg[256];
    int count = 0;

    FILE* f = fopen(argv[1], "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char *c = strtok(line, ":");
            char *n = strtok(NULL, "\n");
            if (c && n) {
                root = insert(root, c, n);
                count++;
            }
        }
        fclose(f);
    }
    printf("Загружено %d аэропортов.\n", count);

    while (1) {
        printf("\n> ");
        scanf("%s", cmd);

        if (strcmp(cmd, "find") == 0) {
            scanf("%s", arg);
            Node* res = find(root, arg);
            if (res) {
                printf("%s -> %s\n", res->code, res->name);
            }
            else {
                printf("Не найдено.\n");
            }

        } 
        else if (strcmp(cmd, "add") == 0) {
            scanf(" %[^\n]", arg); // Считать строку с пробелами
            char *c = strtok(arg, ":");
            char *n = strtok(NULL, "");
            if (c && n) {
                root = insert(root, c, n);
                printf("Добавлено.\n");
            }

        } 
        else if (strcmp(cmd, "save") == 0) {
            f = fopen(argv[1], "w");
            save_to_file(root, f);
            fclose(f);
            printf("Сохранено.\n");

        } 
        else if (strcmp(cmd, "quit") == 0) {
            break;
        }
    }

    return 0;
}
