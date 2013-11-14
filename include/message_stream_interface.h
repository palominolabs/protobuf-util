#ifndef KINETIC_COMMON_MESSAGE_STREAM_INTERFACE_H_
#define KINETIC_COMMON_MESSAGE_STREAM_INTERFACE_H_

#include "google/protobuf/message.h"

#include "common.h"
#include "value_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class MessageStreamInterface {
    public:
    virtual ~MessageStreamInterface() {}
    virtual bool ReadMessage(::google::protobuf::Message *message, ValueInterface** value) = 0;
    virtual bool WriteMessage(const ::google::protobuf::Message &message,
        const std::string& value) = 0;
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_COMMON_MESSAGE_STREAM_INTERFACE_H_
