#ifndef PROTOBUFUTIL_BYTE_STREAM_H_
#define PROTOBUFUTIL_BYTE_STREAM_H_

#include "openssl/ssl.h"

#include "common.h"
#include "outgoing_value.h"
#include "value_factory_interface.h"
#include "value_interface.h"

namespace palominolabs {
namespace protobufutil {

class ByteStreamInterface {
    public:
    virtual ~ByteStreamInterface() {}
    virtual bool Read(void *buf, size_t n) = 0;
    virtual bool Write(const void *buf, size_t n) = 0;
    virtual ValueInterface *ReadValue(size_t n) = 0;
    virtual bool WriteValue(const OutgoingValueInterface &value) = 0;
};

class PlainByteStream : public ByteStreamInterface {
    public:
    PlainByteStream(int fd, ValueFactoryInterface &value_factory);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    ValueInterface *ReadValue(size_t n);
    bool WriteValue(const OutgoingValueInterface &value);

    private:
    int fd_;
    ValueFactoryInterface &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(PlainByteStream);
};

class SslByteStream : public ByteStreamInterface {
    public:
    explicit SslByteStream(SSL *ssl);
    virtual ~SslByteStream();
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    ValueInterface *ReadValue(size_t n);
    bool WriteValue(const OutgoingValueInterface &value);

    private:
    SSL *ssl_;
    DISALLOW_COPY_AND_ASSIGN(SslByteStream);
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_BYTE_STREAM_H_
