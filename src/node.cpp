#include <numeric>

#include "node.h"

using namespace webfs;

Node *Node::findInChildren(const std::string &currentName) {
  for (auto &node : children) {
    if (node->name == currentName) {
      return node;
    }
  }
  return nullptr;
}

void Node::addChild(Node *child) {
  children.push_back(child);
  child->parent = this;
}

Node *Node::findParent(const std::string &relativePath) {
  auto splittedPath = webfs::utils::explode(relativePath, '/');
  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
    [](Node *accumulator, std::string currentName) -> Node * {
      if (accumulator->type == Type::BRANCH) {
        auto c = accumulator->findInChildren(currentName);
        if (c != nullptr) {
          return c;
        }
      }
      return accumulator;
    });
}

Node *Node::findChild(const std::string &relativePath) {
  if (relativePath == this->name) {
    return this;
  }

  auto splittedPath = webfs::utils::explode(relativePath, '/');

  return std::accumulate(splittedPath.begin(), splittedPath.end(), this,
    [](Node *accumulator,const std::string &currentName) -> Node * {
      if (accumulator == nullptr) {
        return accumulator;
      }
      if (accumulator->type == Type::LEAF) {
        if (accumulator->name == currentName) {
          return accumulator;
        }
        return nullptr;
      }

      if (accumulator->type == Type::BRANCH) {
        Node *c = accumulator->findInChildren( currentName);
        if (c != nullptr) {
          return c;
        }
      }

      return nullptr;
    });
}

//rapidjson::Document& operator<<(rapidjson::Document &out,Node &node){
void Node::write(rapidjson::Document& out){
	using namespace rapidjson;
	auto& allocator=out.GetAllocator();
	Value isLeaf(type == Node::Type::LEAF);
	out.AddMember("isLeaf",isLeaf,allocator);
	Value nodeName;
	nodeName.SetString(name.c_str(),name.size(),allocator);
	out.AddMember("name",nodeName,allocator);
	if(type==Node::Type::BRANCH){
		Value jsonChild(kArrayType);
		for(Node *child : children){
			child->write(out);
			//jsonChild.PushBack(out,allocator);
		}
		out.AddMember("Children",jsonChild,allocator);
	}
}

