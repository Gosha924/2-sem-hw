#include "avl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void printTestResult(int testNum, const char* testName, int passed)
{
    printf("Тест %d: %s — %s\n", testNum, testName, passed ? "PASSED" : "FAILED");
}
// Создание и вставка первого узла
void test1()
{
    printf("\n=== ТЕСТ 1: Вставка первого узла ===\n");
    avlNode* root = NULL;
    root = insert(root, "JFK", "John F. Kennedy");

    int passed = (root != NULL) && (strcmp(root->iata, "JFK") == 0) && (strcmp(root->name, "John F. Kennedy") == 0) && (root->height == 1) && (root->left == NULL) && (root->right == NULL);

    printTestResult(1, "Вставка первого узла", passed);
    freeTree(root);
}

//  LL вращение (левое поддерево тяжелее)
void test2()
{
    printf("\n=== ТЕСТ 2: LL вращение ===\n");
    avlNode* root = NULL;
    root = insert(root, "SFO", "San Francisco");
    root = insert(root, "LAX", "Los Angeles");
    root = insert(root, "DEN", "Denver"); // Должно вызвать LL вращение

    // После вращения корнем должен стать LAX
    int passed = (strcmp(root->iata, "LAX") == 0) && (strcmp(root->left->iata, "DEN") == 0) && (strcmp(root->right->iata, "SFO") == 0) && isBalanced(root);

    printTestResult(2, "LL вращение", passed);
    freeTree(root);
}

// RR вращение (правое поддерево тяжелее)
void test3()
{
    printf("\n=== ТЕСТ 3: RR вращение ===\n");
    avlNode* root = NULL;
    root = insert(root, "DEN", "Denver");
    root = insert(root, "LAX", "Los Angeles");
    root = insert(root, "SFO", "San Francisco"); // Должно вызвать RR вращение

    // После вращения корнем должен стать LAX
    int passed = (strcmp(root->iata, "LAX") == 0) && (strcmp(root->left->iata, "DEN") == 0) && (strcmp(root->right->iata, "SFO") == 0) && isBalanced(root);

    printTestResult(3, "RR вращение", passed);
    freeTree(root);
}

// Лево-правое вращение
void test4()
{
    printf("\n=== ТЕСТ 4: LR вращение ===\n");
    avlNode* root = NULL;
    root = insert(root, "SFO", "San Francisco");
    root = insert(root, "DEN", "Denver");
    root = insert(root, "LAX", "Los Angeles"); // Должно вызвать LR вращение

    // После вращения корнем должен стать LAX
    int passed = (strcmp(root->iata, "LAX") == 0) && isBalanced(root);

    printTestResult(4, "LR вращение", passed);
    freeTree(root);
}

//  право-левое вращение
void test5()
{
    printf("\n=== ТЕСТ 5: RL вращение ===\n");
    avlNode* root = NULL;
    root = insert(root, "DEN", "Denver");
    root = insert(root, "SFO", "San Francisco");
    root = insert(root, "LAX", "Los Angeles"); // Должно вызвать RL вращение

    // После вращения корнем должен стать LAX
    int passed = (strcmp(root->iata, "LAX") == 0) && isBalanced(root);

    printTestResult(5, "RL вращение", passed);
    freeTree(root);
}

// Поиск
void test6()
{
    printf("\n=== ТЕСТ 6: Поиск ===\n");
    avlNode* root = NULL;
    root = insert(root, "JFK", "New York");
    root = insert(root, "LAX", "Los Angeles");
    root = insert(root, "SFO", "San Francisco");

    avlNode* found = search(root, "LAX");
    avlNode* notFound = search(root, "ORD");

    int passed = (found != NULL) && (strcmp(found->iata, "LAX") == 0) && (notFound == NULL);

    printTestResult(6, "Поиск ", passed);
    freeTree(root);
}

// Удаление листа
void test7()
{
    printf("\n=== ТЕСТ 7: Удаление листа ===\n");
    avlNode* root = NULL;
    root = insert(root, "JFK", "New York");
    root = insert(root, "LAX", "Los Angeles");
    root = insert(root, "SFO", "San Francisco");

    root = deleteNode(root, "SFO");

    int passed = (search(root, "SFO") == NULL) && (search(root, "JFK") != NULL) && (search(root, "LAX") != NULL) && (countNodes(root) == 2) && isBalanced(root);

    printTestResult(7, "Удаление листа", passed);
    freeTree(root);
}

// Тест 8: Удаление узла с двумя детьми
void test8()
{
    printf("\n=== ТЕСТ 8: Удаление узла с двумя детьми ===\n");
    avlNode* root = NULL;
    root = insert(root, "AAA", "Los Angeles");
    root = insert(root, "DDD", "New York");
    root = insert(root, "ZZZ", "San Francisco");
    root = deleteNode(root, "DDD");
    int passed = (search(root, "DDD") == NULL) && (countNodes(root) == 2) && isBalanced(root); // && isBST(root, NULL, NULL);

    printTestResult(8, "Удаление узла с двумя детьми", passed);
    freeTree(root);
}

//  Массовая вставка
void test9()
{
    printf("\n=== ТЕСТ 10: Массовая вставка ===\n");
    avlNode* root = NULL;

    root = insert(root, "JFK", "New York");
    root = insert(root, "LAX", "Los Angeles");
    root = insert(root, "SFO", "San Francisco");
    root = insert(root, "ORD", "Chicago");
    root = insert(root, "ATL", "Atlanta");
    root = insert(root, "DFW", "Dallas");
    root = insert(root, "DEN", "Denver");
    root = insert(root, "SEA", "Seattle");
    root = insert(root, "MIA", "Miami");
    root = insert(root, "BOS", "Boston");

    int passed = (countNodes(root) == 10) && isBalanced(root) && isBST(root, NULL, NULL);

    printTestResult(10, "Массовая вставка и баланс", passed);
    freeTree(root);
}

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();

    return 0;
}