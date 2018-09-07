#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>

typedef struct tree_node *tree;

extern tree_new();
extern tree tree_insert(tree T, char *key);
extern int tree_search(tree T, char *key);
extern void tree_inorder(tree T, void f(char *key));
extern void tree_preorder(tree T, void f(char *key));
extern tree tree_delete(tree T, char *key); /* won't be used? */
extern tree tree_free(tree T); /* won't be used? */

#endif