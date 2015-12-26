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

int Filesystem::writeChunk(const std::string &path, const char *buf, size_t size, off_t offset) {
  auto node = rootNode->findParent(path);
  if (node->file == nullptr) {
    node->file = new File();
  }

  // If offset == 0 and node is not empty means that the file is being
  // overwritten, create a new one in such case
  if (offset == 0 && !node->file->empty()) {
    node->file = new File();
  }


  // TODO: write the buffer somewhere, the idea is to move all chunks to a
  // local cache and then write to the remote storage N chunks togheter in order
  // to minimize calls to remote systems.
  auto buffer = std::vector<char>(buf+offset,buf+offset+size);
  std::string chunkReference = storage->write(buffer);

  //TODO: the chunk reference must be saved to an internal database in order to
  //be used as chunk identifier

  auto fileChunk = new FileChunk();
  fileChunk->size = size;
  fileChunk->offset = offset;
  fileChunk->identifier = 1; // TODO: identifier based on used storage adapter + real location
  node->file->addChunk(fileChunk);

  return size;
}

Node *Filesystem::findNode(const std::string &path) {
  return rootNode->findChild(path);
}
