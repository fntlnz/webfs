#include "node.h"

using namespace webfs;

static Node *findInChildren(Node *parent, std::string currentName) {
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

Node *Node::findParent(std::string relativePath) {
  auto lastName = webfs::utils::explode(relativePath, '/').back();
  relativePath.erase(relativePath.length() - lastName.length(), lastName.length());
  return findChild(relativePath);
}

Node *Node::findChild(std::string relativePath) {
  if (relativePath == this->name) {
    return this;
  }

  auto splittedPath = webfs::utils::explode(relativePath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
      [](Node *accumulator, std::string currentName) -> Node * {
        if (accumulator == NULL) {
          return accumulator;
        }
        if (accumulator->type == NodeType::LEAF) {
          if (accumulator->name == currentName) {
            return accumulator;
          }
          return NULL;
        }

        if (accumulator->type == NodeType::BRANCH) {
          Node *c = findInChildren(accumulator, currentName);
          if (c) {
            return c;
          }
        }

        return NULL;
      });
}
