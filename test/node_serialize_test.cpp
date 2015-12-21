#include "gtest/gtest.h"
#include "node.h"
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
    EXPECT_TRUE(jsonNode.HasMember("isLeaf"));
    EXPECT_TRUE(jsonNode.HasMember("name"));
    if(!jsonNode["isLeaf"].GetBool()){
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
  Node root (nodeName,Node::Type::LEAF);

  NodeSerializer::serialize(&root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);

  Node rebuildNode(readNode);
  EXPECT_EQ(root,rebuildNode);
}

TEST_F(NodeTestDumpJson, writeBranchNode) {
  using namespace rapidjson;
  using namespace webfs;

  Node root ("writeMe",Node::Type::BRANCH);
  Node dir1 ("dir1",Node::Type::BRANCH);
  Node dir1c1("childe1",Node::Type::LEAF);
  Node c1 ("childe2",Node::Type::LEAF);

  root.addChild(&c1);
  root.addChild(&dir1);
  dir1.addChild(&dir1c1);

  NodeSerializer::serialize(&root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);

  Node rebuildNode(readNode);
  EXPECT_EQ(root,rebuildNode);
}
