#include <gtest/gtest.h>

#include "net/buffer.h"

using tinyserver::net::Buffer;

TEST(BufferTest, testBufferAppendRetrieve) {
  Buffer buf;
  EXPECT_EQ(buf.size(), 0);

  const std::string str(200, 'x');
  buf.append(str);
  EXPECT_EQ(buf.size(), 200);

  const std::string str2 = buf.retrieveAsString(50);
  EXPECT_EQ(str2.size(), 50);
  EXPECT_EQ(str2, std::string(50, 'x'));
  EXPECT_EQ(buf.size(), str.size() - str2.size());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
