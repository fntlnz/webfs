#include "gtest/gtest.h"
#include "filesystem.h"

TEST(FilesystemTest, TestCreateDirectory) {
  using namespace webfs;

  Node root("");

  Filesystem fs(&root);

  Node &createdDirectory = fs.createElement("/mycreateddir");

  Node *foundCreatedDirectory = root.findChild("mycreateddir");

  EXPECT_EQ(*foundCreatedDirectory, createdDirectory);
}

TEST(FilesystemTest, TestCreateFile) {
  using namespace webfs;
  Node root("");

  Filesystem fs(&root);

  Node& createdFile = fs.createElement("/dummyfile.txt");

  fs.createElement("/dummyfile.txt");

  Node* foundCreatedFile = root.findChild("dummyfile.txt");

  EXPECT_EQ(*foundCreatedFile, createdFile);
}
