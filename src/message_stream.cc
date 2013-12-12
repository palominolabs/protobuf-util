#include "protobufutil/message_stream.h"

#include <arpa/inet.h>
#include <string.h>

#include "glog/logging.h"

#include "protobufutil/incoming_value.h"
#include "protobufutil/outgoing_value.h"

namespace palominolabs {
namespace protobufutil {

MessageStream::MessageStream(ByteStreamInterface *byte_stream)
    : byte_stream_(byte_stream) {}

MessageStream::~MessageStream() {
    delete byte_stream_;
}

bool MessageStream::ReadMessage(::google::protobuf::Message *message,
        IncomingValueInterface** value) {
    // First the header
    uint32_t message_size, value_size;
    if (!ReadHeader(&message_size, &value_size)) {
        return false;
    }

    // Now the message
    char *message_bytes = new char[message_size];
    if (!byte_stream_->Read(message_bytes, message_size)) {
        LOG(WARNING) << "Unable to read message";
        delete[] message_bytes;
        return false;
    }

    if (!message->ParseFromArray(message_bytes, message_size)) {
        LOG(WARNING) << "Failed to parse protobuf message";
        delete[] message_bytes;
        return false;
    }

    delete[] message_bytes;

    // Now read the value (if any)
    *value = byte_stream_->ReadValue(value_size);
    if (*value == NULL) {
        return false;
    }

    return true;
}

bool MessageStream::WriteMessage(const ::google::protobuf::Message &message,
        const OutgoingValueInterface& value) {
    // First the header
    if (!WriteHeader(message.ByteSize(), value.size())) {
        LOG(WARNING) << "Failed to write header";
        return false;
    }

    // Now the message
    std::string message_string;
    if (!message.SerializeToString(&message_string)) {
        LOG(WARNING) << "Failed to serialize protocol buffer";
        return false;
    }
    if (!byte_stream_->Write(message_string.data(), message_string.size())) {
        LOG(WARNING) << "Failed to write message";
        return false;
    }

    // And finally the value if any
    if (!byte_stream_->WriteValue(value)) {
        LOG(WARNING) << "Failed to write value";
        return false;
    }

    return true;
}

MessageStreamFactory::MessageStreamFactory(SSL_CTX *ssl_context,
        IncomingValueFactoryInterface &value_factory)
    : ssl_context_(ssl_context), value_factory_(value_factory) {}

bool MessageStreamFactory::NewMessageStream(int fd, bool use_ssl,
        MessageStreamInterface **message_stream) {
    if (use_ssl) {
        SSL *ssl = SSL_new(ssl_context_);
        // We want to automatically retry reads and writes when a renegotiation
        // takes place. This way the only errors we have to handle are real,
        // permanent ones.
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
        if (ssl == NULL) {
            LOG(ERROR) << "Failed to create new SSL object";
            return false;
        }
        if (SSL_set_fd(ssl, fd) != 1) {
            LOG(ERROR) << "Failed to associate SSL object with file descriptor";
            SSL_free(ssl);
            return false;
        }
        if (SSL_accept(ssl) != 1) {
            LOG(ERROR) << "Failed to perform SSL handshake";
            LOG(ERROR) << "The client may have attempted to use an SSL/TLS version below TLSv1.1";
            SSL_free(ssl);
            return false;
        }
        LOG(INFO) << "Successfully performed SSL handshake";
        *message_stream = new MessageStream(new SslByteStream(ssl));
    } else {
        *message_stream = new MessageStream(new PlainByteStream(fd, value_factory_));
    }

    return true;
}

bool MessageStream::ReadHeader(uint32_t *message_size, uint32_t *value_size) {
    char header[9];
    if (!byte_stream_->Read(header, sizeof(header))) {
        return false;
    }

    if (header[0] != 'F') {
        LOG(WARNING) << "Received invalid magic value " << header[0];
        return false;
    }

    memcpy(reinterpret_cast<char *>(message_size), header + 1, sizeof(*message_size));
    memcpy(reinterpret_cast<char *>(value_size), header + 5, sizeof(*value_size));
    *message_size = ntohl(*message_size);
    *value_size = ntohl(*value_size);

    return true;
}

bool MessageStream::WriteHeader(uint32_t message_size, uint32_t value_size) {
    char header[9];
    header[0] = 'F';
    message_size = htonl(message_size);
    value_size = htonl(value_size);
    memcpy(header + 1, reinterpret_cast<char *>(&message_size), sizeof(message_size));
    memcpy(header + 5, reinterpret_cast<char *>(&value_size), sizeof(value_size));
    return byte_stream_->Write(header, sizeof(header));
}

}  // namespace protobufutil
}  // namespace palominolabs
