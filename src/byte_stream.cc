#include "protobufutil/byte_stream.h"

#include <errno.h>
#include <unistd.h>

#include "glog/logging.h"

#include "protobufutil/incoming_value.h"
#include "protobufutil/outgoing_value.h"
#include "protobufutil/reader_writer.h"

namespace palominolabs {
namespace protobufutil {

PlainByteStream::PlainByteStream(int fd, IncomingValueFactoryInterface &value_factory)
    : fd_(fd), value_factory_(value_factory) {}

bool PlainByteStream::Read(void *buf, size_t n) {
    ReaderWriter reader_writer(fd_);
    return reader_writer.Read(buf, n);
}

bool PlainByteStream::Write(const void *buf, size_t n) {
    ReaderWriter reader_writer(fd_);
    return reader_writer.Write(buf, n);
}

IncomingValueInterface *PlainByteStream::ReadValue(size_t n) {
    return value_factory_.NewValue(fd_, n);
}

bool PlainByteStream::WriteValue(const OutgoingValueInterface &value) {
    return value.TransferToSocket(fd_);
}

SslByteStream::SslByteStream(SSL *ssl) : ssl_(ssl) {}

SslByteStream::~SslByteStream() {
    SSL_free(ssl_);
}

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
    if (n == 0) {
        // It's not clear whether SSL_write can handle a write of 0 bytes
        return true;
    }
    if (SSL_write(ssl_, buf, n) != static_cast<int>(n)) {
        LOG(WARNING) << "Failed to write " << n << " bytes over SSL connection";
        return false;
    }
    return true;
}

IncomingValueInterface *SslByteStream::ReadValue(size_t n) {
    // We can't use splice since we're using SSL here--instead we fall back on
    // just copying the value into a IncomingStringValue object.
    char *buf = new char[n];
    if (!Read(buf, n)) {
        delete[] buf;
        return NULL;
    }
    std::string value(buf, n);
    delete[] buf;
    return new IncomingStringValue(value);
}

bool SslByteStream::WriteValue(const OutgoingValueInterface &value) {
    std::string s;
    if (!value.ToString(&s)) {
        return false;
    }
    return Write(s.data(), s.size());
}

} // namespace protobufutil
} // namespace palominolabs
