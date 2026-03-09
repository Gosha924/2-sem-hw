#include "avl.h"
#include <stdio.h>
#include <string.h>

void saveNodeInFile(FILE* file, avlNode* node)
{
    if (node == NULL) {
        return;
    }
    saveNodeInFile(file, node->left);
    fprintf(file, "%s:%s\n", node->iata, node->name);
    saveNodeInFile(file, node->right);
}

int saveTreeInFile(char* filename, avlNode* root)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("File could not be opened\n");
        return 0;
    }
    saveNodeInFile(file, root);
    fclose(file);
    return 1;
}

int main(int arg, char* argv[])
{
    if (arg < 2) {
        printf("Input file is not found\n");
        return 1;
    }
    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    avlNode* tree = NULL;
    char line[200];
    int lineNum = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) {
            continue;
        }
        char* iata = strtok(line, ":");
        char* name = strtok(NULL, ":");
        if (iata && name) {
            tree = insert(tree, iata, name);
        }
    }
    fclose(file);
    printf("Загружено %d аэропортов. Система готова к работе.\n", lineNum);

    char command[10];
    char iata[50];
    while (1) {
        scanf("%s", command);
        fgets(iata, sizeof(iata), stdin);
        iata[strcspn(iata, "\n")] = 0;
        if (iata[0] == ' ') {
            memmove(iata, iata + 1, strlen(iata));
        }
        if (strcmp(command, "find") == 0) {
            avlNode* needNode = search(tree, iata);
            if (needNode) {
                printf("%s -> %s\n", needNode->iata, needNode->name);
            } else {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", iata);
            }
        }
        if (strcmp(command, "add") == 0) {
            char* tokenIata = strtok(iata, ":");
            char* tokenName = strtok(NULL, ":");
            tree = insert(tree, tokenIata, tokenName);
            printf("Аэропорт '%s' добавлен в базу.\n", tokenIata);
            lineNum++;
        }
        if (strcmp(command, "delete") == 0) {
            tree = deleteNode(tree, iata);
            printf("Аэропорт '%s' удалён из базы.\n", iata);
            lineNum--;
        }
        if (strcmp(command, "save") == 0) {
            int result = saveTreeInFile(filename, tree);
            if (result) {
                printf("База сохранена: %d аэропортов.\n", lineNum);
            } else {
                printf("File could not be opened\n");
            }
        }
        if (strcmp(command, "quit") == 0) {
            freeTree(tree);
            break;
        }
    }
    return 0;
}
