#include <numeric>

#include "node.h"

using namespace webfs;

Node* Node::findInChildren(const std::string &currentName) {
  for (Node* node : children) {
    if (node->getName() == currentName) {
      return node;
    }
  }
  return nullptr;
}


Node* Node::findParent(const std::string &relativePath) {
  auto splittedPath = webfs::utils::explode(relativePath, '/');
  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
    [](Node *accumulator, std::string currentName) -> Node* {
      if (accumulator->getType() == Node::Type::BRANCH) {
        Node *c = accumulator->findInChildren(currentName);
        if (c != nullptr) {
          return c;
        }
      }
      return accumulator;
    });
}

Node* Node::findChild(const std::string &relativePath) {
  if (relativePath == this->getName()) {
    return this;
  }

  auto splittedPath = webfs::utils::explode(relativePath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
    [](Node *accumulator,const std::string &currentName) -> Node* {
      if (accumulator == nullptr) {
        return accumulator;
      }
      if (accumulator->getType() == Node::Type::LEAF) {
        if (accumulator->name == currentName) {
          return accumulator;
        }
        return nullptr;
      }

      if (accumulator->getType() == Node::Type::BRANCH) {
        Node *c = accumulator->findInChildren( currentName);
        if (c != nullptr) {
          return c;
        }
      }

      return nullptr;
    });
}

