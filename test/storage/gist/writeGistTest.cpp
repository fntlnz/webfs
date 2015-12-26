#include <algorithm>
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
  const std::string fakeRemoteId("1234567");
  webfs::storage::Gist gist;

  EXPECT_EQ(remoteId,gist.write(fileContent));

  EXPECT_EQ("POST",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));


}




