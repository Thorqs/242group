#include <stdlib.h>
#include "tree.h"
#include "mylib.h"
#include <string.h>
#include <stdio.h>

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

typedef enum { RED, BLACK } tree_colour;

static tree_t tree_type;


struct tree_node {
    char *key;
    tree left;
    tree right;
    int frequency;
    tree_colour colour;
};

/**
 * creates a new tree of the specified type
 *
 * @param type The type of tree desired
 *             RBT for Red/Black Tree
 *             BST for Binary Search Tree
 */
tree tree_new(tree_t type) {
    tree_type = type;
    return NULL;
}



static tree right_rotate(tree T) {
    tree temp = T;
    T = T->left;
    temp->left = T->right;
    T->right = temp;
    return T;
}


static tree left_rotate(tree T) {
    tree temp = T;
    T = T->right;
    temp->right = T->left;
    T->left = temp;
    return T;
}



static tree tree_fix(tree T) {
    if (IS_RED(T -> left) && IS_RED(T -> left -> left)) {
        if (IS_RED(T -> right)) {
            T -> colour = RED;
            T -> left -> colour = BLACK;
            T -> right -> colour = BLACK;
        } else {
            T = right_rotate(T);
            T -> colour = BLACK;
            T -> right -> colour = RED;
        }
    } else if (IS_RED(T -> left) && IS_RED(T -> left -> right)) {
        if (IS_RED(T -> right)) {
            T -> colour = RED;
            T -> left -> colour = BLACK;
            T -> right -> colour = BLACK;
        } else {
            T -> left = left_rotate(T -> left);
            T = right_rotate(T);
            T -> colour = BLACK;
            T -> right -> colour = RED;
        }
    } else if (IS_RED(T->right) && IS_RED(T -> right -> left)) {
        if (IS_RED(T->left)) {
            T->colour = RED;
            T->left->colour = BLACK;
            T->right->colour = BLACK;
        } else {
            T->right = right_rotate(T->right);
            T = left_rotate(T);
            T->colour = BLACK;
            T->left->colour = RED;
        }
    } else if (IS_RED(T -> right) && IS_RED(T -> right -> right)) {
        if (IS_RED(T -> left)) {
            T->colour = RED;
            T->left->colour = BLACK;
            T->right->colour = BLACK;
        } else {
            T = left_rotate(T);
            T->colour = BLACK;
            T->left->colour = RED;
        }
    }
    return T;
}



tree tree_insert(tree T, char *key) {
    if (T == NULL) {
        T = emalloc(sizeof * T);
        T->left = NULL;
        T->right = NULL;
        T->frequency = 1;
        T->key = emalloc((strlen(key) + 1) * sizeof(T->key));
        strcpy(T->key, key);
        if (tree_type == RBT) {
            T->colour = RED;
        }
    } else if (strcmp(T->key, key) < 0) {
        T->right = tree_insert(T->right, key);
    } else if (strcmp(T->key, key) > 0) {
        T->left = tree_insert(T->left, key);
    } else {
        T->frequency++;
    }
    if (tree_type == RBT) {
        T = tree_fix(T);
     }
    return T;
}

int tree_search(tree T, char *key) {
    if (T == NULL) { /* key not found */
        return 0; 
    } else if (strcmp(T->key, key) == 0) { /* key found */
        return 1; 
    } else if (strcmp(T->key, key) > 0) { /* key comes before current */
        return tree_search(T->left, key);
    } else  if (strcmp(T->key, key) < 0) { /*key comes after current */
        return tree_search(T->right, key);
    }
    return 1;
}



void tree_inorder(tree T, void f(char *key)) {
    if(T == NULL) {
        return;
    }
    tree_inorder(T->left, f);
    f(T->key);
    tree_inorder(T->right, f); 
}



void tree_preorder(tree T, void f(char *key)){
    if(T == NULL) {
        return;
    }
    f(T->key);
    tree_preorder(T->left, f);
    tree_preorder(T->right, f);
}

        
tree tree_fix_root(tree T) {
    if(IS_RED(T)) {
        T->colour = BLACK;
    }
    return T;
}

 
tree tree_free(tree T) {
    if (T == NULL) {
        return T;
    }
    tree_free(T->left);
    tree_free(T->right);
    free(T -> key);
    free(T);
    return T;
}


/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}




/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
   fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
   tree_output_dot_aux(t, out);
   fprintf(out, "}\n");
}
