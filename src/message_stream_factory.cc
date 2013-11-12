#include "message_stream_factory.h"

#include "glog/logging.h"

#include "byte_stream.h"
#include "message_stream.h"

namespace com {
namespace seagate {
namespace kinetic {

MessageStreamFactory::MessageStreamFactory(SSL_CTX *ssl_context, ValueFactory &value_factory)
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
