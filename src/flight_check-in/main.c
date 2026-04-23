//без бонусного

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVLNode {
    char code[4];        // IATA код
    char *name;          // Название аэропорта
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Вспомогательные функции
int max(int a, int b) { return (a > b) ? a : b; }

int height(AVLNode *node) { return node ? node->height : 0; }

AVLNode* createNode(const char *code, const char *name) {
    AVLNode *node = malloc(sizeof(AVLNode));
    strcpy(node->code, code);
    node->name = strdup(name);
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Повороты
AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Вставка
AVLNode* insert(AVLNode* node, const char *code, const char *name) {
    if (!node) return createNode(code, name);

    int cmp = strcmp(code, node->code);
    if (cmp < 0) node->left = insert(node->left, code, name);
    else if (cmp > 0) node->right = insert(node->right, code, name);
    else {
        free(node->name);
        node->name = strdup(name);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Балансировка
    if (balance > 1 && strcmp(code, node->left->code) < 0) return rightRotate(node);
    if (balance < -1 && strcmp(code, node->right->code) > 0) return leftRotate(node);
    if (balance > 1 && strcmp(code, node->left->code) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(code, node->right->code) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Поиск
AVLNode* find(AVLNode *node, const char *code) {
    if (!node) return NULL;
    int cmp = strcmp(code, node->code);
    if (cmp == 0) return node;
    if (cmp < 0) return find(node->left, code);
    else return find(node->right, code);
}

// Удаление
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left) current = current->left;
    return current;
}

AVLNode* deleteNode(AVLNode* root, const char *code) {
    if (!root) return root;
    int cmp = strcmp(code, root->code);
    if (cmp < 0) root->left = deleteNode(root->left, code);
    else if (cmp > 0) root->right = deleteNode(root->right, code);
    else {
        if (!root->left || !root->right) {
            AVLNode *temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else *root = *temp;
            free(temp->name);
            free(temp);
        } else {
            AVLNode *temp = minValueNode(root->right);
            strcpy(root->code, temp->code);
            free(root->name);
            root->name = strdup(temp->name);
            root->right = deleteNode(root->right, temp->code);
        }
    }
    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Балансировка
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Сохранение в файл
void saveToFile(AVLNode *node, FILE *fout) {
    if (!node) return;
    saveToFile(node->left, fout);
    fprintf(fout, "%s:%s\n", node->code, node->name);
    saveToFile(node->right, fout);
}

// Освобождение памяти
void freeTree(AVLNode *node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node->name);
    free(node);
}

// Загрузка из файла
AVLNode* loadFromFile(const char *filename, int *count) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    AVLNode *root = NULL;
    char line[1024];
    *count = 0;
    while (fgets(line, sizeof(line), fin)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = 0;
        char *code = line;
        char *name = colon + 1;
        name[strcspn(name, "\r\n")] = 0; // удаляем перевод строки
        root = insert(root, code, name);
        (*count)++;
    }
    fclose(fin);
    return root;
}

// CLI
void runCLI(AVLNode **root, const char *filename) {
    char command[1024];
    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command, "\r\n")] = 0;

        if (strncmp(command, "find ", 5) == 0) {
            char *code = command + 5;
            AVLNode *node = find(*root, code);
            if (node) printf("%s → %s\n", code, node->name);
            else printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
        } else if (strncmp(command, "add ", 4) == 0) {
            char *data = command + 4;
            char *colon = strchr(data, ':');
            if (!colon) { printf("Неверный формат добавления\n"); continue; }
            *colon = 0;
            char *code = data;
            char *name = colon + 1;
            *root = insert(*root, code, name);
            printf("Аэропорт '%s' добавлен в базу.\n", code);
        } else if (strncmp(command, "delete ", 7) == 0) {
            char *code = command + 7;
            *root = deleteNode(*root, code);
            printf("Аэропорт '%s' удалён из базы.\n", code);
        } else if (strcmp(command, "save") == 0) {
            FILE *fout = fopen(filename, "w");
            if (!fout) { printf("Ошибка сохранения файла\n"); continue; }
            saveToFile(*root, fout);
            fclose(fout);
            printf("База сохранена.\n");
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Неизвестная команда\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s airports.txt\n", argv[0]);
        return 1;
    }
    int count = 0;
    AVLNode *root = loadFromFile(argv[1], &count);
    if (!root) return 1;
    printf("Загружено %d аэропортов. Система готова к работе.\n", count);
    runCLI(&root, argv[1]);
    freeTree(root);
    return 0;
}

// бонусное

typedef struct ListNode {
    char code[4];
    char *name;
    struct ListNode *next;
} ListNode;

// Создать узел
ListNode* createNode(const char *code, const char *name) {
    ListNode *node = malloc(sizeof(ListNode));
    strcpy(node->code, code);
    node->name = strdup(name);
    node->next = NULL;
    return node;
}

// Найти
ListNode* find(ListNode *head, const char *code) {
    while (head) {
        if (strcmp(head->code, code) == 0) return head;
        head = head->next;
    }
    return NULL;
}

// Добавить или обновить
ListNode* add(ListNode *head, const char *code, const char *name) {
    ListNode *node = find(head, code);
    if (node) {
        free(node->name);
        node->name = strdup(name);
        return head;
    }
    ListNode *newNode = createNode(code, name);
    newNode->next = head;
    return newNode;
}

// Удалить
ListNode* deleteNode(ListNode *head, const char *code) {
    ListNode *prev = NULL;
    ListNode *cur = head;
    while (cur) {
        if (strcmp(cur->code, code) == 0) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            free(cur->name);
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

// Сохранить в файл
void saveToFile(ListNode *head, FILE *fout) {
    while (head) {
        fprintf(fout, "%s:%s\n", head->code, head->name);
        head = head->next;
    }
}

// Освободить память
void freeList(ListNode *head) {
    while (head) {
        ListNode *tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp);
    }
}

// Загрузка из файла
ListNode* loadFromFile(const char *filename, int *count) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    ListNode *head = NULL;
    char line[1024];
    *count = 0;
    while (fgets(line, sizeof(line), fin)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = 0;
        char *code = line;
        char *name = colon + 1;
        name[strcspn(name, "\r\n")] = 0;
        head = add(head, code, name);
        (*count)++;
    }
    fclose(fin);
    return head;
}

// CLI
void runCLI(ListNode **head, const char *filename) {
    char command[1024];
    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command, "\r\n")] = 0;

        if (strncmp(command, "find ", 5) == 0) {
            char *code = command + 5;
            ListNode *node = find(*head, code);
            if (node) printf("%s → %s\n", code, node->name);
            else printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
        } else if (strncmp(command, "add ", 4) == 0) {
            char *data = command + 4;
            char *colon = strchr(data, ':');
            if (!colon) { printf("Неверный формат добавления\n"); continue; }
            *colon = 0;
            char *code = data;
            char *name = colon + 1;
            *head = add(*head, code, name);
            printf("Аэропорт '%s' добавлен в базу.\n", code);
        } else if (strncmp(command, "delete ", 7) == 0) {
            char *code = command + 7;
            *head = deleteNode(*head, code);
            printf("Аэропорт '%s' удалён из базы.\n", code);
        } else if (strcmp(command, "save") == 0) {
            FILE *fout = fopen(filename, "w");
            if (!fout) { printf("Ошибка сохранения файла\n"); continue; }
            saveToFile(*head, fout);
            fclose(fout);
            printf("База сохранена.\n");
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Неизвестная команда\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s airports.txt\n", argv[0]);
        return 1;
    }
    int count = 0;
    ListNode *head = loadFromFile(argv[1], &count);
    if (!head) return 1;
    printf("Загружено %d аэропортов. Система готова к работе.\n", count);
    runCLI(&head, argv[1]);
    freeList(head);
    return 0;
}
