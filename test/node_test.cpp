#include "gtest/gtest.h"
#include "node.h"

TEST(NodeTest, RootHasNotParent) {
  using namespace webfs;
  Node root("");
  EXPECT_EQ(nullptr, root.getParent());
}

TEST(NodeTest, SingleNodeIsLeaf) {
  using namespace webfs;
  Node root("");
  EXPECT_EQ(Node::Type::LEAF, root.getType());
}

TEST(NodeTest, NodeWithChidIsBranch) {
  using namespace webfs;
  Node root("parent", Node::Type::BRANCH);

  auto child = root.createChild("child", Node::Type::LEAF);

  EXPECT_EQ(Node::Type::BRANCH, root.getType());
  EXPECT_EQ(Node::Type::LEAF, child.lock()->getType());
}

TEST(NodeTest, TestAddChild) {
  using namespace webfs;

  Node root("");
  auto child = root.createChild("child", Node::Type::LEAF);

  EXPECT_EQ(1u, root.getChildren().size());
  EXPECT_EQ(child.lock(), root.getChildren().front().lock());
  EXPECT_EQ("child", child.lock()->getName());
  EXPECT_EQ(&root, child.lock()->getParent());

}

TEST(NodeTest, TestFindParent) {
  using namespace webfs;

  auto root = std::make_shared<Node>("", Node::Type::BRANCH);

  auto folder = root->createChild("folder", Node::Type::LEAF);

  auto parent = Node::findParent(root,"/example.txt");
  EXPECT_EQ(parent.lock(), root);

  auto folderParent =Node::findParent(root,"/folder/example.txt");
  EXPECT_EQ(folderParent.lock(), folder.lock());

  auto closestFolderSubParent = Node::findParent(root,"/folder/a/non-existing/strange/superlong/and/drammatically/silly/folder/containing/a/file.txt");

  EXPECT_EQ(folder.lock(), closestFolderSubParent.lock());
}


TEST(NodeTest, TestFindChild) {
  using namespace webfs;

  auto root = std::make_shared<Node>("", Node::Type::BRANCH);

  root->createChild("folder", Node::Type::BRANCH).lock()->
    createChild("subFolder", Node::Type::BRANCH).lock()->
    createChild("myAwesome.txt", Node::Type::LEAF);
  root->createChild("folder2", Node::Type::BRANCH);

  auto foundFolder2 = Node::findChild(root,"/folder2").lock();
  EXPECT_TRUE(foundFolder2!=nullptr);
  EXPECT_EQ(foundFolder2->getName(), "folder2");
  EXPECT_EQ(foundFolder2->getType(), Node::Type::BRANCH);
  EXPECT_EQ(foundFolder2->getParent(),root.get());

  auto foundFolder = Node::findChild(root,"/folder").lock();
  EXPECT_TRUE(foundFolder!=nullptr);
  EXPECT_EQ(foundFolder->getName(), "folder");
  EXPECT_EQ(foundFolder->getType(), Node::Type::BRANCH);
  EXPECT_EQ(foundFolder->getParent(),root.get());

  auto foundInFolder = Node::findChild(foundFolder,"/subFolder/myAwesome.txt").lock();
  EXPECT_TRUE(foundInFolder!=nullptr);
  EXPECT_EQ(foundInFolder->getName(), "myAwesome.txt");
  EXPECT_EQ(foundInFolder->getType(), Node::Type::LEAF);
  EXPECT_EQ(foundInFolder->getParent()->getName(), "subFolder");

  auto found = Node::findChild(root,"/folder/subFolder/myAwesome.txt").lock();
  EXPECT_TRUE(found!=nullptr);
  EXPECT_EQ(found->getName(), "myAwesome.txt");
  EXPECT_EQ(found->getType(), Node::Type::LEAF);


}

TEST(NodeEqualTest, NodeWithDifferentNameAreDifferent) {
  using namespace webfs;

  Node n1 ("name1");
  Node n2 ("name2");

  EXPECT_FALSE(n1==n2);
  EXPECT_TRUE(n1!=n2);
}

TEST(NodeEqualTest, NodeWithDifferentTypeAreDifferent) {
  using namespace webfs;

  Node n1 ("name1");
  n1.createChild("Child", Node::Type::LEAF);
  Node n2 ("name1");

  EXPECT_FALSE(n1==n2);
  EXPECT_TRUE(n1!=n2);
}

TEST(NodeEqualTest, NodeWithDifferentChildAreDifferent) {
  using namespace webfs;

  Node r1("name1");
  r1.createChild("childName1", Node::Type::LEAF);

  Node r2 ("name1");
  r2.createChild("childName2", Node::Type::LEAF);


  EXPECT_FALSE(r1==r2);
  EXPECT_TRUE(r1!=r2);
}

TEST(NodeEqualTest, NodeWithSameChildInDifferentObjectAreEqual) {
  using namespace webfs;

  Node r1 ("name1");
  r1.createChild("childName1", Node::Type::LEAF);


  Node r2 ("name1");
  r2.createChild("childName1", Node::Type::LEAF);

  EXPECT_TRUE(r1==r2);
  EXPECT_FALSE(r1!=r2);
}

