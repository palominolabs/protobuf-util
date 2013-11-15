#ifndef PROTOBUFUTIL_VALUE_FACTORY_H_
#define PROTOBUFUTIL_VALUE_FACTORY_H_

#include "common.h"
#include "value_interface.h"

namespace palominolabs {
namespace protobufutil {

class ValueFactoryInterface {
    public:
    virtual ~ValueFactoryInterface() {}
    virtual ValueInterface *NewValue(int fd, size_t n) = 0;
};

class ValueFactory : public ValueFactoryInterface {
    public:
    ValueFactory();
    ValueInterface *NewValue(int fd, size_t n);

    private:
    DISALLOW_COPY_AND_ASSIGN(ValueFactory);
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_VALUE_FACTORY_H_
