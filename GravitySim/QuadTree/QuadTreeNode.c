#include "QuadTreeNode.h"

#include <stdlib.h>

/**
 * @brief
 * Indexes into the child array given a name.
 */
typedef enum _QuadTreeChildren
{
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3,
} QuadTreeChildren;

void QTN_init(QuadTreeNode *node)
{
    node->data = NULL;
    node->data_len = 0;
    node->data_in_use = 0;
    node->type = _Undefined;
    node->children = NULL;
}

int QTN_setup_leaf(QuadTreeNode *node, double x, double y, double extent, unsigned int data_len)
{
    if (node->type != _Undefined) return -2;

    node->data = malloc(sizeof(_QuadTreeNodeData) * data_len);
    if (node->data == NULL) return -1;

    node->type = _Leaf;
    node->x = x;
    node->y = y;
    node->extent = extent;
    node->data_len = data_len;

    return 0;
}

/*
 * Does not touch the children member as this assumes a leaf node.
 */
int QTN_free_leaf(QuadTreeNode *node)
{
    if (node->type != _Leaf) return -1;

    if (node->data != NULL) free(node->data);
    node->data = NULL;

    node->data_len = 0;
    node->data_in_use = 0;
    node->type = _Undefined;

    return 0;
}

static inline int
/**
 * Checks if the given coordinates exist inside the given node
 */
coordinates_in_node(QuadTreeNode *node, double x, double y)
{
    if (node->x - node->extent < x && x < node->x + node->extent)
    {
        if (node->y - node->extent < y && y < node->y + node->extent)
        {
            return 1;
        }
    }

    return 0;
}

static inline int
/**
 * Checks if data can be added safely into the given node without needing to allocate more space.
 */
space_available_in_node(QuadTreeNode *node)
{
    if (node->data_in_use < node->data_len) return 1;

    return 0;
}

int QTN_add_data_to_leaf(QuadTreeNode *node, int data, double x, double y, int allow_resize)
{
    // Make sure adding to leaf
    if (node->type != _Leaf) return -1;

    // Check leaf can contain data
    if (!coordinates_in_node(node, x, y)) return -2;

    // Check if there is space
    if (!space_available_in_node(node))
    {
        if (!allow_resize) return -3;

        // Resize time, resize to double of the current length
        _QuadTreeNodeData *new_data = realloc(node->data, sizeof(_QuadTreeNodeData) * node->data_len * 2);

        if (new_data == NULL) return -4;

        // Success
        node->data_len *= 2;
        node->data = new_data;
    }

    // Create the data struct
    _QuadTreeNodeData data_struct = {
        .data = data,
        .x = x,
        .y = y,
    };

    // Add the data
    node->data[node->data_in_use] = data_struct;
    node->data_in_use++;

    // All done
    return 0;
}

static inline void
/**
 * Helper function for QTN_subdivide_leaf.
 */
add_data_to_child(QuadTreeNode *node, _QuadTreeNodeData new_data, QuadTreeChildren child)
{
    node->children[child].data[node->children[child].data_in_use] = new_data;
    node->children[child].data_in_use++;
}

static inline QuadTreeChildren

/**
 * Helper function for QTN_subdivide_leaf.
 */
choose_child(double parent_x, double parent_y, double data_x, double data_y)
{
    return (data_y > parent_y) * 2 + (data_x > parent_x);
}

int QTN_subdivide_leaf(QuadTreeNode *node)
{
    if (node->type != _Leaf) return -2;

    node->children = malloc(sizeof(QuadTreeNode) * 4);
    if (node->children == NULL) return -1;

    // Initialise first
    for (int i = 0; i < 4; i++)
    {
        QTN_init(&node->children[i]);
    }

    double children_extent = node->extent / 2;
    unsigned int child_data_len = node->data_len;
    int result;

    // Setup each child

    result = QTN_setup_leaf(&node->children[TopLeft],
                            node->x - children_extent, node->y + children_extent,
                            children_extent,
                            child_data_len);
    if (result) goto cleanup;

    result = QTN_setup_leaf(&node->children[TopRight],
                            node->x + children_extent, node->y + children_extent,
                            children_extent,
                            child_data_len);
    if (result) goto cleanup;

    result = QTN_setup_leaf(&node->children[BottomLeft],
                            node->x - children_extent, node->y - children_extent,
                            children_extent,
                            child_data_len);
    if (result) goto cleanup;

    result = QTN_setup_leaf(&node->children[BottomRight],
                            node->x + children_extent, node->y - children_extent,
                            children_extent,
                            child_data_len);
    if (result) goto cleanup;

    // Now move this node's children appropriately
    for (unsigned int i = 0; i < node->data_in_use; i++)
    {
        add_data_to_child(
            node,
            node->data[i],
            choose_child(node->x, node->y, node->data[i].x, node->data[i].y)
        );
    }

    // New type
    node->type = _Branch;

    // Finally, we free this node's data array
    free(node->data);
    node->data_len = 0;  // No longer any data memory

    return 0;

    cleanup:
    // Free all the node's memory
    for (int i = 0; i < 4; i++)
    {
        QTN_free_leaf(&node->children[i]);
    }
    // Free the memory containing all the nodes
    free(node->children);
    node->children = NULL;

    return -1;
}

/*
 * Does not touch the data members as this assumes a branch node.
 */
int QTN_free_branch(QuadTreeNode *node)
{
    if (node->type != _Branch) return -1;

    if (node->children != NULL)
    {
        // Free each child
        for (int i = 0; i < 4; i++)
        {
            QuadTreeNode *child_node = &node->children[i];

            switch (child_node->type)
            {
            case _Leaf:
                QTN_free_leaf(child_node);
                break;

            case _Branch:
                QTN_free_branch(child_node);
                break;

            case _Undefined: // Skip this node, assuming already freed
                break;
            }
        }
        free(node->children);
        node->children = NULL;
    }

    node->type = _Undefined;

    return 0;
}
