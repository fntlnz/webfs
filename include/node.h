/**
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <experimental/optional>

#include "utils.h"
#include "file.h"

namespace webfs {

/**
 * Main Node Data structure used to store local file metadata for mapping with
 * external storage
 */
class Node {

  public:

    enum  class Type {
      BRANCH,
      LEAF,
    };

    Node(const std::string &n={}, Type t=Type::LEAF):
      name(n),parent(nullptr),children(),type(t),file(nullptr){}

    Node(Node* p,const std::string &n, Type t=Type::LEAF):
              name(n),parent(p),children(),type(t),file(nullptr){}

    std::weak_ptr<Node> createChild(const std::string &n, Type type) {
      children.push_back(std::make_shared<Node>(this,n, type));
      return children.back();
    }

    /**
     * Add a child to the current Node
     */
    void addChild(std::shared_ptr<Node> newChild){
      type = Type::BRANCH;
      children.emplace_back(newChild);
    }

    /**
     * Find the node at the provided relativePath (relative to the current node)
     * starting from the current Node.
     */

    static std::weak_ptr<Node> findChild(const std::weak_ptr<Node> &node,
    		const std::string &relativePath);
    static std::weak_ptr<Node> findParent(const std::weak_ptr<Node> &node,
    		const std::string &relativePath);

    const std::string& getName()const{
      return name;
    }

    Type getType()const {
      return type;
    }

    //TODO: ADD CONST?
    std::vector<std::weak_ptr<Node>> getChildren(){
    	std::vector<std::weak_ptr<Node>> temp(children.size());

    	std::transform(std::begin(children),std::end(children),std::begin(temp),[](std::shared_ptr<Node> &temp){
    		return std::weak_ptr<Node>(temp);
    	});

    	return temp;
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

    Node* getParent(){
      return parent;
    }

  private:

    static std::weak_ptr<Node> findInChildren(const std::weak_ptr<Node> &node,
    		const std::string &currentName);
    std::string name;
    Node *const parent;

    std::vector<std::shared_ptr<Node>> children;

    Type type;
  public:
    File *file;
}; //Node

} // webfs namespace
#endif // WEBFS_NODE_H_
