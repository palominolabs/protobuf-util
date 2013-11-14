#ifndef KINETIC_COMMON_MESSAGE_STREAM_FACTORY_H_
#define KINETIC_COMMON_MESSAGE_STREAM_FACTORY_H_

#include "openssl/ssl.h"

#include "common.h"
#include "message_stream_factory_interface.h"
#include "message_stream_interface.h"
#include "value_factory.h"

namespace com {
namespace seagate {
namespace kinetic {

class MessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MessageStreamFactory(SSL_CTX *ssl_context, ValueFactory &value_factory);
    bool NewMessageStream(int fd, bool use_ssl, MessageStreamInterface **message_stream);

    private:
    SSL_CTX *ssl_context_;
    ValueFactory &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(MessageStreamFactory);
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_COMMON_MESSAGE_STREAM_FACTORY_H_
