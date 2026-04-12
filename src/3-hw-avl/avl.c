#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getHeight(AvlNode* node)
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

AvlNode* createNewNode(const char* iata, const char* name)
{
    AvlNode* newNode = (AvlNode*)malloc(sizeof(AvlNode));
    if (newNode == NULL) {
        return NULL;
    }
    strncpy(newNode->iata, iata, 3);
    newNode->iata[3] = '\0';

    newNode->name = (char*)malloc(strlen(name) + 1);
    if (!newNode->name) {
        free(newNode);
        return NULL;
    }
    strncpy(newNode->name, name, strlen(name) + 1);
    newNode->name[strlen(name)] = '\0';
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

void updateHeight(AvlNode* node)
{
    if (node == NULL) {
        return;
    }
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int getBalance(AvlNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AvlNode* rightRotation(AvlNode* node)
{
    AvlNode* newRoot = node->left;
    AvlNode* downNode = newRoot->right;
    newRoot->right = node;
    node->left = downNode;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AvlNode* leftRotation(AvlNode* node)
{
    AvlNode* newRoot = node->right;
    AvlNode* downNode = newRoot->left;

    newRoot->left = node;
    node->right = downNode;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AvlNode* leftRightRotation(AvlNode* node)
{
    node->left = leftRotation(node->left);
    return rightRotation(node);
}

AvlNode* rightLeftRotation(AvlNode* node)
{
    node->right = rightRotation(node->right);
    return leftRotation(node);
}

AvlNode* balanceNode(AvlNode* node)
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

AvlNode* insert(AvlNode* node, const char* iata, const char* name)
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

void inOrder(AvlNode* node)
{
    if (node == NULL) {
        return;
    }
    inOrder(node->left);
    printf("%s %s, ", node->iata, node->name);
    inOrder(node->right);
}

void preOrder(AvlNode* node)
{
    if (node == NULL) {
        return;
    }
    printf("%s %s, ", node->iata, node->name);
    preOrder(node->left);
    preOrder(node->right);
}

void postOrder(AvlNode* node)
{
    if (node == NULL) {
        return;
    }
    postOrder(node->left);
    postOrder(node->right);
    printf("%s %s, ", node->iata, node->name);
}

AvlNode* findMin(AvlNode* node)
{
    if (node == NULL) {
        return NULL;
    }
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

AvlNode* deleteNode(AvlNode* node, const char* iata)
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
            AvlNode* temp = node->right;
            free(node->name);
            free(node);
            return temp;
        }
        // только левый ребенок
        if (node->right == NULL) {
            AvlNode* temp = node->left;
            free(node->name);
            free(node);
            return temp;
        }

        // два ребенка
        AvlNode* temp = findMin(node->right);
        char* tempName = malloc(strlen(temp->name) + 1);
        if (tempName == NULL) {
            return node;
        }
        strncpy(tempName, temp->name, strlen(temp->name) + 1);
        tempName[strlen(temp->name)] = '\0';

        char tempIata[4];
        strncpy(tempIata, temp->iata, 4);
        tempIata[3] = '\0';

        node->right = deleteNode(node->right, tempIata);

        free(node->name);
        node->name = tempName;
        strncpy(node->iata, tempIata, 4);
        node->iata[3] = '\0';
    }
    if (node == NULL) {
        return NULL;
    }
    updateHeight(node);
    return balanceNode(node);
}

AvlNode* search(AvlNode* node, const char* value)
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

void freeTree(AvlNode* node)
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

int countNodes(AvlNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Проверка, что все балансы в пределах [-1, 1]
int isBalanced(AvlNode* node)
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

int isBST(AvlNode* node, char* min, char* max)
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
