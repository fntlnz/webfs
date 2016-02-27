#include "gtest/gtest.h"
#include "filesystem.h"

TEST(FilesystemTest, TestCreateDirectory) {
  using namespace webfs;

  Node root("");

  Filesystem fs(&root);

  auto createdDirectory = fs.createDirectory("/mycreateddir");

  auto foundCreatedDirectory = root.findChild("mycreateddir");

  EXPECT_EQ(foundCreatedDirectory, createdDirectory);
  delete createdDirectory;
}

TEST(FilesystemTest, TestCreateFile) {
  using namespace webfs;
  Node root("");

  Filesystem fs(&root);

  auto createdFile = fs.createFile("/dummyfile.txt");

  fs.createFile("/dummyfile.txt");

  auto foundCreatedFile = root.findChild("dummyfile.txt");

  EXPECT_EQ(foundCreatedFile, createdFile);
}
