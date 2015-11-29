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

class Node {
 public:
  std::string name;
  std::vector<Node *> children;
  NodeType type;
};

typedef bool (*NodeFilterFunc)(Node);

Node *findNodeByPath(Node *root, std::string fullPath);
}
#endif
