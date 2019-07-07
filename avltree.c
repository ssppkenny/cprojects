#include "avltree.h"
#define HEIGHT 2
#define PY_SSIZE_T_CLEAN

static PyObject *
Avltree_contains(set_t *self, PyObject* value) {
	int check_int = PyLong_Check(value);
	if (check_int) {
		long v = PyLong_AsLong(value);
		bool ret = contains(self->tree, (int)v);
		if (ret) {
			Py_RETURN_TRUE;
		} else {
			Py_RETURN_FALSE;
		}
	}
	Py_RETURN_FALSE;
}
static PyObject *
Avltree_remove(set_t *self, PyObject* value) {
	int check_int = PyLong_Check(value);
	if (check_int) {
		long v = PyLong_AsLong(value);
		if (self->tree != NULL) {
			delete(&(self->tree), (int)v);
		}
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

static PyObject *
Avltree_tolist(set_t *self) {
	int sz = size(self->tree);
	PyObject* l = PyList_New(sz);
	fill_list(self->tree, l);
	return l;
}

static PyObject *
Avltree_size(set_t *self) {
	long s = size(self->tree);
	return PyLong_FromLong(s);
}


static PyObject *
Avltree_add(set_t *self, PyObject* value) {
	int check_int = PyLong_Check(value);
	if (check_int) {
		long v = PyLong_AsLong(value);
		insert(&(self->tree), (int)v);
	}
	return Py_BuildValue("");
}

static PyMethodDef Custom_methods[] = {
	{"add", (PyCFunction) Avltree_add, METH_O,
		"Return the name, combining the first and last name"
	},
	{"contains", (PyCFunction) Avltree_contains, METH_O,
		"Return the name, combining the first and last name"
	},
	{"remove", (PyCFunction) Avltree_remove, METH_O,
		"Return the name, combining the first and last name"
	},
	{"size", (PyCFunction) Avltree_size, METH_NOARGS,
		"Return the name, combining the first and last name"
	},
	{"tolist", (PyCFunction) Avltree_tolist, METH_NOARGS,
		"Return the name, combining the first and last name"
	},
	{NULL}  /* Sentinel */
};


static PyTypeObject AvltreeType = {
	PyVarObject_HEAD_INIT(NULL, 0)
		.tp_name = "avltree.Avltree",
	.tp_doc = "Avltree",
	.tp_basicsize = sizeof(set_t),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_new = PyType_GenericNew,
	.tp_methods = Custom_methods,
};


static PyModuleDef avltreemodule = {
	PyModuleDef_HEAD_INIT,
	.m_name = "avltree",
	.m_doc = "Module.",
	.m_size = -1,
};

	PyMODINIT_FUNC
PyInit_avltree(void)
{
	PyObject* m;

	if (PyType_Ready(&AvltreeType) < 0)
		return NULL;

	m = PyModule_Create(&avltreemodule);
	if (m == NULL)
		return NULL;

	Py_INCREF(&AvltreeType);
	PyModule_AddObject(m, "Avltree", (PyObject *) &AvltreeType);
	return m;
}


void fill_list(struct avltree* tree, PyObject* list) {
   int c = 0;
   fill_list_rec(tree, list, &c);
}


void fill_list_rec(struct avltree* tree, PyObject* list, int* c) {
	if (tree == NULL) {
		return;
	} else {
		fill_list_rec(tree->left, list, c);
		PyObject* index = PyLong_FromLong(*c); 
		PyObject* value = PyLong_FromLong(tree->value); 
		PyList_SetItem(list, PyLong_AsSsize_t(index) , value);
		(*c)++;
		fill_list_rec(tree->right, list, c);
	}
	return;
}

void print(struct avltree* tree) {
	if (tree == NULL) {
		return;
	} else {
		print(tree->left);
		printf("%d ", tree->value);
		print(tree->right);
	}
}

struct avltree* remove_min(struct avltree* bt, int* v) {
	if (bt->left == NULL && bt->right == NULL) {
		*v = bt->value;
		return NULL;
	} else if (bt->left != NULL) {
		struct avltree* l = remove_min(bt->left, v);
		bt->left = l;
		return bt;
	} else {
		struct avltree* r = remove_min(bt->right, v);
		bt->right = r;
		return bt;
	}
}

void delete(struct avltree** bt, int value) {
	if (bt == NULL) {
		return;
	} else if (value < (*bt)->value) {
		delete(&(*bt)->left, value);
		(*bt)->lh = size((*bt)->left);
		(*bt)->rh = size((*bt)->right);
		(*bt)->size = (*bt)->lh + (*bt)->rh + 1;
		return;
	} else if (value > (*bt)->value) {
		delete(&(*bt)->right, value);
		(*bt)->lh = size((*bt)->left);
		(*bt)->rh = size((*bt)->right);
		(*bt)->size = (*bt)->lh + (*bt)->rh + 1;
		return;
	} else {
		if ((*bt)->left == NULL && (*bt)->right == NULL) {
			free((*bt));
			(*bt) = NULL;
		} else if ((*bt)->left != NULL && (*bt)->right == NULL) {
			free((*bt));
			(*bt) = (*bt)->left;
			(*bt)->lh = size((*bt)->left);
			(*bt)->rh = size((*bt)->right);
			//(*bt)->lh + (*bt)->rh + 1;
		} else if ((*bt)->left == NULL && (*bt)->right !=NULL) {
			free((*bt));
			(*bt) = (*bt)->right;
			(*bt)->lh = size((*bt)->left);
			(*bt)->rh = size((*bt)->right);
			//(*bt)->lh + (*bt)->rh + 1;
		} else {
			int v = (*bt)->value;
			struct avltree* r = remove_min((*bt)->right, &v);
			(*bt)->value = v;
			(*bt)->right = r;
			(*bt)->lh = size((*bt)->left);
			(*bt)->rh = size((*bt)->right);
			//(*bt)->lh + (*bt)->rh + 1;
		}
	}
}


bool contains(struct avltree* tree, int value) {
	if (tree == NULL) {
		return false;
	} else {
		if (tree->value == value) {
			return true;
		} else {
			bool lc = contains(tree->left, value);
			if (lc) {
				return true;
			} else {
				return contains(tree->right, value);
			}
		}
	}
}


int size(struct avltree* tree){
	if (tree == NULL) {
		return 0;
	} else {
		int lsize = tree->left == NULL ? 0 : tree->left->size;
		int rsize = tree->right == NULL ? 0 : tree->right->size;
		return lsize + rsize + 1;
	}
}

struct avltree* rebalance(struct avltree* tree) {

	struct avltree* left = tree->left;
	struct avltree* right = tree->right;


	if (left != NULL) {
		if ((left->lh - left->rh > HEIGHT ||
					left->rh - left->lh > HEIGHT)) {
			left = rebalance(left);
		}
	}
	if (right != NULL) {
		if ((right->lh - right->rh > HEIGHT ||
					right->rh - right->lh > HEIGHT)) {
			right = rebalance(right);
		}
	}

	tree->left = left;

	int ll = left == NULL ? 0 : left->lh;
	int lr = left == NULL ? 0 : left->rh;

	int rl = right == NULL ? 0 : right->lh;
	int rr = right == NULL ? 0 : right->rh;

	int lm = ll > lr ? ll : lr;
	int rm = rl > rr ? rl : rr;

	tree->lh = lm + 1;
	tree->rh = rm + 1;


	tree->right = right;
	tree->size = size(left) + size(right) + 1;
	tree = rotate(tree);
	return tree;
}

struct avltree* rotate(struct avltree* tree) {
	if (tree->lh - tree->rh > HEIGHT) {
		if (tree->left->lh >= tree->left->rh) {
			return rotate_left(tree);
		} else {
			return rotate_double_left(tree);
		}
	} else if (tree->rh - tree->lh > HEIGHT) {
		if (tree->right->rh >= tree->right->lh) {
			return rotate_right(tree);
		} else {
			return rotate_double_right(tree);
		}
	}
	return tree;
}


struct avltree* rotate_right(struct avltree* tree) {

	struct avltree* x= tree->left;
	struct avltree* y= tree->right->left;
	struct avltree* z= tree->right->right;

	struct avltree* k1= tree;
	struct avltree* k2= tree->right;

	k1->rh = k2->lh;
	k1->right = y;

	k1->size = size(x) + size(y) + 1;

	int m = k1->lh > k1->rh ? k1->lh : k1->rh;
	k2->lh = m+1;
	k2->left = k1;

	k2->size = size(k1) + size(z) + 1;

	return k2;


}
struct avltree* rotate_double_left(struct avltree* tree) {

	struct avltree* k3 = tree;
	struct avltree* k1 = tree->left;
	struct avltree* k2 = tree->left->right;
	struct avltree* a = k1->left;
	struct avltree* b = k2->left;
	struct avltree* c = k2->right;
	struct avltree* d = k3->right;

	k1->right = b;
	k1->rh = k2->lh;

	k1->size = size(a) + size(b) + 1;

	k3->left = c;
	k3->lh = k2->rh;

	k3->size = size(c) + size(d) + 1;

	k2->left = k1;
	k2->right = k3;

	k2->size = size(k1) + size(k2) + 1;

	k2->lh = (k1->lh > k1->rh ? k1->lh : k1->rh) + 1;
	k2->rh = (k3->lh > k3->rh ? k3->lh : k3->rh) + 1;

	return k2;
}

struct avltree* rotate_double_right(struct avltree* tree) {

	struct avltree* k1 = tree;
	struct avltree* a = k1->left;
	struct avltree* k3 = k1->right;
	struct avltree* k2 = k3->left;
	struct avltree* b = k2->left;
	struct avltree* c = k2->right;
	struct avltree* d = k3->right;


	k1->rh = k2->lh;
	k1->right = b;

	k1->size = size(a) + size(b) + 1;

	k3->lh = k2->rh;
	k3->left = c;

	k3->size = size(c) + size(d) + 1;

	k2->left = k1;
	k2->right = k3;

	k2->size = size(k1) + size(k3) + 1;

	k2->lh = (k1->lh > k1->rh ? k1->lh : k1->rh) + 1;
	k2->rh = (k3->lh > k3->rh ? k3->lh : k3->rh) + 1;
	return k2;
}

struct avltree* rotate_left(struct avltree* tree) {

	struct avltree* x= tree->left->left;
	struct avltree* y= tree->left->right;
	struct avltree* z= tree->right;

	struct avltree* k1= tree->left;
	struct avltree* k2= tree;


	k2->lh = k1->rh;
	k2->left = y;

	k2->size = size(y) + size(z) + 1;


	int m = k2->rh > k2->lh ? k2->rh : k2->lh;
	k1->rh = m + 1;
	k1->right = k2;

	k1->size = size(x) + size(k2) + 1;

	return k1;
}

void insert(avltree_t** tree, int value) {
	insert_helper(tree, value);
	*tree = rebalance(*tree);
}
int insert_helper(avltree_t** tree, int value) {

	if (*tree == NULL) {
		*tree = (struct avltree*)malloc(sizeof(struct avltree));
		(*tree)->value = value;
		(*tree)->lh = 0;
		(*tree)->rh = 0;
		(*tree)->left = NULL;
		(*tree)->right = NULL;
		(*tree)->size = 1;
		return 1;
	} else {
		if (value < (*tree)->value) {
			struct avltree *l = (*tree)->left;
			int v = insert_helper(&l, value);
			//l->size += 1;
			(*tree)->lh += v;
			(*tree)->left = l;
			if (v == 1) {
				(*tree)->size += 1;
			}
			return v;

		} else if (value > (*tree)->value) {
			struct avltree *r = (*tree)->right;
			int v = insert_helper(&r, value);
			if (v == 1) {
				(*tree)->size += 1;
			}
			(*tree)->rh += v;
			(*tree)->right = r;
			return v;
		}
		return 0;
	}

}


int main() {

	avltree_t* tree = NULL;

    insert(&tree,1);
    insert(&tree,2);
    insert(&tree,3);
    insert(&tree,5);
    insert(&tree,200);

	print(tree);

	printf("\n");
	printf("%d\n", size(tree));
	printf("%d\n", contains(tree, 56));

}
