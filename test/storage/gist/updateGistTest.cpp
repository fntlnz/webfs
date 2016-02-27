#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

#include "GistTestUtil.h"

class TestUpdateGist : public ::testing::Test{

public:
  std::vector<char> fileContent;
  std::string newRemoteId;
  virtual void SetUp() {

	  newRemoteId="12345";
    curlMock_init();
    curlMock_setResponseCode(200);
    curlMock_enqueuResponse("{\"id\":\""+newRemoteId+"\" }");
  }

  virtual void TearDown() {  }

};


TEST_F(TestUpdateGist, askForUpdate) {
  const std::string fakeRemoteId("1234567");
  pStorageId remoteId = std::make_unique<TestStorageId>(fakeRemoteId);

  const std::vector<char> fakeData ={'f','a','k','e'};
  webfs::storage::Gist gist;
  auto idIt= gist.update(remoteId,fakeData)->getRemoteReadUrl().rfind(newRemoteId);
  EXPECT_TRUE(idIt!=std::string::npos);
  //EXPECT_EQ(remoteId->getRemoteReadUrl(),gist.update(remoteId,fakeData)->getRemoteReadUrl());

  EXPECT_EQ("PATCH",curlMock_getOptionValue(CURLOPT_CUSTOMREQUEST));
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_URL).find(fakeRemoteId)
        !=std::string::npos);

}
