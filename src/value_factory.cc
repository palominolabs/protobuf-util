#include <errno.h>
#include <unistd.h>

#include "glog/logging.h"

#include "reader_writer.h"
#include "string_value.h"
#include "value_factory.h"

namespace com {
namespace seagate {
namespace kinetic {

ValueFactory::ValueFactory() {}

ValueInterface *ValueFactory::NewValue(int fd, size_t n) {
    ReaderWriter reader_writer(fd);
    char *buf = new char[n];
    if (!reader_writer.Read(buf, n)) {
        delete[] buf;
        return NULL;
    }
    std::string value(buf, n);
    delete[] buf;
    return new StringValue(value);
}

} // namespace kinetic
} // namespace seagate
} // namespace com
