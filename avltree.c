#include "avltree.h"
#define HEIGHT 2

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
            (*bt)->lh + (*bt)->rh + 1;
        } else if ((*bt)->left == NULL && (*bt)->right !=NULL) {
            free((*bt));
            (*bt) = (*bt)->right;
            (*bt)->lh = size((*bt)->left);
            (*bt)->rh = size((*bt)->right);
            (*bt)->lh + (*bt)->rh + 1;
        } else {
            int v = (*bt)->value;
            struct avltree* r = remove_min((*bt)->right, &v);
            (*bt)->value = v;
            (*bt)->right = r;
            (*bt)->lh = size((*bt)->left);
            (*bt)->rh = size((*bt)->right);
            (*bt)->lh + (*bt)->rh + 1;
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
    for (int i = 0; i<100000; i++) {
        insert(&tree, i);
    }


    for (int i = 0; i<50000; i++) {
        delete(&tree, i);
    }


    print(tree);

    printf("\n");
    printf("%d\n", size(tree));

}
