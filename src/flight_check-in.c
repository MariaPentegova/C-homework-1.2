#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct Node {
    char *code;
    char *name;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

int height(Node *n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* create_node(const char *code, const char *name) {
    Node *node = malloc(sizeof(Node));
    node->code = strdup(code);
    node->name = strdup(name);
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

Node* rotate_right(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* rotate_left(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int get_balance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node* insert(Node *node, const char *code, const char *name) {
    if (!node)
        return create_node(code, name);

    int cmp = strcmp(code, node->code);

    if (cmp < 0)
        node->left = insert(node->left, code, name);
    else if (cmp > 0)
        node->right = insert(node->right, code, name);
    else {
        free(node->name);
        node->name = strdup(name);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // LL
    if (balance > 1 && strcmp(code, node->left->code) < 0)
        return rotate_right(node);

    // RR
    if (balance < -1 && strcmp(code, node->right->code) > 0)
        return rotate_left(node);

    // LR
    if (balance > 1 && strcmp(code, node->left->code) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // RL
    if (balance < -1 && strcmp(code, node->right->code) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

Node* find(Node *root, const char *code) {
    if (!root) return NULL;

    int cmp = strcmp(code, root->code);

    if (cmp == 0) return root;
    if (cmp < 0) return find(root->left, code);
    return find(root->right, code);
}

Node* min_value_node(Node *node) {
    Node *current = node;
    while (current->left)
        current = current->left;
    return current;
}

Node* delete_node(Node *root, const char *code) {
    if (!root) return root;

    int cmp = strcmp(code, root->code);

    if (cmp < 0)
        root->left = delete_node(root->left, code);
    else if (cmp > 0)
        root->right = delete_node(root->right, code);
    else {
        if (!root->left || !root->right) {
            Node *temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp->code);
            free(temp->name);
            free(temp);
        } else {
            Node *temp = min_value_node(root->right);

            free(root->code);
            free(root->name);

            root->code = strdup(temp->code);
            root->name = strdup(temp->name);

            root->right = delete_node(root->right, temp->code);
        }
    }

    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = get_balance(root);
  
    if (balance > 1 && get_balance(root->left) >= 0)
        return rotate_right(root);

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0)
        return rotate_left(root);

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

void save_inorder(Node *root, FILE *f, int *count) {
    if (!root) return;
    save_inorder(root->left, f, count);
    fprintf(f, "%s:%s\n", root->code, root->name);
    (*count)++;
    save_inorder(root->right, f, count);
}

Node* load_file(const char *filename, int *count) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Ошибка открытия файла");
        exit(1);
    }

    Node *root = NULL;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;

        *colon = '\0';
        char *code = line;
        char *name = colon + 1;

        name[strcspn(name, "\n")] = '\0';

        root = insert(root, code, name);
        (*count)++;
    }

    fclose(f);
    return root;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <airports.txt>\n", argv[0]);
        return 1;
    }

    int count = 0;
    Node *root = load_file(argv[1], &count);

    printf("Загружено %d аэропортов. Система готова к работе.\n", count);

    char input[MAX_LINE];

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, "find ", 5) == 0) {
            char *code = input + 5;
            Node *res = find(root, code);

            if (res)
                printf("%s → %s\n", code, res->name);
            else
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);

        } else if (strncmp(input, "add ", 4) == 0) {
            char *data = input + 4;
            char *colon = strchr(data, ':');

            if (!colon) {
                printf("Неверный формат.\n");
                continue;
            }

            *colon = '\0';
            char *code = data;
            char *name = colon + 1;

            root = insert(root, code, name);
            printf("Аэропорт '%s' добавлен в базу.\n", code);

        } else if (strncmp(input, "delete ", 7) == 0) {
            char *code = input + 7;
            if (find(root, code)) {
                root = delete_node(root, code);
                printf("Аэропорт '%s' удалён из базы.\n", code);
            } else {
                printf("Аэропорт '%s' не найден.\n", code);
            }

        } else if (strcmp(input, "save") == 0) {
            FILE *f = fopen(argv[1], "w");
            if (!f) {
                perror("Ошибка записи");
                continue;
            }

            int saved = 0;
            save_inorder(root, f, &saved);
            fclose(f);

            printf("База сохранена: %d аэропортов.\n", saved);

        } else if (strcmp(input, "quit") == 0) {
            break;

        } else {
            printf("Неизвестная команда.\n");
        }
    }

    return 0;
}
