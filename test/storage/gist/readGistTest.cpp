#include <algorithm>
#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/gist.h"

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

  virtual void TearDown() {
	  //EXPECT_TRUE(curlMock_slist_free_all_is_call());
	  //EXPECT_TRUE(curlMock_cleanup_isCall());
  }


};

TEST_F(TestReadGist, checkHeaderContnet) {

	webfs::storage::Gist gist;
	EXPECT_TRUE(curlMock_headerHasString("cache-control: no-cache"));
	EXPECT_TRUE(curlMock_headerHasString("User-Agent: webFS"));
	EXPECT_TRUE(curlMock_headerHasString("Content-Type: application/json"));
}

TEST_F(TestReadGist, extractResponse) {
	const std::string fakeRemoteId("1234567");
	webfs::storage::Gist gist;
	std::vector<char> data=gist.read(fakeRemoteId);
	EXPECT_EQ(fileContent.size(),data.size());
	for (uint32_t i=0; i<data.size();i++){
		EXPECT_EQ(fileContent[i],data[i]);
	}
}
