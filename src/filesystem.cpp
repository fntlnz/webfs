#include "filesystem.h"

using namespace webfs;

Node *Filesystem::createDirectory(const std::string &path) {
  Node *parent = rootNode->findParent(path);

  Node *curNode = new Node(
      utils::explode(path, '/').back(),
      Node::Type::BRANCH);
  parent->addChild(curNode);
  return curNode;
}

Node *Filesystem::createFile(const std::string &path) {
  Node *parent = rootNode->findParent(path);

  Node *curNode = new Node(
      utils::explode(path, '/').back(),
      Node::Type::LEAF);
  parent->addChild(curNode);
  return curNode;
}

Node *Filesystem::findNode(const std::string &path) {
  return rootNode->findChild(path);
}
