#ifndef PROTOBUFUTIL_MOCK_MESSAGE_STREAM_H_
#define PROTOBUFUTIL_MOCK_MESSAGE_STREAM_H_

#include "gmock/gmock.h"
#include "google/protobuf/message.h"
#include "openssl/ssl.h"

#include "byte_stream.h"
#include "common.h"
#include "incoming_value.h"

namespace palominolabs {
namespace protobufutil {

class MockMessageStream : public MessageStreamInterface {
    public:
    MockMessageStream() {}
    MOCK_METHOD2(ReadMessage, MessageStreamReadStatus(::google::protobuf::Message *message,
        IncomingValueInterface** value));
    MOCK_METHOD2(WriteMessage, bool(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value));
    MOCK_METHOD0(BytesRead, int64_t());
    MOCK_METHOD0(BytesWritten, int64_t());
};

class MockMessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MockMessageStreamFactory() {}
    virtual ~MockMessageStreamFactory() {}
    MOCK_METHOD4(NewMessageStream,
        bool(int fd, bool use_ssl, uint32_t max_message_size_bytes,
            MessageStreamInterface **message_stream));
};

}  // namespace protobufutil
}  // namespace palominolabs

#endif  // PROTOBUFUTIL_MOCK_MESSAGE_STREAM_H_
