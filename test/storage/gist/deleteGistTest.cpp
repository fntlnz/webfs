#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

#include "GistTestUtil.h"


class TestDeleteGist : public ::testing::Test{

public:
  std::vector<char> fileContent;
  std::string remoteId;
  virtual void SetUp() {

    curlMock_init();
    curlMock_setResponseCode(204);
    curlMock_enqueuResponse("");
  }

  virtual void TearDown() {  }

};


TEST_F(TestDeleteGist, failIfNoAuthToken) {
  webfs::storage::Gist gist;
  pStorageId remoteId = std::make_unique<TestStorageId>("1234567");
  EXPECT_FALSE(gist.remove(remoteId));
}

TEST_F(TestDeleteGist, failIfNoReturn204) {
  const std::string fakeRemoteId("1234567");
  pStorageId remoteId = std::make_unique<TestStorageId>("1234567");
  webfs::storage::Gist gist("fakeToken");
  curlMock_setResponseCode(200);
  EXPECT_FALSE(gist.remove(remoteId));

}

TEST_F(TestDeleteGist, removeGist) {
  const std::string fakeRemoteId("1234567");
  webfs::storage::Gist gist("fakeToken");
  pStorageId remoteId = std::make_unique<TestStorageId>("1234567");
  EXPECT_TRUE(gist.remove(remoteId));

  EXPECT_EQ("DELETE",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_URL).find(fakeRemoteId)
        !=std::string::npos);

}

