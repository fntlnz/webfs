#include "gtest/gtest.h"
#include "filesystem.h"

TEST(FilesystemTest, TestCreateDirectory) {
  using namespace webfs;

  auto root = std::make_shared<Node>("/", Node::Type::BRANCH);

  Filesystem fs(root);

  auto createdDirectory = fs.createElementDirectory("/mycreateddir");

  auto foundCreatedDirectory = Node::findChild(root,"mycreateddir");

  EXPECT_EQ(*foundCreatedDirectory.lock(), *createdDirectory.lock());
}

TEST(FilesystemTest, TestCreateFile) {
  using namespace webfs;
  auto root = std::make_shared<Node>("/", Node::Type::BRANCH);

  Filesystem fs(root);

  auto createdFile = fs.createElementFile("/dummyfile.txt");

  auto foundCreatedFile = Node::findChild(root,"dummyfile.txt");

  EXPECT_EQ(*foundCreatedFile.lock(), *createdFile.lock());
}
