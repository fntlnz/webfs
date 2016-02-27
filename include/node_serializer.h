#include "node.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#ifndef WEBFS_NODE_SERIALIZER_H_
#define WEBFS_NODE_SERIALIZER_H_

namespace webfs {

class NodeSerializer {
  public:
    template<typename BufferType>
    static void serialize(const Node &node, rapidjson::Writer<BufferType> &out) {
      out.StartObject();
      out.String(NAME_TAG);
      out.String(node.getName());

      if(node.getType()!=Node::Type::LEAF){
        out.String(CHILDREN_TAG);
        out.StartArray();
        for(const Node *n : node.getChildren()) {
          NodeSerializer::serialize(*n, out);
        }
        out.EndArray();
      }//if !isLeaf

      out.EndObject();
    }//serialize

    static Node *unserialize(const rapidjson::Value &jsonNode);

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
