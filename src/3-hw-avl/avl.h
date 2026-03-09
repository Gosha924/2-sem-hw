#pragma once

typedef struct avlNode {
    struct avlNode* left;
    struct avlNode* right;
    int height;
    char iata[4];
    char* name;
} avlNode;

int getHeight(avlNode* node);
int max(int a, int b);
avlNode* createNewNode(const char* iata, const char* name);
void updateHeight(avlNode* node);
int getBalance(avlNode* node);

// повороты
avlNode* rightRotation(avlNode* node);
avlNode* leftRotation(avlNode* node);
avlNode* leftRightRotation(avlNode* node);
avlNode* rightLeftRotation(avlNode* node);
avlNode* balanceNode(avlNode* node);
avlNode* insert(avlNode* node, const char* iata, const char* name);

void inOrder(avlNode* node);
void preOrder(avlNode* node);
void postOrder(avlNode* node);

int countNodes(avlNode* node);
int isBalanced(avlNode* node);
int isBST(avlNode* node, char* min, char* max);

avlNode* search(avlNode* node, const char* value);
avlNode* findMin(avlNode* node);
avlNode* deleteNode(avlNode* node, const char* iata);
void freeTree(avlNode* node);
