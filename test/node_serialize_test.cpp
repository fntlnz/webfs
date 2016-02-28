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
  Node root (nodeName);

  NodeSerializer::serialize(root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);

  Node rebuiltNode = NodeSerializer::unserialize(readNode);
  EXPECT_EQ(root, rebuiltNode);

}

TEST_F(NodeTestDumpJson, writeBranchNode) {
  using namespace rapidjson;
  using namespace webfs;

  Node root ("writeMe");
  Node dir1 ("dir1");
  Node dir1c1("childe1");
  Node c1 ("childe2");

  root.createChild("child1");
  root.createChild("dir1").createChild("child2");

  NodeSerializer::serialize(root, *writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkHasAllField(readNode);
  
  Node rebuiltNode = NodeSerializer::unserialize(readNode);
  EXPECT_EQ(root, rebuiltNode);
}

