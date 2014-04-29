#ifndef PROTOBUFUTIL_MOCK_INCOMING_VALUE_H_
#define PROTOBUFUTIL_MOCK_INCOMING_VALUE_H_

#include <string>

#include "gmock/gmock.h"

#include "common.h"

namespace palominolabs {
namespace protobufutil {

class MockIncomingValue : public IncomingValueInterface {
    public:
    MockIncomingValue() {}
    MOCK_METHOD0(size, size_t());
    MOCK_METHOD1(TransferToFile, bool(int fd));
    MOCK_METHOD1(ToString, bool(std::string *result));
    MOCK_METHOD0(Consume, void());
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_MOCK_INCOMING_VALUE_H_
