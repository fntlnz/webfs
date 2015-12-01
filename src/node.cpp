#include "node.h"

using namespace webfs;

Node *findInChildren(Node *parent, std::string currentName) {
  for (auto &node : parent->children) {
    if (node->name == currentName) {
      return node;
    }
  }
  return nullptr;
};

void Node::addChild(Node *child) {
  children.push_back(child);
  child->parent = this;
}

Node *Node::findClosestParent(std::string relativePath) {
  auto splittedPath = webfs::utils::explode(relativePath, '/');
  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
      [](Node *accumulator, std::string currentName) {
        auto node = accumulator->findChild(accumulator->name + '/' + currentName);
        if (!node) {
          return accumulator;
        }
        return node;
      });
}

Node *Node::findChild(std::string relativePath) {
  if (relativePath == this->name) {
    return this;
  }

  auto splittedPath = webfs::utils::explode(relativePath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
      [](Node *accumulator, std::string currentName) {
        if (accumulator->type == NodeType::LEAF) {
          if (accumulator->name == currentName) {
            return accumulator;
          }
        }

        if (accumulator->type == NodeType::BRANCH) {
          Node *c = findInChildren(accumulator, currentName);
          if (c) {
            return c;
          }
        }

        return accumulator;
      });
}
