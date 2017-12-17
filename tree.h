#include <stdbool.h>

// The type of items stored in the tree. Change this for different applications.
typedef int item;

//tree structure defined in tree.c
struct tree;
typedef struct tree tree;

// Create a new empty Tree.
tree *newTree();

//inserts item in correct position in tree unless item already exists.
void insert(item x, tree *t);

//checks if item exists in tree
bool search(item x, tree *t);

//deletes a node from tree
void delete(item x, tree *t);
