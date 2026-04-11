#pragma once

typedef struct AvlNode {
    struct AvlNode* left;
    struct AvlNode* right;
    int height;
    char iata[4];
    char* name;
} AvlNode;

int getHeight(AvlNode* node);
int max(int a, int b);
AvlNode* createNewNode(const char* iata, const char* name);
void updateHeight(AvlNode* node);
int getBalance(AvlNode* node);

// повороты
AvlNode* rightRotation(AvlNode* node);
AvlNode* leftRotation(AvlNode* node);
AvlNode* leftRightRotation(AvlNode* node);
AvlNode* rightLeftRotation(AvlNode* node);
AvlNode* balanceNode(AvlNode* node);
AvlNode* insert(AvlNode* node, const char* iata, const char* name);

void inOrder(AvlNode* node);
void preOrder(AvlNode* node);
void postOrder(AvlNode* node);

int countNodes(AvlNode* node);
int isBalanced(AvlNode* node);
int isBST(AvlNode* node, char* min, char* max);

AvlNode* search(AvlNode* node, const char* value);
AvlNode* findMin(AvlNode* node);
AvlNode* deleteNode(AvlNode* node, const char* iata);
void freeTree(AvlNode* node);
