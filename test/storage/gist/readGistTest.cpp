#include <algorithm>
#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

#include "GistTestUtil.h"

class TestReadGist : public ::testing::Test{

public:
  std::string fileContent;

  virtual void SetUp() {
    fileContent ="TEST";
    curlMock_init();
    curlMock_setResponseCode(200);
    curlMock_enqueuResponse("{\"user\": null, \"files\": { \"testFile.txt\": {"
        "\"type\": \"text/plain\","
          " \"content\": \""+fileContent+"\"  } }}");
  }

  virtual void TearDown() {  }

};


TEST_F(TestReadGist, extractResponse) {
  const std::string fakeRemoteId("1234567");
  pStorageId remoteId = std::make_unique<TestStorageId>(fakeRemoteId);
  webfs::storage::Gist gist;
  std::vector<char> data=gist.read(remoteId);
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_URL).find(fakeRemoteId)
      !=std::string::npos);
  EXPECT_EQ(fileContent.size(),data.size());
  for (uint32_t i=0; i<data.size();i++){
    EXPECT_EQ(fileContent[i],data[i]);
  }
}
