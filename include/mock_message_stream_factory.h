#ifndef KINETIC_COMMON_MOCK_MESSAGE_STREAM_FACTORY_H_
#define KINETIC_COMMON_MOCK_MESSAGE_STREAM_FACTORY_H_

#include "message_stream_factory_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class MockMessageStreamFactory : public MessageStreamFactoryInterface {
    public:
    MockMessageStreamFactory() {}
    MOCK_METHOD3(NewMessageStream,
        bool(int fd, bool use_ssl, MessageStreamInterface **message_stream));
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_COMMON_MOCK_MESSAGE_STREAM_FACTORY_H_
