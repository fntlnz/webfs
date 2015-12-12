/**
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "utils.h"


#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_
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

    Node(const std::string &n,const Type t):
      name(n),parent(nullptr),type(t){}

  /**
   * Add a child to the current Node
   */
  void addChild(Node *child);

  /**
   * Find the node at the provided relativePath (relative to the current node)
   * starting from the current Node.
   */
  Node *findChild(const std::string &relativePath);

  Node *findParent(const std::string &relativePath);

  const std::string& getName()const{
    return name;
  }

  const Type& getType()const {
    return type;
  }

  const std::vector<Node*> getChildren(){
    return children;
  }

  template<typename Writer>
  void serialize(Writer &out)const{
	  out.StartObject();
	  out.String("isLeaf");
	  out.Bool(type==Type::LEAF);
	  out.String("name");
	  out.String(name.c_str(),name.length(),true);
	  out.String("id");
	  out.String(remoteId.c_str(),remoteId.length(),true);

	  if(type==Type::BRANCH){
		  out.String("children");
		  out.StartArray();
		  for(Node *n : children){
			  n->serialize(out);
		  }
		  out.EndArray();
	  }

	  out.EndObject();
  }

  void setRemoteId(const std::string &id){
	  remoteId=id;
  }

  void write(rapidjson::Document &doc);
  //friend rapidjson::Document& operator<<(rapidjson::Document &out,Node &node);
  //friend std::ostream& operator<<(std::ostream &out, const Node &node);

  private:

    Node* findInChildren(const std::string &currentName);

    std::string name;
    std::string remoteId;
    Node *parent;
    const Type type;
    std::vector<Node *> children;

}; //Node




} // webfs namespace
#endif // WEBFS_NODE_H_
