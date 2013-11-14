#ifndef KINETIC_COMMON_STRING_VALUE_H_
#define KINETIC_COMMON_STRING_VALUE_H_

#include "value_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

/*
 * StringValue represents a value stored internally as a plain string. It's
 * preferable to use SpliceableValue whenever possible because of its
 * performance benefits.
 */
class StringValue : public ValueInterface {
    public:
    explicit StringValue(const std::string &s);
    size_t size();
    bool TransferToFile(int fd);
    bool ToString(std::string *result);
    void Consume();

    private:
    const std::string s_;
    bool defunct_;
    DISALLOW_COPY_AND_ASSIGN(StringValue);
};

} // namespace kinetic
} // namespace seagate
} // namespace com

#endif  // KINETIC_COMMON_STRING_VALUE_H_
