#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

class TestGist : public ::testing::Test{

public:

  virtual void SetUp() {
    curlMock_init();
    curlMock_setResponseCode(201);
  }

  virtual void TearDown() {  }

};


TEST_F(TestGist, checkHeaderCleanUp) {
  {
    webfs::storage::Gist gist;
  }
  EXPECT_TRUE(curlMock_slist_free_all_isCall());
}

TEST_F(TestGist, checkCURLCleanUp) {
  curlMock_enqueuResponse("{\"id\":\"123\"}");
  {
    webfs::storage::Gist gist;
    gist.write({'d'});
  }
  EXPECT_TRUE(curlMock_cleanup_isCall());
}

TEST_F(TestGist, checkHeaderContnet) {

  webfs::storage::Gist gist;
  EXPECT_TRUE(curlMock_headerHasString("cache-control: no-cache"));
  EXPECT_TRUE(curlMock_headerHasString("User-Agent: webFS"));
  EXPECT_TRUE(curlMock_headerHasString("Content-Type: application/json"));
}

TEST_F(TestGist, checkHeaderContnetWithAuth) {
  const std::string token("12345667");
  webfs::storage::Gist gist(token);
  EXPECT_TRUE(curlMock_headerHasString("cache-control: no-cache"));
  EXPECT_TRUE(curlMock_headerHasString("User-Agent: webFS"));
  EXPECT_TRUE(curlMock_headerHasString("Content-Type: application/json"));
  EXPECT_TRUE(curlMock_headerHasString(token));
}
