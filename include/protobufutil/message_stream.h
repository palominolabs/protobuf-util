#ifndef PROTOBUFUTIL_MESSAGE_STREAM_H_
#define PROTOBUFUTIL_MESSAGE_STREAM_H_

#include "gmock/gmock.h"
#include "google/protobuf/message.h"
#include "openssl/ssl.h"

#include "byte_stream.h"
#include "common.h"
#include "incoming_value.h"

namespace palominolabs {
namespace protobufutil {

class MessageStreamInterface {
    public:
    virtual ~MessageStreamInterface() {}
    virtual bool ReadMessage(::google::protobuf::Message *message,
        IncomingValueInterface** value) = 0;
    virtual bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value) = 0;
};

class MessageStream : public MessageStreamInterface {
    public:
    explicit MessageStream(ByteStreamInterface *byte_stream);
    ~MessageStream();
    bool ReadMessage(::google::protobuf::Message *message, IncomingValueInterface** value);
    bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value);

    private:
    bool ReadHeader(uint32_t *message_size, uint32_t *value_size);
    bool WriteHeader(uint32_t message_size, uint32_t value_size);
    ByteStreamInterface *byte_stream_;
    DISALLOW_COPY_AND_ASSIGN(MessageStream);
};

class MockMessageStream : public MessageStreamInterface {
    public:
    MockMessageStream() {}
    MOCK_METHOD2(ReadMessage, bool(::google::protobuf::Message *message,
        IncomingValueInterface** value));
    MOCK_METHOD2(WriteMessage, bool(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value));
    MOCK_METHOD0(BytesRead, int64_t());
    MOCK_METHOD0(BytesWritten, int64_t());
};

class MessageStreamFactoryInterface {
    public:
    virtual bool NewMessageStream(int fd, bool use_ssl,
        MessageStreamInterface **message_stream) = 0;
};

class MessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MessageStreamFactory(SSL_CTX *ssl_context, IncomingValueFactoryInterface &value_factory);
    bool NewMessageStream(int fd, bool use_ssl, MessageStreamInterface **message_stream);

    private:
    SSL_CTX *ssl_context_;
    IncomingValueFactoryInterface &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(MessageStreamFactory);
};

class MockMessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MockMessageStreamFactory() {}
    MOCK_METHOD3(NewMessageStream,
        bool(int fd, bool use_ssl, MessageStreamInterface **message_stream));
};

}  // namespace protobufutil
}  // namespace palominolabs

#endif  // PROTOBUFUTIL_MESSAGE_STREAM_H_
