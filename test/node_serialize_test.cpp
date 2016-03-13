#include "gtest/gtest.h"
#include "node.h"
#include "node_serializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

class NodeTestDumpJson : public ::testing::Test{

public:

  rapidjson::GenericStringBuffer<rapidjson::UTF8<>> buffer;
  rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>>* writer;

  virtual void SetUp() {
    writer = new rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>>(buffer);
  }

  virtual void TearDown() {
    delete writer;
  }

  void checkHasAllField(const rapidjson::Value &jsonNode) {
    //EXPECT_TRUE(jsonNode.HasMember("isLeaf"));
    EXPECT_TRUE(jsonNode.HasMember("name"));
    if(jsonNode.HasMember("children")){
      EXPECT_TRUE(jsonNode.HasMember("children"));
      const rapidjson::Value &childrenJson = jsonNode["children"];
      EXPECT_TRUE(childrenJson.IsArray());
      for(decltype(childrenJson.Size()) i=0; i<childrenJson.Size();i++)
        checkHasAllField(childrenJson[i]);
    }// if
  }// isSameNode
};

TEST_F(NodeTestDumpJson, writeLeafNode) {
  using namespace rapidjson;
  using namespace webfs;

  const std::string nodeName("writeMe");
  Node* root = new Node(nodeName, Node::Type::BRANCH);

  NodeSerializer::serialize(root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);

  Node *rebuiltNode = NodeSerializer::unserialize(readNode);
  EXPECT_EQ(*root, *rebuiltNode);

}

TEST_F(NodeTestDumpJson, writeBranchNode) {
  using namespace rapidjson;
  using namespace webfs;

  Node* root = new Node("writeMe", Node::Type::BRANCH);

  root->createChild("child1", Node::Type::LEAF);
  root->createChild("dir1", Node::Type::BRANCH)
    ->createChild("child2", Node::Type::LEAF);

  NodeSerializer::serialize(root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);

  Node *rebuiltNode = NodeSerializer::unserialize(readNode);
  EXPECT_EQ(*root, *rebuiltNode);
}

