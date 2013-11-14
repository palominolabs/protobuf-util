#ifndef KINETIC_COMMON_BYTE_STREAM_H_
#define KINETIC_COMMON_BYTE_STREAM_H_

#include "openssl/ssl.h"

#include "common.h"
#include "value_factory.h"
#include "value_interface.h"

namespace com {
namespace seagate {
namespace kinetic {

class ByteStreamInterface {
    public:
    virtual ~ByteStreamInterface() {}
    virtual bool Read(void *buf, size_t n) = 0;
    virtual bool Write(const void *buf, size_t n) = 0;
    virtual ValueInterface *ReadValue(size_t n) = 0;
};

class PlainByteStream : public ByteStreamInterface {
    public:
    PlainByteStream(int fd, ValueFactoryInterface &value_factory);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    ValueInterface *ReadValue(size_t n);

    private:
    int fd_;
    ValueFactoryInterface &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(PlainByteStream);
};

class SslByteStream : public ByteStreamInterface {
    public:
    explicit SslByteStream(SSL *ssl);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    ValueInterface *ReadValue(size_t n);

    private:
    SSL *ssl_;
    DISALLOW_COPY_AND_ASSIGN(SslByteStream);
};

} // namespace kinetic
} // namespace seagate
} // namespace com

#endif  // KINETIC_COMMON_BYTE_STREAM_H_
