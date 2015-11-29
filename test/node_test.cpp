#include "gtest/gtest.h"
#include "node.h"

class NodeTest : public ::testing::Test {
};

TEST_F(NodeTest, TestFindNode) {
  auto *root = new webfs::Node();
  root->name = "";
  root->type = webfs::NodeType::BRANCH;

  auto *folder = new webfs::Node();
  folder->name = "folder";
  folder->type = webfs::NodeType::BRANCH;

  auto *folder2 = new webfs::Node();
  folder2->name = "folder2";
  folder2->type = webfs::NodeType::BRANCH;

  auto *subfolder = new webfs::Node();
  subfolder->name = "subfolder";
  subfolder->type = webfs::NodeType::BRANCH;

  auto *file = new webfs::Node();
  file->name = "myawesome.txt";
  file->type = webfs::NodeType::LEAF;

  subfolder->children.push_back(file);
  folder->children.push_back(subfolder);
  root->children.push_back(folder);
  root->children.push_back(folder2);

  auto *found = webfs::findNodeByPath(root, "/folder/subfolder/myawesome.txt");
  EXPECT_EQ(found, file);
}
