#include "node.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#ifndef WEBFS_NODE_SERIALIZER_H_
#define WEBFS_NODE_SERIALIZER_H_

namespace webfs {

class NodeSerializer {
  public:
	//TODO ADD CONST TO NODE?
    template<typename BufferType>
    static void serialize( Node &node, rapidjson::Writer<BufferType> &out) {
      out.StartObject();
      out.String(IS_LEAF_TAG);
      const bool isLeaf= node.getType() == Node::Type::LEAF;
      out.Bool(isLeaf);
      out.String(NAME_TAG);
      out.String(node.getName());

      if(!isLeaf){
        out.String(CHILDREN_TAG);
        out.StartArray();
        for(auto n : node.getChildren()) {
          NodeSerializer::serialize(*n.lock(), out);
        }
        out.EndArray();
      }//if !isLeaf

      out.EndObject(); 
    }//serialize

    static std::shared_ptr<Node> unserialize(const rapidjson::Value &jsonNode);

  private:

    /*
    static inline Node::Type extractType(const rapidjson::Value &jsonNode){
      if(jsonNode[IS_LEAF_TAG].GetBool())
        return Node::Type::LEAF;
      else
        return Node::Type::BRANCH;
    }
    */

    const static std::string NAME_TAG;
    const static std::string IS_LEAF_TAG;
    const static std::string CHILDREN_TAG;
};
} // webfs namespace
#endif // WEBFS_NODE_SERIALIZER_H_
