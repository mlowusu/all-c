#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct node{
    struct node *l;
    struct node *r;
    item value;
};
typedef struct node node;

struct tree{
    node *root;
    bool isEmpty;
};


tree *newTree(){
    tree *t = malloc(sizeof(tree));
    t->root = malloc(sizeof(node));
    t->isEmpty = true;
    node *root = t->root;
    root->l = NULL;
    root->r = NULL;
    return t;
}

//iinserts node into list 
void insertNode(node *node, tree *t){
    struct node *current;
    current = t->root;
    while(current != NULL){
        if(node->value == current-> value) break;
        if(node->value < current->value){
            if(current->l == NULL) current->l = node;
            current = current->l;
        }
        else{
            if(current->r == NULL) current->r = node;
            current = current->r;
        }
    }
    
}

void insert(item x, tree *t){
    if(t->isEmpty == true) t->root->value = x;
    else{
        node *xnode = malloc(sizeof(node));
        xnode->value = x;
        xnode->l = NULL;
        xnode->r = NULL;
        insertNode(xnode, t);
    }
    t->isEmpty = false;
}


bool search(item x, tree *t){
    if(t->isEmpty == true) return false;
    struct node *current;
    current = t->root;
    bool found = false;
    while(found == false){
        if(x == current-> value) found = true;
        if(x < current->value){
            if(current->l == NULL) break;
            current = current->l;
        }
        else{
            if(current->r == NULL) break;
            current = current->r;
        }
    }
    return found;
}

void delete(item x, tree *t){
    assert(search(x,t) == true);
    struct node *current;
    current = t->root;
    bool found = false;
    node *prev;
    bool directionl;//capture which child node of previous node c is at(true for l, false for r)
    while(found == false){
        if(x == current-> value) found = true;
        if(x < current->value){
            prev = current;
            directionl = true;
            current = current->l;
        }
        else if(x > current->value){
            prev = current;
            directionl = false;
            current = current->r;
        }
    }
    node *lchild = current->l;
    node *rchild = current->r;
    if (current != t->root){
        if(directionl)prev->l = NULL;
        else prev->r =NULL;
        free(current);
        if(lchild !=NULL) insertNode(lchild, t);
        if(rchild != NULL)insertNode(rchild,t);
    }
    else if((lchild == NULL)&&(rchild == NULL)) t->isEmpty = true;
    else{
        free(current);
        if(lchild != NULL){
            t->root = lchild;
            if(rchild != NULL)insertNode(rchild,t);
        }
        else if(rchild != NULL){
            t->root = rchild;
            if(lchild != NULL)insertNode(lchild,t);
        }
    }
}

//-------------------------------------------------------------
//tests

//free nodes and tree structure if tree has just one node (ie root)
void freeEmpty(tree *t){
    free(t->root);
    free(t);
}

//free all nodes in a tree and then the tree itself
void freeTree(tree *t){
    node *c = t->root;
    node *prev;
    bool directionl;//capture which child node of previous node c is at(true for l, false for r)
    while((c->l != NULL) || (c->r != NULL)){
        while((c->l != NULL) || (c->r != NULL)){
            prev = c;
            directionl = false;
            if(c->l != NULL){
                c = c->l;
                directionl = true;
            }
            else{
                c = c->r;
                directionl = false;
            }
        }
    if(directionl == true) prev->l = NULL;
    else prev->r = NULL;
    free(c);
    c = t->root;
    }
    freeEmpty(t);
}

void testNew(){
    tree *t = newTree();
    assert(t->isEmpty == true);
    freeEmpty(t);
}

void testInsert(){
    tree *t = newTree();
    node *root = t->root;
    insert(4,t);
    insert(5,t);
    insert(4,t);
    assert(root->value == 4);
    assert(root->r->value == 5);
    assert(root->l == NULL);
    freeTree(t);
}

void testSearch(){
    tree *t = newTree();
    insert(5,t);
    insert(3,t);
    insert(7,t);
    insert(9,t);
    assert(search(5,t));
    assert(search(9,t));
    assert(!search(4,t));
    assert(!search(8,t));
}

void testDelete(){
    
    tree *t = newTree();
    insert(5,t);
    insert(3,t);
    insert(7,t);
    insert(9,t);
    delete(3,t);
    assert(!search(3,t));
    delete(5,t);
    assert(!search(5,t));
    freeTree(t);
}

void test(){
    testNew();
    testInsert();
    testSearch();
    testDelete();
    printf("Tests pass\n");
}

int treeMain(){
    test();
    return 0;
}

