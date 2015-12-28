#include "gtest/gtest.h"
#include "../../curlMock/curlMock.h"
#include "storage/GistUtil.h"

class TestGistGetToken : public ::testing::Test{

public:
  std::string fakeToken;

  virtual void SetUp() {
    fakeToken ="GoldenTocken";
    curlMock_init();
    curlMock_setResponseCode(201);
    curlMock_enqueuResponse("{\"id\": \"12345\",\"token\": \""+fakeToken+"\"}");
  }

  virtual void TearDown() {  }

};

TEST_F(TestGistGetToken, requereUserAgent) {
	webfs::storage::GistUtil::getAuthToken("","","");
	EXPECT_TRUE(curlMock_headerHasString("User-Agent: "));
}

TEST_F(TestGistGetToken, setUserAgent) {
	const std::string userAgent("SecretAgent007");
	webfs::storage::GistUtil::getAuthToken("","","",userAgent);
	EXPECT_TRUE(curlMock_headerHasString("User-Agent: "+userAgent));
}

TEST_F(TestGistGetToken, useBasicAuth) {
	webfs::storage::GistUtil::getAuthToken("","","");
	EXPECT_EQ(curlMock_getOptionValue(CURLOPT_HTTPAUTH),
			std::to_string(CURLAUTH_BASIC));
}

TEST_F(TestGistGetToken, getToken) {
  const std::string user("PINCOPALLO");
  const std::string pass("qwerty");
  const std::string tokenNote("I need it!! NOW!");
  std::string token = webfs::storage::GistUtil::getAuthToken(user,pass,tokenNote);

  EXPECT_EQ(fakeToken,token);
  EXPECT_EQ(user,curlMock_getOptionValue(CURLOPT_USERNAME));
  EXPECT_EQ(pass,curlMock_getOptionValue(CURLOPT_PASSWORD));
  EXPECT_TRUE(curlMock_getOptionValue(CURLOPT_POSTFIELDS).find(tokenNote)
		  !=std::string::npos);

}


