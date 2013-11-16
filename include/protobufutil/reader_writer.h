#ifndef PROTOBUFUTIL_READER_WRITER_H_
#define PROTOBUFUTIL_READER_WRITER_H_

#include <stddef.h>

#include "common.h"

namespace palominolabs {
namespace protobufutil {

class ReaderWriter {
    public:
    explicit ReaderWriter(int fd);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);

    private:
    int fd_;
    DISALLOW_COPY_AND_ASSIGN(ReaderWriter);
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_READER_WRITER_H_
