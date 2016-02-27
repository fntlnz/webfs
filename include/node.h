/**
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_

#include <string>
#include <vector>

#include "utils.h"
#include "file.h"

namespace webfs {

/**
 * Main Node Data structure used to store local file metadata for mapping with
 * external storage
 */
class Node {
  public:
    enum class Type {
      BRANCH,
      LEAF,
    };

    Node(const std::string &n):
      name(n),parent(nullptr){}

    /**
     * Add a child to the current Node
     */
    //TODO(wise): how we handle the pointer? who have to free it?
    //ad a function create Child and do the allocation inside the class?
    void addChild(Node *child);

    /**
     * Find the node at the provided relativePath (relative to the current node)
     * starting from the current Node.
     */
    //TODO(wise): return a const reference?
    Node *findChild(const std::string &relativePath);
    //TODO(wise): return a const reference?
    Node *findParent(const std::string &relativePath);

    const std::string& getName()const{
      return name;
    }

    const Type getType()const {
      return children.size()==0 ? Type::LEAF : Type::BRANCH;
    }

    const std::vector<Node*> getChildren()const{
      return children;
    }

    /**
     * Check if two nodes are equal comparing their properties and children
     */
    bool operator==(const Node &other)const {
      bool nodeAreEqual = (name==other.name  &&
          children.size()==other.children.size());

      if(!nodeAreEqual){
        return false;
      }

      // check if children are equal
      for(auto i=0u; i< children.size();i++){
        if ((*children[i]) != (*other.children[i])) {
          return false;
        }
      }// for

      return true;
    }

    bool operator!=(const Node &other) const{
      return !(*this == other);
    }

    File *file;

  private:

    Node* findInChildren(const std::string &currentName);
    std::string name;
    Node *parent;

    std::vector<Node *> children;
}; //Node

} // webfs namespace
#endif // WEBFS_NODE_H_
