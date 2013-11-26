#include "gtest/gtest.h"

#include <unistd.h>

#include <string>

#include "protobufutil/outgoing_value.h"
#include "protobufutil/string_value.h"

namespace palominolabs {
namespace protobufutil {

TEST(StringValueTest, TransferToFileWorks) {
    // Write StringValue into a pipe
    StringValue value("a");
    int output_pipe[2];
    ASSERT_EQ(0, pipe(output_pipe));
    ASSERT_TRUE(value.TransferToFile(output_pipe[1]));

    // Check that the pipe contains the right value
    char result;
    ASSERT_EQ(1, read(output_pipe[0], &result, 1));
    ASSERT_EQ('a', result);

    ASSERT_EQ(0, close(output_pipe[0]));
    ASSERT_EQ(0, close(output_pipe[1]));
}

TEST(StringValueTest, ToStringWorks) {
    StringValue value("a");
    std::string result;
    ASSERT_TRUE(value.ToString(&result));
    ASSERT_EQ("a", result);
}

TEST(StringValueTest, ToStringDepletesObject) {
    StringValue value("a");
    std::string result;
    ASSERT_TRUE(value.ToString(&result));
    ASSERT_FALSE(value.ToString(&result));
}

TEST(StringValueTest, TransferToFileDepletesObject) {
    // Write StringValue into a pipe
    StringValue value("a");
    int output_pipe[2];
    ASSERT_EQ(0, pipe(output_pipe));
    ASSERT_TRUE(value.TransferToFile(output_pipe[1]));

    // Verify that further operations fail
    std::string result;
    ASSERT_FALSE(value.ToString(&result));

    ASSERT_EQ(0, close(output_pipe[0]));
    ASSERT_EQ(0, close(output_pipe[1]));
}

TEST(StringValueTest, ConsumeDepletesObject) {
    StringValue value("a");
    value.Consume();
    std::string result;
    ASSERT_FALSE(value.ToString(&result));
}

TEST(OutgoingStringValueTest, SizeWorks) {
    OutgoingStringValue value("abc");
    ASSERT_EQ(3u, value.size());
}

TEST(OutgoingStringValueTest, TransferToSocketWorks) {
    // Write OutgoingStringValue into a pipe
    OutgoingStringValue value("a");
    int output_pipe[2];
    ASSERT_EQ(0, pipe(output_pipe));
    ASSERT_TRUE(value.TransferToSocket(output_pipe[1]));

    // Check that the pipe contains the right value
    char result;
    ASSERT_EQ(1, read(output_pipe[0], &result, 1));
    ASSERT_EQ('a', result);

    ASSERT_EQ(0, close(output_pipe[0]));
    ASSERT_EQ(0, close(output_pipe[1]));
}

TEST(OutgoingStringValueTest, ToStringWorks) {
    OutgoingStringValue value("abc");
    std::string s;
    ASSERT_TRUE(value.ToString(&s));
    ASSERT_EQ("abc", s);
}

} // namespace protobufutil
} // namespace palominolabs
