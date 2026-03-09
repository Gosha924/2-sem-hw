#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getHeight(avlNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

avlNode* createNewNode(const char* iata, const char* name)
{
    avlNode* newNode = (avlNode*)malloc(sizeof(avlNode));
    strncpy(newNode->iata, iata, 3);
    newNode->iata[3] = '\0';

    newNode->name = (char*)malloc(strlen(name) + 1);
    strcpy(newNode->name, name);

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

void updateHeight(avlNode* node)
{
    if (node == NULL) {
        return;
    }
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int getBalance(avlNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

avlNode* rightRotation(avlNode* node)
{
    avlNode* newRoot = node->left;
    avlNode* downNode = newRoot->right;
    newRoot->right = node;
    node->left = downNode;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

avlNode* leftRotation(avlNode* node)
{
    avlNode* newRoot = node->right;
    avlNode* downNode = newRoot->left;

    newRoot->left = node;
    node->right = downNode;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

avlNode* leftRightRotation(avlNode* node)
{
    node->left = leftRotation(node->left);
    return rightRotation(node);
}

avlNode* rightLeftRotation(avlNode* node)
{
    node->right = rightRotation(node->right);
    return leftRotation(node);
}

avlNode* balanceNode(avlNode* node)
{
    if (node == NULL) {
        return NULL;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1) {
        if (getBalance(node->left) >= 0) {
            return rightRotation(node);
        } else {
            return leftRightRotation(node);
        }
    }
    if (balance < -1) {
        if (getBalance(node->right) <= 0) {
            return leftRotation(node);
        } else {
            return rightLeftRotation(node);
        }
    }
    return node;
}

avlNode* insert(avlNode* node, const char* iata, const char* name)
{
    if (node == NULL) {
        return createNewNode(iata, name);
    }
    int cmp = strcmp(iata, node->iata);
    if (cmp < 0) {
        node->left = insert(node->left, iata, name);
    } else if (cmp > 0) {
        node->right = insert(node->right, iata, name);
    }
    return balanceNode(node);
}

void inOrder(avlNode* node)
{
    if (node == NULL) {
        return;
    }
    inOrder(node->left);
    printf("%s %s, ", node->iata, node->name);
    inOrder(node->right);
}

void preOrder(avlNode* node)
{
    if (node == NULL) {
        return;
    }
    printf("%s %s, ", node->iata, node->name);
    preOrder(node->left);
    preOrder(node->right);
}

void postOrder(avlNode* node)
{
    if (node == NULL) {
        return;
    }
    postOrder(node->left);
    postOrder(node->right);
    printf("%s %s, ", node->iata, node->name);
}

avlNode* findMin(avlNode* node)
{
    if (node == NULL) {
        return NULL;
    }
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

avlNode* deleteNode(avlNode* node, const char* iata)
{
    if (node == NULL) {
        return NULL;
    }
    int cmp = strcmp(iata, node->iata);
    if (cmp < 0) {
        node->left = deleteNode(node->left, iata);
    } else if (cmp > 0) {
        node->right = deleteNode(node->right, iata);
    } else {
        // нет детей
        if (node->left == NULL && node->right == NULL) {
            free(node->name);
            free(node);
            return NULL;
        }
        // только правый ребенок
        if (node->left == NULL) {
            avlNode* temp = node->right;
            free(node->name);
            free(node);
            return temp;
        }
        // только левый ребенок
        if (node->right == NULL) {
            avlNode* temp = node->left;
            free(node->name);
            free(node);
            return temp;
        }

        // два ребенка
        avlNode* temp = findMin(node->right);
        char* tempName = malloc(strlen(temp->name) + 1);
        if (tempName == NULL) {
            return node;
        }
        strcpy(tempName, temp->name);
        char tempIata[4];
        strcpy(tempIata, temp->iata);

        node->right = deleteNode(node->right, tempIata);

        // Обновляем текущий узел
        free(node->name);
        node->name = tempName;
        strcpy(node->iata, tempIata);
    }
    if (node == NULL) {
        return NULL;
    }
    updateHeight(node);
    return balanceNode(node);
}

avlNode* search(avlNode* node, const char* value)
{
    if (node == NULL) {
        return NULL;
    }
    int cmp = strcmp(value, node->iata);

    if (cmp == 0) {
        return node;
    }
    if (cmp < 0) {
        return search(node->left, value);
    }
    return search(node->right, value);
}

void freeTree(avlNode* node)
{
    if (node == NULL) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    if (node->name) {
        free(node->name);
        node->name = NULL;
    }
    free(node);
}

int countNodes(avlNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Проверка, что все балансы в пределах [-1, 1]
int isBalanced(avlNode* node)
{
    if (node == NULL) {
        return 1;
    }
    int balance = getBalance(node);
    if (balance < -1 || balance > 1) {
        return 0;
    }
    return isBalanced(node->left) && isBalanced(node->right);
}

int isBST(avlNode* node, char* min, char* max)
{
    if (node == NULL) {
        return 1;
    }
    if (min != NULL && strcmp(node->iata, min) <= 0) {
        return 0;
    }
    if (max != NULL && strcmp(node->iata, max) >= 0) {
        return 0;
    }
    return isBST(node->left, min, node->iata) && isBST(node->right, node->iata, max);
}
