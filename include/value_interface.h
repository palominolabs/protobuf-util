#ifndef KINETIC_COMMON_VALUE_INTERFACE_H_
#define KINETIC_COMMON_VALUE_INTERFACE_H_

#include <string>

#include "gmock/gmock.h"

#include "common.h"

namespace com {
namespace seagate {
namespace kinetic {

/*
 * ValueInterface represents a byte-array value arriving in a PUT request. This
 * can take the form of either a string in memory or a socket file descriptor
 * from which we can read a specified number of bytes.
 */
class ValueInterface {
    public:
    virtual ~ValueInterface() {}

    virtual size_t size() = 0;

    /*
     * TransferToFile transfers the contents of the value to a file represented
     * by the given file descriptor. As soon as this method has been called,
     * the object should be considered defunct and all further calls to
     * TransferToFile and ToString will fail.
     */
    virtual bool TransferToFile(int fd) = 0;

    /*
     * ToString copies the value to the string pointed to by the result
     * parameter.
     */
    virtual bool ToString(std::string *result) = 0;

    /*
     * Consume does whatever is necessary to consume the resources underlying
     * the value. In the SpliceableValue implementation this consists of
     * reading the appropriate bytes from the socket and throwing them away.
     * After this method has been called, the object should be considered
     * defunct and all further calls to TransferToFile and ToString will fail.
     */
    virtual void Consume() = 0;
};

class MockValue : public ValueInterface {
    public:
    MockValue() {}
    MOCK_METHOD0(size, size_t());
    MOCK_METHOD1(TransferToFile, bool(int fd));
    MOCK_METHOD1(ToString, bool(std::string *result));
    MOCK_METHOD0(Consume, void());
};

} // namespace kinetic
} // namespace seagate
} // namespace com

#endif  // KINETIC_COMMON_VALUE_INTERFACE_H_
