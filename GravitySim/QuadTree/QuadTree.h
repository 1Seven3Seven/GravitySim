#ifndef GRAVITYSIM_QUADTREE_H
#define GRAVITYSIM_QUADTREE_H

#include "QuadTreeNode.h"

/*
 * Prefixes:
 *  QT: Quad Tree
 */

/**
 * @brief
 * The structure for a quad tree.
 */
typedef struct _QuadTree
{
    /**
     * @brief The root of the quad tree, where it all begins.
     *
     * @note This pointer may change throughout the existence of this quad tree.
     */
    QuadTreeNode *root;

    /**
     * @brief The initial length of the data array of a leaf node.
     *
     * @note
     * Cannot be 0.
     * Best to keep this a power of 2.
     */
    unsigned int initial_data_len;

    /**
     * @brief The maximum amount of data per node before subdividing.
     *
     * @note
     * A value of 0 means there is no maximum and thus no automatic subdividing will happen.
     * Best to keep this a power of 2.
     */
    unsigned int max_data_len;

    /**
     * @brief The maximum depth from the root the quad tree can be.
     *
     * @note
     * A value of 0 means there is no max depth.
     * A value of 1 means only the root node and so on.
     */
    unsigned int max_depth;
} QuadTree;

/**
 * @brief Initialise the given QuadTree struct, must be called before setup.
 *
 * @param tree The tree to initialise.
 */
void QT_init(QuadTree *tree);

/**
 * @brief
 *
 * @param tree
 * @param x
 * @param y
 * @param extent
 * @param initial_data_len
 * @param max_data_len
 * @param max_depth
 *
 * @retval 0 on success.
 * @retval -1 on failure due to memory allocation error.
 * @retval -2 on failure due to receiving an incorrect initial_data_len value of 0.
 */
int QT_setup(QuadTree *tree,
             double x, double y, double extent,
             unsigned int initial_data_len, unsigned int max_data_len,
             unsigned int max_depth);

/**
 * @brief Frees any allocated memory in the given tree and returns the state to just after init was called.
 *
 * @param tree The tree to be freed.
 */
void QT_free(QuadTree *tree);

#endif //GRAVITYSIM_QUADTREE_H
