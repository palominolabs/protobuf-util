#include "message_stream.h"

#include <arpa/inet.h>

#include "glog/logging.h"

#include "string_value.h"

namespace com {
namespace seagate {
namespace kinetic {

MessageStream::MessageStream(ByteStreamInterface *byte_stream)
    : byte_stream_(byte_stream) {}

MessageStream::~MessageStream() {
    delete byte_stream_;
}

bool MessageStream::ReadMessage(::google::protobuf::Message *message, ValueInterface** value) {
    char magic;
    if (!byte_stream_->Read(&magic, sizeof(magic))) {
        return false;
    }

    VLOG(2) << "Read magic byte " << magic;
    if (magic != 'F') {
        LOG(WARNING) << "Received invalid magic value " << magic;
        return false;
    }

    // Next read the 4 byte message size
    int32_t message_size;
    if (!byte_stream_->Read(&message_size, sizeof(message_size))) {
        LOG(WARNING) << "Unable to read message length";
        return false;
    }
    message_size = ntohl(message_size);
    VLOG(2) << "Read message size " << message_size;

    // Finally the 4 byte value size
    uint32_t value_size;
    if (!byte_stream_->Read(&value_size, sizeof(value_size))) {
        LOG(WARNING) << "Unable to read value length";
        return false;
    }
    value_size = ntohl(value_size);
    VLOG(2) << "Read value size " << value_size;

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
        const std::string& value) {
    // First we need to write the magic 'F'
    if (!byte_stream_->Write("F", 1)) {
        LOG(WARNING) << "Failed to write magic value";
        return false;
    }

    // The the message and value lengths
    uint32_t message_size = htonl(message.ByteSize());
    if (!byte_stream_->Write(&message_size, sizeof(message_size))) {
        LOG(WARNING) << "Failed to write message size";
        return false;
    }
    uint32_t value_size = htonl(value.size());
    if (!byte_stream_->Write(&value_size, sizeof(value_size))) {
        LOG(WARNING) << "Failed to write value size";
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
    if (value.size()) {
        if (!byte_stream_->Write(value.data(), value.size())) {
            LOG(WARNING) << "Failed to write value";
            return false;
        }
    }

    return true;
}

MessageStreamFactory::MessageStreamFactory(SSL_CTX *ssl_context,
        ValueFactoryInterface &value_factory)
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

}  // namespace kinetic
}  // namespace seagate
}  // namespace com
