#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


struct node {
    struct node *next;
    struct node *back;
    item element;
};
typedef struct node node;


struct list {
    struct node *current;//current position will be pointing to next node
    struct node *start;//sentinel at start
    struct node *end;//sentinetl at end
};


//sets current positu=ion to between start and end (ie points to end)
list *newList(){
    list *new;
    new = malloc(sizeof(list));
    new->current = malloc(sizeof(node));
    new->start = malloc(sizeof(node));
    new->end = malloc(sizeof(node));
    node *start = new->start;
    node *end = new->end;
    start->next = end;
    end->back = start;
    new->current = end;
    return new;
}

//create a node with item inside(undefined pointers)
node *newNode(item x){
    node *newnode = malloc(sizeof(node));
    newnode->element = x;
    return newnode;
}

//change position to before first node(after 1st sentinel)
void start(list *l){
    node *s = l->start;
    l->current = s->next;
}

//change position to after last node(before 2nd sentinel)
void end(list *l){
    l->current = l->end;
}

//check if currnt position is at the start(before first non-sentinel node)
bool atStart(list *l){
    node *start = l->start;
    if(l->current == start->next)return true;
    return false;
}

//check current is at end(after last non-sentinel node)
bool atEnd(list *l){
    if(l->current == l->end)return true;
    return false;
}

//move current position forward once
void forward(list *l){
    assert(atEnd(l) == false);
    node *c = l->current;
    c = c->next;
    l->current = c;
}

//move current position back one
void backward(list *l){
    assert(atStart(l) == false);
    node *c = l->current;
    c = c->back;
    l->current = c;
}

//create new node before current position 
void insertBefore(list *l, item x){
    node *newnode = newNode(x);
    node *after = l->current;
    node *prev = after->back;
    newnode->next = after;
    newnode->back = prev;
    after->back = newnode;
    prev->next = newnode;
    l->current = newnode->next;
}

//create new node after current position
void insertAfter(list *l, item x){
    node *newnode = newNode(x);
    node *after = l->current;
    node *prev = after->back;
    newnode->next = after;
    newnode->back = prev;
    after->back = newnode;
    prev->next = newnode;
    l->current = newnode;
}

//return item before current position
item getBefore(list *l){
    assert(atStart(l) == false);
    node *after = l->current;
    node *prev = after->back;
    return prev->element;
}

//return item after current
item getAfter(list *l){
    assert(atEnd(l) == false);
    node *after = l->current;
    return after->element;
}

//change item value in node before current position
void setBefore(list *l, item x){
    assert(atStart(l) == false);
    node *after = l->current;
    node *prev = after->back;
    prev->element = x;
}

//change item value in node before current position
void setAfter(list *l, item x){
    assert(atEnd(l) == false);
    node *after = l->current;
    after->element = x;
}

//delete node before current position
void deleteBefore(list *l){
    assert(atStart(l) == false);
    backward(l);
    deleteAfter(l);
}

//delete node after current position
void deleteAfter(list *l){
    assert(atEnd(l) == false);
    node *c = l->current;
    node *n = c->next;
    node *b = c->back;
    b->next = n;
    n->back = b;
    free(l->current);
    l->current = n;
}

//tests
//--------------------------------------------------------

//join nodes together in given order for testing purposes
void series(node *t[7]){
    for(int i=0;i<6;i++){
        t[i]->next = t[i+1];
        t[i+1]->back = t[i];
    }
}

//make a list for testing purposes and set curretn position to start
list *testList(){
    list *l = newList();
    node *t[7];
    t[0] = l->start;
    t[6] = l->end;
    for(int i=1;i<6;i++){
        t[i] = newNode(i);
    }
    series(t);
    start(l);
    return l;
}

//free all nodes in a list then the list itself
void empty(list *l){
    start(l);
    while (l->current != l->end){
        deleteAfter(l);
    }
    free(l->start);
    free(l->end);
    free(l);
}


void testNew(){
    list *new;
    new = newList();
    assert(new->current == new->end);
    //backward(new);
    empty(new);
}

void testStart(){
    list *new;
    new = newList();
    new->current = NULL;
    node *s = new->start;
    start(new);
    assert(new->current = s->next);
    end(new);
    assert(new->current = new->end);
    empty(new);
}

void testAt(){
    list *new;
    new = newList();
    assert(atEnd(new) == true);
    assert(atStart(new) == true);
    new->current = NULL;
    assert(atEnd(new) == false);
    assert(atStart(new) == false);
    empty(new);
}

void testInsert(){
    list *new;
    new = newList();
    insertBefore(new,2);
    assert(new->current == new->end);
    insertAfter(new,3);
    node *current = new->current;
    node *back = current->back;
    assert(current->element == 3);
    assert(back->element == 2);
    empty(new);
}

void testForward(){
    list *l = testList();
    forward(l);
    node *c = l->current;
    assert(c->element == 2);
    backward(l);
    c = l->current;
    assert(c->element == 1);
}

void testGet(){
    list *l = testList();
    forward(l); forward(l);
    assert(getBefore(l) == 2);
    assert(getAfter(l) == 3);
    empty(l);
}

void testSet(){
    list *l = testList();
    forward(l);
    setBefore(l,6);
    assert(getBefore(l) == 6);
    setAfter(l,7);
    assert(getAfter(l)==7);
    empty(l);
}

void testDelete(){
    list *l = testList();
    forward(l);
    deleteBefore(l);
    assert(atStart(l) == true);
    deleteAfter(l);
    assert(getAfter(l) == 3);
    empty(l);
}

void test(){
    testNew();
    testStart();
    testAt();
    testInsert();
    testForward();
    testGet();
    testSet();
    testDelete();
    printf("Tests pass\n");
}

int listMain(){
    test();
    tesList();
    return 0;
}


