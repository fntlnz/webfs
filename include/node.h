#include <string>
#include <vector>
#include <list>
#include <functional>
#include <numeric>
#include <experimental/optional>
#include <iostream>
#include <sstream>
#include <utility>

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
  std::vector<Node> children;
  NodeType type;
};

typedef bool (*NodeFilterFunc)(Node);

std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); ) {
        result.push_back(std::move(token));
    }

    return result;
}

std::experimental::optional<Node> findInChildren(Node parent, std::string name) {
  for (auto &node : parent.children) {
    if (node.name == name) {
      return node;
    }
  }
  return std::experimental::optional<Node>();
}

std::experimental::optional<Node> findNodeByPath(Node root, std::string fullPath) {
  if (fullPath == root.name) {
    return root;
  }

  auto splittedPath = explode(fullPath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), root, [](Node &accumulator, std::string name) {
    if (accumulator.type == NodeType::LEAF) {
      if (accumulator.name == name) {
        return accumulator;
      }
    }

    if (accumulator.type == NodeType::BRANCH) {
      std::experimental::optional<Node> c = findInChildren(accumulator, name);
      if (c) {
        return c.value();
      }
    }

    return accumulator;
  }
);
};

}
#endif
