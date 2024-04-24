#include "rbtree.h"
#include <stdlib.h>
#define SENTINEL

// rbtree *new_rbtree(void) {
//   rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
//   // TODO: initialize struct if needed
//   return p;
// }

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
#ifdef SENTINEL
    p->nil = NIL;
    p->root = NIL;
#else
    p->nil = NULL;
    p->root = NULL;
#endif
    NIL->color = RBTREE_BLACK;
    return p;
}



void DeleteByPostOrder(node_t *root, rbtree *t){
    if (root == t -> nil)
    {
        return;
    }
    DeleteByPostOrder(root -> left, t);
    DeleteByPostOrder(root -> right, t);
    free(root);
}
void delete_rbtree(rbtree *t){
    // TODO: reclaim the tree nodes's memory
    if (t == NULL)
    {
        return;
    }
    DeleteByPostOrder(t -> root, t);
    free(t-> nil);
    free(t);
}


void left_rotate(rbtree *t, node_t *x){
    node_t *y;
    y = x -> right;
    x -> right = y -> left;
    if (y -> left != t -> nil)
    {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == t -> nil)
    {
        t -> root = y;
    }
    else if (x == x -> parent -> left)
    {
        x -> parent -> left = y;
    }
    else
    {
        x -> parent -> right = y;
    }
    y -> left = x;
    x -> parent = y;
    return;
}

void right_rotate(rbtree *t, node_t *x){
    node_t *y;
    y = x -> left;
    x -> left = y -> right;
    if (y -> right != t -> nil)
    {
        y -> right -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == t -> nil)
    {
        t -> root = y;
    }
    else if (x == x -> parent -> right)
    {
        x -> parent -> right = y;
    }
    else
    {
        x -> parent -> left = y;
    }
    y -> right = x;
    x -> parent = y;
    return;
}
void rb_insert_Fixup(rbtree *t, node_t *z){
    node_t *uncle;
    // while ((z != t->root) && (z->color != RBTREE_BLACK) && (z->parent->color == RBTREE_RED))
    while (z->parent->color == RBTREE_RED)
    {
        if (z -> parent == z -> parent -> parent -> left)
        {
            uncle = z -> parent -> parent -> right;
            //경우1
            if (uncle -> color == RBTREE_RED)
            {
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                z = z -> parent -> parent;
            }
            //경우2
            else {
                if (z == z -> parent -> right)
                {
                    z = z -> parent;
                    left_rotate(t, z);
                }
                //경우3
                z -> parent -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                right_rotate(t, z -> parent -> parent);
            }
        }
        //반대로
        else
        {
            uncle = z -> parent -> parent -> left;
            //경우1
            if (uncle != t -> nil && uncle -> color == RBTREE_RED)
            {
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                z = z -> parent -> parent;
            }
            //경우2
            else {
                if (z == z -> parent -> left)
                {
                    z = z -> parent;
                    right_rotate(t, z);
                }
                //경우3
                if (z != t -> root && z -> parent != t -> root)
                {
                    z -> parent -> color = RBTREE_BLACK;
                    z -> parent -> parent->color = RBTREE_RED;
                    left_rotate(t, z -> parent -> parent);
                }
            }
        }
    }
    t -> root -> color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *parent = t -> nil;
    node_t *p = t -> root;
    while (p != t -> nil)
    {
        parent = p;
        if (p -> key > key)
        {
            p = p -> left;
        }
        else
        {
            p = p -> right;
        }
    }
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    new_node->parent = parent;
    // 트리의 첫 노드일 때 - 루트가 된다
    if (parent == t -> nil){
        t -> root = new_node;
        new_node -> color = RBTREE_BLACK;
    }
    //찾은 부모노드보다 새로운 노드가 작을때 - 왼쪽 자식으로
    else if (key < parent -> key){
        parent -> left = new_node;
        new_node -> color = RBTREE_RED;
    }
    //크거나 같을때 - 오른쪽 자식으로
    else{
        parent -> right = new_node;
        new_node -> color = RBTREE_RED;
    }
    //새로운 노드 설정
    new_node -> key = key;
    new_node -> left = t->nil;
    new_node -> right = t->nil;

    rb_insert_Fixup(t, new_node);
    return t -> root;
}
/*node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}*/
/*node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = NULL;
    node_t *x = t->root;
    node_t *z = (node_t *)calloc(1, sizeof(node_t)); // 새로운 노드 생성

    z->key = key; // 새로운 노드에 키 할당
    z->left = NULL;
    z->right = NULL;
    z->color = RBTREE_RED; // RBTREE_RED로 초기화

    // 삽입할 위치를 찾을 때까지 이진 탐색 트리의 형태로 이동
    while (x != NULL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NULL)
        t->root = z; // 트리가 비어있을 경우 새로운 노드를 루트로 설정
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = NULL;
    z->right = NULL;
    z->color = RBTREE_RED; // RBTREE_RED로 초기화

    return z; // 삽입된 노드를 반환
}*/



/*
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}*/
node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *x = t->root;

    while (x != NULL) {
        if (key == x->key)
            return x;
        else if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return NULL; // Key not found
}



/*
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}
*/
node_t *rbtree_min(const rbtree *t){
    // TODO: implement find
    node_t *r = t -> root;
    if (r == t -> nil)
        return r;
    while (r -> left != t -> nil)
    {
        r = r -> left;
    }
    return r;
}
node_t *rbtree_max(const rbtree *t){
    // TODO: implement find
    node_t *r = t -> root;
    if (r == t -> nil)
        return r;
    while (r -> right != t -> nil)
    {
        r = r -> right;
    }
    return r;
}





// int rbtree_erase(rbtree *t, node_t *p) {
//   // TODO: implement erase
//   return 0;
// }

void rb_delete_fixup(rbtree *t, node_t *x){
    node_t *w;
    while ((x != t -> root) && (x -> color == RBTREE_BLACK))
    {
        if (x == x -> parent -> left)
        {
            w = x -> parent -> right;
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                left_rotate(t, x -> parent);
                w = x -> parent -> right;
            }
            if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK)
            {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            else
            {
                if (w -> right -> color == RBTREE_BLACK)
                {
                    w -> left -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    right_rotate(t, w);
                    w = x -> parent -> right;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> right -> color = RBTREE_BLACK;
                left_rotate(t, x -> parent);
                x = t->root;
            }
        }
        else
        {
            w = x -> parent -> left;
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK;
                x -> parent->color = RBTREE_RED;
                right_rotate(t, x -> parent);
                w = x -> parent->left;
            }
            if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK)
            {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            else
            {
                if (w -> left -> color == RBTREE_BLACK)
                {
                    w -> right -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x -> parent -> left;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> left -> color = RBTREE_BLACK;
                right_rotate(t, x -> parent);
                x = t -> root;
            }
        }
    }
    x -> color = RBTREE_BLACK;
}



node_t *tree_minimum(const rbtree *t, node_t *sub_root){
    // TODO: implement find
    node_t *r = sub_root;
    if (r == t -> nil)
        return r;
    while (r -> left != t -> nil)
    {
        r = r -> left;
    }
    return r;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
    if (u -> parent == t -> nil)
    {
        t -> root = v;
    }
    else if (u == u -> parent -> left)
    {
        u -> parent -> left = v;
    }
    else
        u -> parent -> right = v;
    v -> parent = u -> parent;
    return;
}
int rbtree_erase(rbtree *t, node_t *p) {
    node_t *y = p;
    color_t y_orginal_color = y->color;
    node_t *x;
    if (p -> left == t -> nil)
    {
        x = p -> right;
        rb_transplant(t, p, p -> right);
    }
    else if (p -> right == t -> nil)
    {
        x = p -> left;
        rb_transplant(t, p, p -> left);
    }
    else
    {
        y = tree_minimum(t, p -> right);
        y_orginal_color = y->color;
        x = y -> right;
        if (y -> parent == p)
        {
            x -> parent = y;
        }
        else
        {
            rb_transplant(t, y, y -> right);
            y -> right = p -> right;
            y -> right -> parent = y;
        }
        rb_transplant(t, p, y);
        y -> left = p -> left;
        y -> left -> parent = y;
        y -> color = p -> color;
    }
    if (y_orginal_color == RBTREE_BLACK)
    {
        rb_delete_fixup(t, x);
    }
    free(p);
    return 0;
}


/*
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
*/
int inorder_rbtree(node_t *root, key_t *res,const rbtree *t, int i){
    if (root == t -> nil)
    {
        return i;
    }

    i = inorder_rbtree(root->left, res, t, i);
    res[i] = root->key;
    i += 1;
    i = inorder_rbtree(root->right, res, t, i);
    return i;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
    if (t->root == t->nil)
    {
        return -1;
    }
    inorder_rbtree(t->root, arr, t, 0);
    return 0;
}

