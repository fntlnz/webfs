/**
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

/**
 * Main Node Data structure used to store local file metadata for mapping with
 * external storage
 */
class Node {
 public:
  std::string name;
  Node *parent;
  NodeType type;
  std::vector<Node *> children;

  /**
   * Add a child to the current Node
   */
  void addChild(Node *child);

  /**
   * Find the node at the provided relativePath (relative to the current node)
   * starting from the current Node.
   */
  Node *findChild(std::string relativePath);

  Node *findParent(std::string relativePath);
};
}
#endif
