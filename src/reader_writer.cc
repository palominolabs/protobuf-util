#include "protobufutil/reader_writer.h"

#include <errno.h>
#include <unistd.h>

#include "glog/logging.h"

namespace palominolabs {
namespace protobufutil {

ReaderWriter::ReaderWriter(int fd) : fd_(fd) {}

bool ReaderWriter::Read(void *buf, size_t n) {
    size_t bytes_read = 0;
    while (bytes_read < n) {
        int status = read(fd_, reinterpret_cast<char *>(buf) + bytes_read, n - bytes_read);
        if (status == -1 && errno == EINTR) {
            continue;
        }
        if (status < 0) {
            PLOG(WARNING) << "Failed to read from socket";
            return false;
        }
        if (status == 0) {
            LOG(WARNING) << "Failed to read from socket";
            return false;
        }
        bytes_read += status;
    }

    return true;
}

bool ReaderWriter::Write(const void *buf, size_t n) {
    size_t bytes_written = 0;
    while (bytes_written < n) {
        int status = write(fd_, reinterpret_cast<const char *>(buf) + bytes_written,
            n - bytes_written);
        if (status == -1 && errno == EINTR) {
            continue;
        }
        if (status < 0) {
            PLOG(WARNING) << "Failed to write to socket";
            return false;
        }
        if (status == 0) {
            LOG(WARNING) << "Failed to write to socket";
            return false;
        }
        bytes_written += status;
    }

    return true;
}

} // namespace protobufutil
} // namespace palominolabs
