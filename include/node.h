/**
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_

#include <iostream>
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

    Node(const std::string &n={}):
      name(n),parent(nullptr),children(),file(nullptr){
    	std::cout<<"create node"<<std::endl;
    }

    Node(Node* p,const std::string &n):
              name(n),parent(p),children(),file(nullptr){
    	std::cout<<"create child"<<n<<std::endl;
    }

    /**
     * Add a child to the current Node
     */
    Node& createChild(const std::string &n){
    	children.emplace_back(this,n);
    	return children.back();
    }

    void addChild(Node &&newChild){
		children.push_back(newChild);
    }

    /**
     * Find the node at the provided relativePath (relative to the current node)
     * starting from the current Node.
     */

    Node* findChild(const std::string &relativePath);
    Node* findParent(const std::string &relativePath);

    const std::string& getName()const{
      return name;
    }

    const Type getType()const {
      return children.size()==0 ? Type::LEAF : Type::BRANCH;
    }

    const std::vector<Node>& getChildren()const{
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
        if ((children[i]) != (other.children[i])) {
          return false;
        }
      }// for

      return true;
    }

    bool operator!=(const Node &other) const{
      return !(*this == other);
    }

    Node* getParent(){
    	return parent;
    }

  private:

    Node* findInChildren(const std::string &currentName);
    std::string name;
    Node *const parent;

    std::vector<Node> children;
  public:
    File *file;
}; //Node

} // webfs namespace
#endif // WEBFS_NODE_H_
