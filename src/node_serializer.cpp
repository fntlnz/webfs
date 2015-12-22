#include "node_serializer.h"

using namespace webfs;

Node *NodeSerializer::unserialize(const rapidjson::Value &jsonNode) {
  using namespace rapidjson;
  Node *root = new Node(jsonNode["name"].GetString(), extractType(jsonNode));

  if(jsonNode.HasMember("children")){
    const rapidjson::Value &childrenJson = jsonNode["children"];
    if(childrenJson.Size() != 0){
      //children.reserve(childrenJson.Size());
      for(decltype(childrenJson.Size()) i=0; i<childrenJson.Size(); i++) {
        root->addChild(unserialize(childrenJson[i]));
      }
    }
  }
  return root;
}
