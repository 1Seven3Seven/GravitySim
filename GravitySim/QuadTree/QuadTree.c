#include "QuadTree.h"

#include <stdlib.h>

void QT_init(QuadTree *tree)
{
    tree->root = NULL;
}

int QT_setup(QuadTree *tree,
             double x, double y, double extent,
             unsigned int initial_data_len, unsigned int max_data_len,
             unsigned int max_depth)
{
    // Check for correct initial data len
    if (initial_data_len == 0) return -2;

    // Allocate root
    tree->root = malloc(sizeof(QuadTreeNode));
    if (tree->root == NULL) return -1;

    // Setup and initialise root as a leaf
    QTN_init(tree->root);
    int setup_result = QTN_setup_leaf(tree->root, x, y, extent, 2);
    // Not worrying about -2 as that should only occur if init is not called before setup
    if (setup_result == -1)
    {
        free(tree->root);
        tree->root = NULL;
        return -1;
    }

    tree->initial_data_len = initial_data_len;
    tree->max_data_len = max_data_len;
    tree->max_depth = max_depth;

    return 0;
}

void QT_free(QuadTree *tree)
{
    // Nothing to be done if there is no root
    if (tree->root == NULL) return;

    // Else we free the root depending on its type
    switch (tree->root->type)
    {
    case _Leaf:
        QTN_free_leaf(tree->root);
        break;

    case _Branch:
        QTN_free_branch(tree->root);
        break;

    case _Undefined:  // Nothing to be done here as undefined means no allocated memory
        break;
    }

    // Finally free the root itself
    free(tree->root);
    tree->root = NULL;
}
