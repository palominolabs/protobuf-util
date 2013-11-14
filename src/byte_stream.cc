#include "byte_stream.h"

#include <errno.h>
#include <unistd.h>

#include "glog/logging.h"

#include "reader_writer.h"
#include "string_value.h"

namespace com {
namespace seagate {
namespace kinetic {

PlainByteStream::PlainByteStream(int fd, ValueFactoryInterface &value_factory)
    : fd_(fd), value_factory_(value_factory) {}

bool PlainByteStream::Read(void *buf, size_t n) {
    ReaderWriter reader_writer(fd_);
    return reader_writer.Read(buf, n);
}

bool PlainByteStream::Write(const void *buf, size_t n) {
    ReaderWriter reader_writer(fd_);
    return reader_writer.Write(buf, n);
}

ValueInterface *PlainByteStream::ReadValue(size_t n) {
    return value_factory_.NewValue(fd_, n);
}

SslByteStream::SslByteStream(SSL *ssl) : ssl_(ssl) {}

bool SslByteStream::Read(void *buf, size_t n) {
    if (n == 0) {
        // SSL_read with 0 bytes causes openssl to get really upset in mysterious ways
        return true;
    }
    if (SSL_read(ssl_, buf, n) != static_cast<int>(n)) {
        LOG(WARNING) << "Failed to read " << n << " bytes over SSL connection";
        return false;
    }
    return true;
}

bool SslByteStream::Write(const void *buf, size_t n) {
    if (SSL_write(ssl_, buf, n) != static_cast<int>(n)) {
        LOG(WARNING) << "Failed to write " << n << " bytes over SSL connection";
        return false;
    }
    return true;
}

ValueInterface *SslByteStream::ReadValue(size_t n) {
    // We can't use splice since we're using SSL here--instead we fall back on
    // just copying the value into a StringValue object.
    char *buf = new char[n];
    if (!Read(buf, n)) {
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
