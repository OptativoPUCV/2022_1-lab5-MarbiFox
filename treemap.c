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

    TreeMap * new = (TreeMap *) malloc (sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    //Crear el nodo nuevo.
    TreeNode * new = createTreeNode(key, value);
    //Buscar el espacio disponible con la key.
    if (searchTreeMap(tree, key) != NULL) return;
    //Insertar como hijo del current.
    if (tree->lower_than(key, tree->current->pair->key) == 1) {
      tree->current->left = new;
    }
    else {
      tree->current->right = new;
    }
    new->parent = tree->current;
    tree->current = new;
}

TreeNode * minimum(TreeNode * x) {
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    //Identificar Caso.
    if (node->left == NULL && node->right == NULL) {
        //Eliminar, caso 0 hijos.
        if (tree->lower_than(node->pair->key, node->parent->pair->key) == 1) {
            node->parent->left = NULL;
        }
        else {
          node->parent->right = NULL;
        }
        free(node);
        return;
    }
    if (node->left == NULL || node->right == NULL) {
        //Eliminar, caso 1 hijo.
        if (tree->lower_than(node->pair->key, node->parent->pair->key) == 1) {
            if (node->left != NULL) { //Hijo del nodo a eliminar, está a la izquierda.
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            else { //Hijo del nodo a eliminar, está a la izquierda.
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
        }
        else {
            if (node->left != NULL) { //Hijo del nodo a eliminar, está a la izquierda.
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            else { //Hijo del nodo a eliminar, está a la izquierda.
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
        free(node);
        return;
      }
      else {
        //Eliminar, caso 2 hijos.
        printf("key: %d\n", *(int*)tree->root->left->pair->key);
        //Buscar el valor mínimo del subárbol derecho.
        TreeNode * min = minimum(node->right);
        //Copiar min en node.
        node->pair->value = min->pair->value;
        node->pair->key = min->pair->key;
        //Llamar de vuelta a la función, pero siendo min el nodo a eliminar.
        removeNode(tree, min);
      }
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
        tree->current = aux;
        if (tree->lower_than(key, aux->pair->key) == 1) { //es la clave buscada menor que la clave actual.
            aux = aux->left;
            //tree->current = aux;
            continue;
        }
        if (tree->lower_than(aux->pair->key, key) == 1) { //es la clave actual menor que la buscada.
            aux = aux->right;
            //tree->current = aux;
            continue;
        }
        return aux->pair;
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    while (1) {
        Pair * aux = searchTreeMap(tree, key);
        if (aux != NULL) return aux;
        else key++;
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    while (aux->left != NULL) {
        aux = aux->left;
    }
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    //Buscar Hijos.
    if(tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    //Buscar en Ascendencia.
    while (tree->current->parent != NULL) {
        if (tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) == 1) { //Si la key current es menor que la de su ancestro.
            tree->current = tree->current->parent;
            return tree->current->pair;
        }
        tree->current = tree->current->parent;
    }
    return NULL;
}
