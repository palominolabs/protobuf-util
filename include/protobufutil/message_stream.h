#ifndef PROTOBUFUTIL_MESSAGE_STREAM_H_
#define PROTOBUFUTIL_MESSAGE_STREAM_H_

#include "google/protobuf/message.h"
#include "openssl/ssl.h"

#include "byte_stream.h"
#include "common.h"
#include "incoming_value.h"

namespace palominolabs {
namespace protobufutil {

class MessageStreamInterface {
    public:
    typedef enum {
        MessageStreamReadStatus_SUCCESS,
        MessageStreamReadStatus_INTERNAL_ERROR,
        MessageStreamReadStatus_TOO_LARGE
    } MessageStreamReadStatus;

    virtual ~MessageStreamInterface() {}
    virtual MessageStreamReadStatus ReadMessage(::google::protobuf::Message *message,
        IncomingValueInterface** value) = 0;
    virtual bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value) = 0;
};

class MessageStream : public MessageStreamInterface {
    public:
    explicit MessageStream(uint32_t max_message_size_bytes, ByteStreamInterface *byte_stream);
    ~MessageStream();
    MessageStreamReadStatus ReadMessage(::google::protobuf::Message *message,
        IncomingValueInterface** value);
    bool WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value);

    private:
    bool ReadHeader(uint32_t *message_size, uint32_t *value_size);
    bool WriteHeader(uint32_t message_size, uint32_t value_size);
    uint32_t max_message_size_bytes_;
    ByteStreamInterface *byte_stream_;
    DISALLOW_COPY_AND_ASSIGN(MessageStream);
};

class MessageStreamFactoryInterface {
    public:
    virtual bool NewMessageStream(int fd, bool use_ssl, uint32_t max_message_size_bytes,
        MessageStreamInterface **message_stream) = 0;
    virtual ~MessageStreamFactoryInterface() {}
};

class MessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MessageStreamFactory(SSL_CTX *ssl_context, IncomingValueFactoryInterface &value_factory);
    bool NewMessageStream(int fd, bool use_ssl, uint32_t max_message_size_bytes,
        MessageStreamInterface **message_stream);
    virtual ~MessageStreamFactory() {}

    private:
    SSL_CTX *ssl_context_;
    IncomingValueFactoryInterface &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(MessageStreamFactory);
};

}  // namespace protobufutil
}  // namespace palominolabs

#endif  // PROTOBUFUTIL_MESSAGE_STREAM_H_
