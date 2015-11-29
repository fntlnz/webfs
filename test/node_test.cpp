#include "gtest/gtest.h"
#include "node.h"

class NodeTest : public ::testing::Test {
};

TEST_F(NodeTest, TestCanCreateNode) {
  webfs::Node *node = new webfs::Node();
}
