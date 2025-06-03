#include <stdbool.h>
#include <string.h>
#define M 4

struct keyValuePair
{
    int key;
    char value[50];
} Pair;

struct BtreeNode
{
    int numOfKeys;
    struct keyValuePair members[M];
    struct BtreeNode* children[M+1];
    bool isLeaf;
} Node;
