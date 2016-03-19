#include <numeric>

#include "node.h"

using namespace webfs;

std::weak_ptr<Node> Node::findInChildren(const std::weak_ptr<Node> &node,
        const std::string &currentName){
  auto parentPtr = node.lock();
  for (auto &node : parentPtr->getChildren()) {
    auto nodePtr = node.lock();
    if (nodePtr->getName() == currentName) {
      return node;
    }
  }
  return std::weak_ptr<Node>();
}


std::weak_ptr<Node> Node::findParent(const std::weak_ptr<Node> &root,
    const std::string &relativePath) {
  auto splittedPath = webfs::utils::explode(relativePath, '/');
  return std::accumulate(splittedPath.begin(), splittedPath.end(), root,
    [](std::weak_ptr<Node> &accumulator, std::string currentName) -> std::weak_ptr<Node> {
    auto accPtr = accumulator.lock();
      if (accPtr->getType() == Type::BRANCH) {
        std::weak_ptr<Node> c = Node::findInChildren(accumulator,currentName);
        auto cPtr = c.lock();
        if (cPtr != nullptr) {
          return c;
        }
      }
      return accumulator;
    });
}


std::weak_ptr<Node> Node::findChild(const std::weak_ptr<Node> &root,
    const std::string &relativePath) {

  if (relativePath == root.lock()->getName()) {
    return root;
  }

  auto splittedPath = webfs::utils::explode(relativePath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), root,
    [](std::weak_ptr<Node> accumulator,const std::string &currentName) -> std::weak_ptr<Node> {
    auto accPtr = accumulator.lock();
      if (accPtr == nullptr) {
        return accumulator;
      }

      if (accPtr->getType() == Type::LEAF) {
        if (accPtr->getName() == currentName) {
          return accumulator;
        }
        return std::weak_ptr<Node>();
      }

      if (accPtr->getType() == Type::BRANCH) {
        return Node::findInChildren(accumulator, currentName);
      }

      return std::weak_ptr<Node>();
    });
}
