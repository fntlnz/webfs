#include "gtest/gtest.h"
#include "node.h"

TEST(NodeTest, TestAddChild) {
  auto *root = new webfs::Node();
  root->name = "";
  root->type = webfs::NodeType::BRANCH;

  auto *child = new webfs::Node();
  child->name = "file.txt";
  child->type = webfs::NodeType::LEAF;
  root->addChild(child);

  EXPECT_EQ(child, root->children.front());
}

TEST(NodeTest, TestFindParent) {
  auto *root = new webfs::Node();
  root->name = "";
  root->type = webfs::NodeType::BRANCH;

  auto *folder = new webfs::Node();
  folder->name = "folder";
  folder->type = webfs::NodeType::BRANCH;

  root->addChild(folder);

  auto *parent = root->findParent("/example.txt");
  EXPECT_EQ(parent, root);

  auto *folderParent = root->findParent("/folder/example.txt");
  EXPECT_EQ(folderParent, folder);

  auto *closestFolderSubParent = root->findParent("/folder/a/non-existing/strange/superlong/and/drammatically/silly/folder/containing/a/file.txt");

  EXPECT_EQ(folder, closestFolderSubParent);
}

TEST(NodeTest, TestFindChild) {
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

  subfolder->addChild(file);
  folder->addChild(subfolder);
  root->addChild(folder);
  root->addChild(folder2);

  auto *found = root->findChild("/folder/subfolder/myawesome.txt");
  EXPECT_EQ(found, file);

  auto *foundInFolder = folder->findChild("/subfolder/myawesome.txt");
  EXPECT_EQ(foundInFolder, file);

  auto *foundInSubfolder = subfolder->findChild("/myawesome.txt");
  EXPECT_EQ(foundInSubfolder, file);

  auto *foundFolder = root->findChild("/folder2");
  EXPECT_EQ(foundFolder, folder2);
}
