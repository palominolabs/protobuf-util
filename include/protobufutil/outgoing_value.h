#ifndef PROTOBUFUTIL_OUTGOING_VALUE_H_
#define PROTOBUFUTIL_OUTGOING_VALUE_H_

#include <string>

#include "gmock/gmock.h"

#include "common.h"

namespace palominolabs {
namespace protobufutil {

class OutgoingValueInterface {
    public:
    virtual ~OutgoingValueInterface() {}
    virtual size_t size() const = 0;
    virtual bool TransferToSocket(int fd) const = 0;
    virtual bool ToString(std::string *result) const = 0;
};

class OutgoingStringValue : public OutgoingValueInterface {
    public:
    explicit OutgoingStringValue(const std::string &s);
    size_t size() const;
    bool TransferToSocket(int fd) const;
    bool ToString(std::string *result) const;

    private:
    const std::string s_;
    DISALLOW_COPY_AND_ASSIGN(OutgoingStringValue);
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_OUTGOING_VALUE_H_
