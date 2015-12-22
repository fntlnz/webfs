#include "node.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#ifndef WEBFS_NODE_SERIALIZER_H_
#define WEBFS_NODE_SERIALIZER_H_

namespace webfs {

static inline Node::Type extractType(const rapidjson::Value &jsonNode){
  if(jsonNode["isLeaf"].GetBool())
    return Node::Type::LEAF;
  else
    return Node::Type::BRANCH;
}

class NodeSerializer {
  public:
    template<typename BufferType>
    static void serialize(const Node *node, rapidjson::Writer<BufferType> &out) {
      out.StartObject();
      out.String("isLeaf");
      const bool isLeaf= node->getType() == Node::Type::LEAF;
      out.Bool(isLeaf);
      out.String("name");
      out.String(node->getName());

      if(!isLeaf){
        out.String("children");
        out.StartArray();
        for(const Node *n : node->getChildren()) {
          NodeSerializer::serialize(n, out);
        }
        out.EndArray();
      }//if !isLeaf

      out.EndObject();
    }

    static Node *unserialize(const rapidjson::Value &jsonNode);
};
} // webfs namespace
#endif // WEBFS_NODE_SERIALIZER_H_
