// бонусное
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
