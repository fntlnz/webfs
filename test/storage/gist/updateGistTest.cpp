#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

class TestUpdateGist : public ::testing::Test{

public:
  std::vector<char> fileContent;
  std::string remoteId;
  virtual void SetUp() {

    remoteId="12345";
    curlMock_init();
    curlMock_setResponseCode(200);
    curlMock_enqueuResponse("{\"id\":\""+remoteId+"\" }");
  }

  virtual void TearDown() {  }

};


TEST_F(TestUpdateGist, askForUpdate) {
  const std::string fakeRemoteId("1234567");
  const std::vector<char> fakeData ={'f','a','k','e'};
  webfs::storage::Gist gist;

  EXPECT_EQ(remoteId,gist.update(fakeRemoteId,fakeData));

  EXPECT_EQ("PATCH",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_URL).find(fakeRemoteId)
        !=std::string::npos);

}
