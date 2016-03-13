#include "gtest/gtest.h"
#include "node.h"

TEST(NodeTest, RootHasNotParent) {
  using namespace webfs;
  Node* root = new Node("", Node::Type::BRANCH);
  EXPECT_EQ(nullptr, root->getParent());
}

TEST(NodeTest, SingleNodeIsLeaf) {
  using namespace webfs;
  Node* root = new Node("", Node::Type::LEAF);
  EXPECT_EQ(Node::Type::LEAF, root->getType());
}

TEST(NodeTest, NodeWithChidIsBranch) {
  using namespace webfs;
  Node* root = new Node("parent", Node::Type::BRANCH);
  Node* child = root->createChild("child", Node::Type::LEAF);
  EXPECT_EQ(Node::Type::BRANCH, root->getType());
  EXPECT_EQ(Node::Type::LEAF, child->getType());
}

TEST(NodeTest, TestAddChild) {
  using namespace webfs;

  Node* root = new Node("", Node::Type::BRANCH);
  Node* child = root->createChild("child", Node::Type::LEAF);

  EXPECT_EQ(1u, root->getChildren().size());
  EXPECT_EQ(child, root->getChildren().front());
  EXPECT_EQ("child", child->getName());
  EXPECT_EQ(root, child->getParent());
}

TEST(NodeTest, TestFindParent) {
  using namespace webfs;

  Node* root = new Node("", Node::Type::BRANCH);

  Node* folder = root->createChild( "folder", Node::Type::LEAF);

  auto *parent = root->findParent("/example.txt");
  EXPECT_EQ(parent, root);

  auto *folderParent = root->findParent("/folder/example.txt");
  EXPECT_EQ(folderParent, folder);

  auto *closestFolderSubParent = root->findParent("/folder/a/non-existing/strange/superlong/and/drammatically/silly/folder/containing/a/file.txt");

  EXPECT_EQ(folder, closestFolderSubParent);
}


TEST(NodeTest, TestFindChild) {
  using namespace webfs;

  Node* root = new Node("", Node::Type::BRANCH);

  root->createChild("folder", Node::Type::BRANCH)
    ->createChild("subFolder", Node::Type::BRANCH)
    ->createChild("myAwesome.txt", Node::Type::LEAF);
  root->createChild("folder2", Node::Type::BRANCH);

  Node *foundFolder2 = root->findChild("/folder2");
  EXPECT_TRUE(foundFolder2!=nullptr);
  EXPECT_EQ(foundFolder2->getName(), "folder2");
  EXPECT_EQ(foundFolder2->getType(), Node::Type::BRANCH);
  EXPECT_EQ(foundFolder2->getParent(), root);

  Node *foundFolder = root->findChild("/folder");
  EXPECT_TRUE(foundFolder!=nullptr);
  EXPECT_EQ(foundFolder->getName(), "folder");
  EXPECT_EQ(foundFolder->getType(), Node::Type::BRANCH);
  EXPECT_EQ(foundFolder->getParent(), root);

  Node *foundInFolder = foundFolder->findChild("/subFolder/myAwesome.txt");
  EXPECT_TRUE(foundInFolder!=nullptr);
  EXPECT_EQ(foundInFolder->getName(), "myAwesome.txt");
  EXPECT_EQ(foundInFolder->getType(), Node::Type::LEAF);
  EXPECT_EQ(foundInFolder->getParent()->getName(), "subFolder");

  Node *found = root->findChild("/folder/subFolder/myAwesome.txt");
  EXPECT_TRUE(found!=nullptr);
  EXPECT_EQ(found->getName(), "myAwesome.txt");
  EXPECT_EQ(found->getType(), Node::Type::LEAF);


}

/* Overloaded operators */
TEST(NodeEqualTest, NodeWithDifferentNameAreDifferent) {
  using namespace webfs;

  Node n1("name1", Node::Type::BRANCH);
  Node n2("name2", Node::Type::BRANCH);

  EXPECT_FALSE(n1==n2);
  EXPECT_TRUE(n1!=n2);
}

TEST(NodeEqualTest, NodeWithDifferentTypeAreDifferent) {
  using namespace webfs;

  Node n1("name1", Node::Type::BRANCH);
  n1.createChild("Child", Node::Type::LEAF);

  Node n2("name1", Node::Type::LEAF);

  EXPECT_FALSE(n1==n2);
  EXPECT_TRUE(n1!=n2);
}

TEST(NodeEqualTest, NodeWithDifferentChildAreDifferent) {
  using namespace webfs;

  Node r1("name1", Node::Type::BRANCH);
  r1.createChild("childName1", Node::Type::LEAF);

  Node r2("name1", Node::Type::BRANCH);
  r2.createChild("childName2", Node::Type::LEAF);

  EXPECT_FALSE(r1==r2);
  EXPECT_TRUE(r1!=r2);
}

TEST(NodeEqualTest, NodeWithSameChildInDifferentObjectAreEqual) {
  using namespace webfs;

  Node r1("name1", Node::Type::BRANCH);
  r1.createChild("childName1", Node::Type::LEAF);

  Node r2("name1", Node::Type::BRANCH);
  r2.createChild("childName1", Node::Type::LEAF);

  EXPECT_TRUE(r1==r2);
  EXPECT_FALSE(r1!=r2);
}

