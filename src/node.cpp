#include "node.h"

using namespace webfs;

Node *findChildren(Node *parent, std::string name) {
  for (auto &node : parent->children) {
    if (node->name == name) {
      return node;
    }
  }
  return nullptr;
};

Node *webfs::findNodeByPath(Node *root, std::string fullPath) {
  if (fullPath == root->name) {
    return root;
  }

  auto splittedPath = webfs::utils::explode(fullPath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), root, [](Node *accumulator, std::string name) {
    if (accumulator->type == NodeType::LEAF) {
      if (accumulator->name == name) {
        return accumulator;
      }
    }

    if (accumulator->type == NodeType::BRANCH) {
      Node *c = findChildren(accumulator, name);
      if (c) {
        return c;
      }
    }

    return accumulator;
  }
);
}
