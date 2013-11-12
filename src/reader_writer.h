#ifndef KINETIC_READER_WRITER_H_
#define KINETIC_READER_WRITER_H_

#include <stddef.h>

#include "common.h"

namespace com {
namespace seagate {
namespace kinetic {

class ReaderWriter {
    public:
    explicit ReaderWriter(int fd);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);

    private:
    int fd_;
    DISALLOW_COPY_AND_ASSIGN(ReaderWriter);
};

} // namespace kinetic
} // namespace seagate
} // namespace com

#endif  // KINETIC_READER_WRITER_H_
