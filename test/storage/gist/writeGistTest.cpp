#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

class TestWriteGist : public ::testing::Test{

public:
  std::vector<char> fileContent;
  std::string remoteId;
  virtual void SetUp() {
    fileContent ={'f','a','k','e'};
    remoteId="12345";
    curlMock_init();
    curlMock_setResponseCode(201);
    curlMock_enqueuResponse("{\"id\":\""+remoteId+"\" }");
  }

  virtual void TearDown() {  }

};


TEST_F(TestWriteGist, extractResponse) {
  webfs::storage::Gist gist;

  auto idIt= gist.write(fileContent)->getRemoteReadUrl().rfind(remoteId);
  EXPECT_TRUE(idIt!=std::string::npos);


  EXPECT_EQ("POST",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));


}

