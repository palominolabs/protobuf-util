#ifndef PROTOBUFUTIL_VALUE_FACTORY_INTERFACE_H_
#define PROTOBUFUTIL_VALUE_FACTORY_INTERFACE_H_

#include "common.h"
#include "value_interface.h"

namespace palominolabs {
namespace protobufutil {

class ValueFactoryInterface {
    public:
    virtual ~ValueFactoryInterface() {}
    virtual ValueInterface *NewValue(int fd, size_t n) = 0;
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_VALUE_FACTORY_INTERFACE_H_
