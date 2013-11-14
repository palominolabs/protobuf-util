#ifndef KINETIC_COMMON_VALUE_FACTORY_H_
#define KINETIC_COMMON_VALUE_FACTORY_H_

#include "common.h"
#include "value_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class ValueFactory {
    public:
    ValueFactory();
    ValueInterface *NewValue(int fd, size_t n);

    private:
    DISALLOW_COPY_AND_ASSIGN(ValueFactory);
};

} // namespace kinetic
} // namespace seagate
} // namespace com

#endif  // KINETIC_COMMON_VALUE_FACTORY_H_
