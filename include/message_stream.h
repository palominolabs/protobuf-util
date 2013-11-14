#ifndef KINETIC_COMMON_MESSAGE_STREAM_H_
#define KINETIC_COMMON_MESSAGE_STREAM_H_

#include "byte_stream.h"
#include "common.h"
#include "message_stream_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class MessageStream : public MessageStreamInterface {
    public:
    explicit MessageStream(ByteStreamInterface *byte_stream);
    ~MessageStream();
    bool ReadMessage(::google::protobuf::Message *message, ValueInterface** value);
    bool WriteMessage(const ::google::protobuf::Message &message, const std::string& value);

    private:
    ByteStreamInterface *byte_stream_;
    DISALLOW_COPY_AND_ASSIGN(MessageStream);
};

}  // namespace kinetic
}  // namespace seagate
}  // namespace com

#endif  // KINETIC_COMMON_MESSAGE_STREAM_H_
