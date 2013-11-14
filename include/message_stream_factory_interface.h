#ifndef KINETIC_COMMON_MESSAGE_STREAM_FACTORY_INTERFACE_H_
#define KINETIC_COMMON_MESSAGE_STREAM_FACTORY_INTERFACE_H_

#include "message_stream_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class MessageStreamFactoryInterface {
    public:
    virtual bool NewMessageStream(int fd, bool use_ssl,
        MessageStreamInterface **message_stream) = 0;
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_COMMON_MESSAGE_STREAM_FACTORY_INTERFACE_H_
