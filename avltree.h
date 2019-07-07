#include <Python.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct avltree {
	int value;
	int size;
	int lh;
	int rh;
	struct avltree* left;
	struct avltree* right;
} avltree_t;

typedef struct set {
	PyObject_HEAD
    struct avltree* tree;
} set_t;

void fill_list_rec(struct avltree* tree, PyObject* list, int* c);
void fill_list(struct avltree* tree, PyObject* list);
void insert(avltree_t** tree, int value);
int insert_helper(avltree_t** tree, int value);
struct avltree* rebalance(struct avltree* tree);
void print(struct avltree* tree);
int size(struct avltree* tree);
bool contains(struct avltree* tree, int value);
struct avltree* remove_min(struct avltree* bt, int* v);
void delete(struct avltree** bt, int value);
struct avltree* rotate(struct avltree* tree);
struct avltree* rotate_left(struct avltree* tree);
struct avltree* rotate_right(struct avltree* tree);
struct avltree* rotate_double_left(struct avltree* tree);
struct avltree* rotate_double_right(struct avltree* tree);

