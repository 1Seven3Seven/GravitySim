#ifndef GRAVITYSIM_QUADTREENODE_H
#define GRAVITYSIM_QUADTREENODE_H

/*
 * Prefixes:
 *  QTN: Quad Tree Node
 */

/**
 * @brief
 * The different types of node a QuadTreeNode can be.
 * Should not be used outside of source.
 */
typedef enum __QuadTreeNodeType
{
    /// @brief Indicates that the node has not been setup.
    _Undefined = 0,
    /// @brief Indicates that the node contains child nodes.
    _Branch = 1,
    /// @brief Indicates that the node contains no children.
    _Leaf = 2,
} _QuadTreeNodeType;

/**
 * @brief
 * Contains data and its position.
 * Should not be used outside of source.
 */
typedef struct __QuadTreeNodeData
{
    int data;
    double x, y;
} _QuadTreeNodeData;

typedef struct _QuadTreeNode
{
    /// @brief A dynamically allocated array of length data_len.
    /// @note Only exists if this node is a leaf node.
    _QuadTreeNodeData *data;
    /// @brief The length of the data array.
    unsigned int data_len;
    /// @brief The number of slots of data in use;
    unsigned int data_in_use;

    /// @brief Indicates what type of node this is, a leaf or a branch.
    _QuadTreeNodeType type;

    /// @brief A dynamically allocated array for the children of this node indexed using the _QuadTreeChildren enum.
    /// @note Only exists if this node is a Branch node.
    struct _QuadTreeNode *children;

    /// @brief The x coordinate of the center of this node.
    double x;
    /// @brief The y coordinate of the center of this node.
    double y;
    /// @brief Half the width and height of this node's region.
    double extent;
} QuadTreeNode;

/**
 * @brief Initialise the given QuadTreeNode struct, must be called before setup.
 *
 * @param node The node to be initialised.
 */
void QTN_init(QuadTreeNode *node);

/**
 * @brief Sets up the given QuadTreeNode to be a leaf.
 *
 * @pre QTN_init must be called before calling QTN_setup.
 *
 * @param node The node to be setup.
 * @param x The x coordinate of the center of the node.
 * @param y The y coordinate of the center of the node.
 * @param extent Half the width and height of the node.
 * @param data_len The initial length of the data array.
 *
 * @retval 0 on success.
 * @retval -1 on failure due to memory allocation error.
 * @retval -2 on failure due to type not being Undefined, see precondition.
 *
 * @details
 * In the case of returning -1, the function reverts the node to just after QTN_init was called.
 * In the case of returning -2, the node will remain unchanged.
 */
int QTN_setup_leaf(QuadTreeNode *node, double x, double y, double extent, unsigned int data_len);

/**
 * @brief Frees any allocated memory in the given leaf node and returns the state to just after QTN_init was called.
 *
 * @param node The node to free any allocated memory from.
 *
 * @retval 0 on success.
 * @retval -1 on failure due to not being a leaf node.
 *
 * @details
 * In the case of returning -1, the given node will not have changed.
 */
int QTN_free_leaf(QuadTreeNode *node);

/**
 * @brief Converts the given leaf node into a branch node and moves the data into the appropriate children.
 *
 * @param node The leaf node to convert to a branch.
 *
 * @retval 0 on success.
 * @retval -1 on failure due to memory allocation error.
 * @retval -2 on failure due to receiving either an Undefined or a Branch node.
 *
 * @note
 * In the case of returning -1, the node will remain unchanged.
 * In the case of returning -2, the node will remain unchanged.
 *
 * @details
 * The children nodes inherit the parent nodes data_len member.
 */
int QTN_subdivide_leaf(QuadTreeNode *node);

/**
 * @brief Recursively frees the children then frees itself and returns the state to just after QTN_init was called.
 *
 * @param node The node to free.
 *
 * @retval 0 on success.
 * @retval -1 on failure due to not being a Branch node.
 *
 * @details
 * In the case of returning -1, the given node will not have changed.
 *
 * @paragraph Implementation
 * When performing the recursive free, if an Undefined child is encountered it is skipped.
 * Undefined nodes are assumed to have no allocated memory, as they should only exist after init and before setup, or
 *  after free.
 */
int QTN_free_branch(QuadTreeNode *node);

#endif //GRAVITYSIM_QUADTREENODE_H
