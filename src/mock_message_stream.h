#ifndef KINETIC_MOCK_MESSAGE_STREAM_H_
#define KINETIC_MOCK_MESSAGE_STREAM_H_

#include "gmock/gmock.h"

#include "kinetic.pb.h"
#include "message_stream_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class MockMessageStream : public MessageStreamInterface {
    public:
    MockMessageStream() {}
    MOCK_METHOD2(ReadMessage, bool(::google::protobuf::Message *message, ValueInterface** value));
    MOCK_METHOD2(WriteMessage, bool(const ::google::protobuf::Message &message,
        const std::string& value));
    MOCK_METHOD0(BytesRead, int64_t());
    MOCK_METHOD0(BytesWritten, int64_t());
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_MOCK_MESSAGE_STREAM_H_
