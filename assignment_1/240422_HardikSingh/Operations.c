#include "Node.h"
#include <stdio.h>
#include <stdlib.h>

void insertKey(struct BtreeNode* node, struct keyValuePair pair) {
    int i = node->numOfKeys - 1;

    while (i >= 0 && node->members[i].key > pair.key) {
        node->members[i + 1] = node->members[i];
        i--;
    }
    node->members[i + 1] = pair;
    node->numOfKeys++;
}

void splitChild(struct BtreeNode* parent, int index) {
    struct BtreeNode* child = parent->children[index];
    struct BtreeNode* newChild = malloc(sizeof(struct BtreeNode));
    newChild->isLeaf = child->isLeaf;

    int mid = M / 2; 
    newChild->numOfKeys = M - mid - 1; 
    for (int j = 0; j < newChild->numOfKeys; j++) {
        newChild->members[j] = child->members[mid + 1 + j];
    }

    if (!child->isLeaf) {
        for (int j = 0; j <= newChild->numOfKeys; j++) {
            newChild->children[j] = child->children[mid + 1 + j];
        }
    }

    child->numOfKeys = mid;

    for (int j = parent->numOfKeys; j > index; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = newChild;

    for (int j = parent->numOfKeys - 1; j >= index; j--) {
        parent->members[j + 1] = parent->members[j];
    }

    parent->members[index] = child->members[mid];
    parent->numOfKeys++;
}


void insertNonFull(struct BtreeNode* node, struct keyValuePair pair) {
    if (node->isLeaf) {
        insertKey(node, pair);
    } else {
        int i = node->numOfKeys - 1;
        while (i >= 0 && node->members[i].key > pair.key) {
            i--;
        }
        i++;
        if ((node->children[i]->numOfKeys) == M) {
            splitChild(node, i);
            if (pair.key > node->members[i].key) {
                i++;
            }
        }
        insertNonFull(node->children[i], pair);
    }
}

void insert(struct BtreeNode** root, struct keyValuePair pair) {
    if (*root == NULL) {
        *root = malloc(sizeof(struct BtreeNode));
        (*root)->isLeaf = true;
        (*root)->numOfKeys = 0;
    }

    if ((*root)->numOfKeys == M) {
        struct BtreeNode* newRoot = malloc(sizeof(struct BtreeNode));
        newRoot->isLeaf = false;
        newRoot->numOfKeys = 0;
        newRoot->children[0] = *root;
        splitChild(newRoot, 0);
        *root = newRoot;
    }

    insertNonFull(*root, pair);
}

void printLevelOrder(struct BtreeNode* root) {
    if (root == NULL) return;

    struct BtreeNode* currentLevel[1000];
    int count = 1;
    currentLevel[0] = root;

    while (count > 0) {
        printf("[");
        for (int i = 0; i < count; i++) {
            struct BtreeNode* node = currentLevel[i];
            printf("{");
            for (int j = 0; j < node->numOfKeys; j++) {
                printf("%d,%s", node->members[j].key, node->members[j].value);
                if (j < node->numOfKeys - 1) printf(" ");
            }
            printf("}");
            if (i < count - 1) printf(" ");
        }
        printf("]\n");

        // Prepare next level
        int nextCount = 0;
        struct BtreeNode* nextLevel[1000];
        for (int i = 0; i < count; i++) {
            if (!currentLevel[i]->isLeaf) {
                for (int j = 0; j <= currentLevel[i]->numOfKeys; j++) {
                    nextLevel[nextCount++] = currentLevel[i]->children[j];
                }
            }
        }

        for (int i = 0; i < nextCount; i++) {
            currentLevel[i] = nextLevel[i];
        }
        count = nextCount;
    }
}

void printBtree(struct BtreeNode* node, int level) {
    if (node == NULL) return;

    printf("Level %d: ", level);
    for (int i = 0; i < node->numOfKeys; i++) {
        printf("%d ", node->members[i].key);
    }
    printf("\n");

    if (!node->isLeaf) {
        for (int i = 0; i <= node->numOfKeys; i++) {
            printBtree(node->children[i], level + 1);
        }
    }
}

void freeBtree(struct BtreeNode* node) {
    if (node == NULL) return;

    if (!node->isLeaf) {
        for (int i = 0; i <= node->numOfKeys; i++) {
            freeBtree(node->children[i]);
        }
    }
    free(node);
}

void searchKey(struct BtreeNode* node, int key) {
    if (node == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    int i = 0;
    while (i < node->numOfKeys && key > node->members[i].key) {
        i++;
    }

    if (i < node->numOfKeys && node->members[i].key == key) {
        printf("Key %d found with value: %s\n", key, node->members[i].value);
        return;
    }

    if (node->isLeaf) {
        printf("Key %d not found.\n", key);
        return;
    }

    searchKey(node->children[i], key);
}

void deleteKey(struct BtreeNode* node, int key) {
    if (node == NULL) {
        printf("Key %d not found for deletion.\n", key);
        return;
    }

    int i = 0;
    while (i < node->numOfKeys && key > node->members[i].key) {
        i++;
    }

    if (i < node->numOfKeys && node->members[i].key == key) {
        if (node->isLeaf) {
            for (int j = i; j < node->numOfKeys - 1; j++) {
                node->members[j] = node->members[j + 1];
            }
            node->numOfKeys--;
        } else {
            printf("Deletion in internal nodes is not implemented yet.\n");
        }
    } else if (!node->isLeaf) {
        deleteKey(node->children[i], key);
    } else {
        printf("Key %d not found for deletion.\n", key);
    }
}

int main() {
    struct BtreeNode* root = NULL;

    struct keyValuePair data[] = {
        {10, "Pratham"}, {20, "Rahul"}, {5, "Ridin"},
        {3, "Burhan"}, {2, "Hardik"}, {12, "Yash"},
        {17, "Divyansh"}, {30, "Shivam"}, {25, "Aniket"},
        {15, "Aarav"}, {8, "Karan"}, {1, "Sahil"},
        {4, "Nikhil"}, {6, "Rohan"}, {9, "Vikas"},
        {11, "Siddharth"}, {13, "Aakash"}, {14, "Gaurav"},
        {16, "Anshul"}, {18, "Kunal"}, {19, "Pawan"},
        {21, "Vivek"}, {22, "Akash"}, {23, "Deepak"},
        {24, "Rajesh"}, {26, "Suresh"}, {27, "Vinay"},
        {28, "Ajay"}, {29, "Mohit"}
    };

    int n = sizeof(data) / sizeof(data[0]);

    printf("=== Inserting Keys ===\n");
    for (int i = 0; i < n; i++) {
        insert(&root, data[i]);
    }

    printf("\n=== B-tree (Level Order) ===\n");
    printLevelOrder(root);  // use the level-order function we made earlier

    printf("\n=== Searching for Keys ===\n");
    searchKey(root, 17);  // present
    searchKey(root, 25);  // present

    printf("\n=== Deleting Key 3 ===\n");
    deleteKey(root, 3);  // implemented only for leaves
    printLevelOrder(root);

    printf("\n=== Deleting Key 10 (Internal, not implemented) ===\n");
    deleteKey(root, 10);  // not implemented
    printLevelOrder(root);

    printf("\n=== Freeing Tree ===\n");
    freeBtree(root);
    return 0;
}
