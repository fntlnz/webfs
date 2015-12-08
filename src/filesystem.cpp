#include "filesystem.h"

void webfs::Filesystem::createDirectory(const std::string &path) {
  webfs::Node *parent = rootNode->findParent(path);

  webfs::Node *curNode = new webfs::Node(
      webfs::utils::explode(path, '/').back(),
      webfs::Node::Type::BRANCH);
  parent->addChild(curNode);
}

void webfs::Filesystem::createFile(const std::string &path) {
  webfs::Node *parent = rootNode->findParent(path);

  webfs::Node *curNode = new webfs::Node(
      webfs::utils::explode(path, '/').back(),
      webfs::Node::Type::LEAF);
  parent->addChild(curNode);
}

webfs::Node *webfs::Filesystem::findNode(const std::string &path) {
  return rootNode->findChild(path);
}
