#ifndef PROTOBUFUTIL_MESSAGE_STREAM_H_
#define PROTOBUFUTIL_MESSAGE_STREAM_H_

#include "gmock/gmock.h"
#include "google/protobuf/message.h"
#include "openssl/ssl.h"

#include "byte_stream.h"
#include "common.h"
#include "value_factory_interface.h"
#include "value_interface.h"

namespace palominolabs {
namespace protobufutil {

class MessageStreamInterface {
    public:
    virtual ~MessageStreamInterface() {}
    virtual bool ReadMessage(::google::protobuf::Message *message, ValueInterface** value) = 0;
    virtual bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value) = 0;
};

class MessageStream : public MessageStreamInterface {
    public:
    explicit MessageStream(ByteStreamInterface *byte_stream);
    ~MessageStream();
    bool ReadMessage(::google::protobuf::Message *message, ValueInterface** value);
    bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value);

    private:
    ByteStreamInterface *byte_stream_;
    DISALLOW_COPY_AND_ASSIGN(MessageStream);
};

class MockMessageStream : public MessageStreamInterface {
    public:
    MockMessageStream() {}
    MOCK_METHOD2(ReadMessage, bool(::google::protobuf::Message *message, ValueInterface** value));
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
    MessageStreamFactory(SSL_CTX *ssl_context, ValueFactoryInterface &value_factory);
    bool NewMessageStream(int fd, bool use_ssl, MessageStreamInterface **message_stream);

    private:
    SSL_CTX *ssl_context_;
    ValueFactoryInterface &value_factory_;
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
