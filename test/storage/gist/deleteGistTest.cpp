#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

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

  EXPECT_FALSE(gist.remove("1234567"));
}

TEST_F(TestDeleteGist, failIfNoReturn204) {
  const std::string fakeRemoteId("1234567");
  webfs::storage::Gist gist("fakeToken");
  curlMock_setResponseCode(200);
  EXPECT_FALSE(gist.remove(fakeRemoteId));

}

TEST_F(TestDeleteGist, removeGist) {
  const std::string fakeRemoteId("1234567");
  webfs::storage::Gist gist("fakeToken");

  EXPECT_TRUE(gist.remove(fakeRemoteId));

  EXPECT_EQ("DELETE",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_URL).find(fakeRemoteId)
        !=std::string::npos);

}

