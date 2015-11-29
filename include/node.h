/*!
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#include <string>
#include <vector>
#include <numeric>

#include "utils.h"

#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_
namespace webfs {

enum NodeType {
  BRANCH,
  LEAF,
};

/*!
 * Main Node Data structure used to store local file metadata
 * for mapping with the external storage
 */
class Node {
 public:
  std::string name;
  std::vector<Node *> children;
  NodeType type;
};

typedef bool (*NodeFilterFunc)(Node);

/*! 
 * \brief Look for the Node at the given path recursively starting from the provided root
 */
Node *findNodeByPath(Node *root, std::string fullPath);
}
#endif
