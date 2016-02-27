#include "gtest/gtest.h"
#include "node.h"

TEST(NodeTest, TestAddChild) {
  using namespace webfs;

  Node root("");

  Node child("file.txt");

  root.addChild(&child);

  EXPECT_EQ(1u, root.getChildren().size());
  EXPECT_EQ(&child, root.getChildren().front());

}

TEST(NodeTest, TestFindParent) {
  using namespace webfs;

  Node root("");
  Node folder( "folder");

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

  Node root ("");

  Node folder("folder");

  Node folder2("folder2");

  Node subfolder("subfolder");

  Node file("myawesome.txt");

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
  n1.addChild(new Node("Child"));
  Node n2 ("name1");

  EXPECT_FALSE(n1==n2);
  EXPECT_TRUE(n1!=n2);
}

TEST(NodeEqualTest, NodeWithDifferentChildAreDifferent) {
  using namespace webfs;

  Node r1 ("name1");
  Node c1 ("childName1");
  r1.addChild(&c1);

  Node r2 ("name1");
  Node c2 ("childName2");
  r2.addChild(&c2);

  EXPECT_FALSE(r1==r2);
  EXPECT_TRUE(r1!=r2);
}

TEST(NodeEqualTest, NodeWithSameChildInDifferentObjectAreEqual) {
  using namespace webfs;

  Node r1 ("name1");
  Node c1 ("childName1");
  r1.addChild(&c1);

  Node r2 ("name1");
  Node c2 ("childName1");
  r2.addChild(&c2);

  EXPECT_TRUE(r1==r2);
  EXPECT_FALSE(r1!=r2);
}

