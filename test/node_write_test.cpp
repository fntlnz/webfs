#include "gtest/gtest.h"
#include "node.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

class NodeTestDumpJson : public ::testing::Test{

public:

  rapidjson::GenericStringBuffer<rapidjson::UTF8<>> buffer;
  rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>>* writer;

  virtual void SetUp(){
    writer = new rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>>(buffer);
   }

  virtual void TearDown(){
    delete writer;
  }

  void checkIsSameNode(const webfs::Node &node,const rapidjson::Value &jsonNode){
    const bool isLeaf =node.getType()==webfs::Node::Type::LEAF;
	EXPECT_TRUE(jsonNode.HasMember("isLeaf"));
	EXPECT_EQ(isLeaf,jsonNode["isLeaf"].GetBool());
	EXPECT_TRUE(jsonNode.HasMember("name"));
	EXPECT_EQ(node.getName(),
			jsonNode["name"].GetString());
	EXPECT_TRUE(jsonNode.HasMember("name"));
	if(!isLeaf){
	  EXPECT_TRUE(jsonNode.HasMember("children"));
	  const std::vector<webfs::Node*>  childrenNode = node.getChildren();
	  const rapidjson::Value &childrenJson = jsonNode["children"];
	  EXPECT_TRUE(childrenJson.IsArray());
	  EXPECT_EQ(childrenNode.size(),childrenJson.Size());
	  for(uint32_t i=0; i<childrenNode.size();i++)
	    checkIsSameNode(*childrenNode[i],childrenJson[i]);
	}//if
  }//isSameNode

protected:

};

TEST_F(NodeTestDumpJson, writeLeafNode) {
  using namespace rapidjson;
  using namespace webfs;

  const std::string nodeName("writeMe");
  Node root (nodeName,Node::Type::LEAF);

  root.writeTo(*writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkIsSameNode(root,readNode);

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

  root.writeTo(*writer);

  //std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());

  checkIsSameNode(root,readNode);

}



