#include "gtest/gtest.h"
#include "node.h"

TEST(NodeTest, TestAddChild) {
  using namespace webfs;

  Node root("",Node::Type::BRANCH);

  Node child("file.txt",Node::Type::LEAF);

  root.addChild(&child);

  EXPECT_EQ(1u, root.getChildren().size());
  EXPECT_EQ(&child, root.getChildren().front());

}

TEST(NodeTest, TestFindParent) {
  using namespace webfs;

  Node root("",Node::Type::BRANCH);
  Node folder( "folder",Node::Type::BRANCH);

  root.addChild(&folder);

  auto *parent = root.findParent("/example.txt");
  EXPECT_EQ(parent, &root);

  auto *folderParent = root.findParent("/folder/example.txt");
  EXPECT_EQ(folderParent, &folder);

  auto *closestFolderSubParent = root.findParent("/folder/a/non-existing/strange/superlong/and/drammatically/silly/folder/containing/a/file.txt");

  EXPECT_EQ(&folder, closestFolderSubParent);
}


TEST(NodeTest, TestFindChild) {
  using namespace webfs;

  Node root ("",Node::Type::BRANCH);

  Node folder("folder",Node::Type::BRANCH);

  Node folder2("folder2", Node::Type::BRANCH);

  Node subfolder("subfolder",Node::Type::BRANCH);

  Node file("myawesome.txt",Node::Type::LEAF);

  subfolder.addChild(&file);
  folder.addChild(&subfolder);
  root.addChild(&folder);
  root.addChild(&folder2);

  auto *found = root.findChild("/folder/subfolder/myawesome.txt");
  EXPECT_EQ(found, &file);

  auto *foundInFolder = folder.findChild("/subfolder/myawesome.txt");
  EXPECT_EQ(foundInFolder, &file);

  auto *foundInSubfolder = subfolder.findChild("/myawesome.txt");
  EXPECT_EQ(foundInSubfolder, &file);

  auto *foundFolder = root.findChild("/folder2");
  EXPECT_EQ(foundFolder, &folder2);
}

#include "rapidjson/writer.h"

TEST(NodeTest, writeLeafNode) {
  using namespace rapidjson;
  using namespace webfs;

  const std::string nodeName("writeMe");
  Node root (nodeName,Node::Type::LEAF);

  GenericStringBuffer<UTF8<>> buffer;
  Writer<GenericStringBuffer<UTF8<>>> writer(buffer);

  root.serialize(writer);

  std::cout<<buffer.GetString()<<std::endl;
  Document readNode;
  readNode.Parse(buffer.GetString());

  EXPECT_TRUE(readNode.HasMember("isLeaf"));
  EXPECT_TRUE(readNode["isLeaf"].GetBool());

  EXPECT_TRUE(readNode.HasMember("name"));
  EXPECT_EQ(nodeName,readNode["name"].GetString());

}

TEST(NodeTest, writeChild) {
  using namespace rapidjson;
  using namespace webfs;

  Node root ("writeMe",Node::Type::BRANCH);
  Node dir1 ("dir1",Node::Type::BRANCH);
  Node dir1c1("childe1",Node::Type::LEAF);
  Node c1 ("childe2",Node::Type::LEAF);

  root.addChild(&c1);
  root.addChild(&dir1);
  dir1.addChild(&dir1c1);

  GenericStringBuffer<UTF8<>> buffer;
  Writer<GenericStringBuffer<UTF8<>>> writer(buffer);

  root.serialize(writer);

  std::cout<<buffer.GetString()<<std::endl;

  Document readNode;
  readNode.Parse(buffer.GetString());


  EXPECT_TRUE(readNode.HasMember("isLeaf"));
  EXPECT_FALSE(readNode["isLeaf"].GetBool());
  EXPECT_TRUE(readNode.HasMember("children"));
  //TODO add test
  auto &bho = readNode["children"][0];

}

#include "gists/gistsRequest.h"

TEST(NodeTest, createRequest) {
  using namespace rapidjson;
  using namespace webfs;

  Node root ("writeMe",Node::Type::BRANCH);

  gists::create(&root);

  GenericStringBuffer<UTF8<>> buffer;
    Writer<GenericStringBuffer<UTF8<>>> writer(buffer);

    root.serialize(writer);

    std::cout<<buffer.GetString()<<std::endl;

}
