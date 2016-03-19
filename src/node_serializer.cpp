#include "node_serializer.h"

using namespace webfs;

const std::string NodeSerializer::NAME_TAG("name");
const std::string NodeSerializer::IS_LEAF_TAG("isLeaf");
const std::string NodeSerializer::CHILDREN_TAG("children");

std::shared_ptr<Node> NodeSerializer::unserialize(const rapidjson::Value &jsonNode) {
  using namespace rapidjson;

  std::shared_ptr<Node> root = std::make_shared<Node>(jsonNode[NAME_TAG].GetString(),extractType(jsonNode));

  if(jsonNode.HasMember(CHILDREN_TAG)){
    const rapidjson::Value &childrenJson = jsonNode[CHILDREN_TAG];
    if(childrenJson.Size() != 0){
      //children.reserve(childrenJson.Size());
      for(decltype(childrenJson.Size()) i=0; i<childrenJson.Size(); i++) {
        root->addChild(unserialize(childrenJson[i]));
      }
    }
  }
  return root;
}
