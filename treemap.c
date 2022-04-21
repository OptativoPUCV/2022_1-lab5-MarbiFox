#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    //new->lower_than = lower_than;
    return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

}

TreeNode * minimum(TreeNode * x) {
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while (aux != NULL){
        if (tree->lower_than(key, aux->pair->key) == 1) { //es la clave buscada menor que la clave actual.
            aux = aux->left;
            continue;
        }
        if (tree->lower_than(aux->pair->key, key) == 1) { //es la clave actual menor que la buscada.
            aux = aux->right;
            continue;
        }
        tree->current = aux;
        return aux->pair;
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    while (aux->left != NULL) {
        aux = aux->left;
    }
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->current;
    //Buscar Hijos.
    if(aux->right != NULL) {
        aux = (TreeNode *) aux->right;
        return minimum(aux);
    }
    //Buscar en Ascendencia.
    while (aux->parent != NULL) {
        if (tree->lower_than(aux->pair->key, aux->parent->pair->key) == 1) { //Si la key current es menor que la de su ancestro.
            aux = aux->parent;
            return aux->pair;
        }
        aux = aux->parent;
    }
    return NULL;
}
