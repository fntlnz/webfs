#include <cassert>
#include "filesystem.h"

using namespace webfs;

std::weak_ptr<Node> Filesystem::createElementDirectory(const std::string &path) {
  auto parent = Node::findParent(rootNode,path);
  assert(parent.lock()!=nullptr);
  return parent.lock()->createChild(utils::explode(path, '/').back(), Node::Type::BRANCH);
}


std::weak_ptr<Node> Filesystem::createElementFile(const std::string &path) {
  //TODO merge with createElementDirectory??
  auto parent = Node::findParent(rootNode,path);
  assert(parent.lock()!=nullptr);
  return parent.lock()->createChild(utils::explode(path, '/').back(), Node::Type::LEAF);
}

int Filesystem::writeChunk(const std::string &path, const char *buf, size_t size, off_t offset) {
  auto node = Node::findParent(rootNode,path);
  auto nodePtr = node.lock();
  if (nodePtr->file == nullptr) {
    nodePtr->file = new File();
  }

  // If offset == 0 and node is not empty means that the file is being
  // overwritten, create a new one in such case
  if (offset == 0 && !nodePtr->file->empty()) {
    nodePtr->file = new File();
  }


  // TODO: write the buffer somewhere, the idea is to move all chunks to a
  // local cache and then write to the remote storage N chunks togheter in order
  // to minimize calls to remote systems.
  auto buffer = std::vector<char>(buf+offset,buf+offset+size);

  //TODO: the chunk reference must be saved to an internal database in order to
  //be used as chunk identifier

  auto fileChunk = new FileChunk();
  fileChunk->size = size;
  fileChunk->offset = offset;
  fileChunk->identifier = storage->write(buffer);
  nodePtr->file->addChunk(fileChunk);

  return size;
}

std::weak_ptr<Node> Filesystem::findNode(const std::string &path) {
  return Node::findChild(rootNode,path);
}
