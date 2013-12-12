#ifndef PROTOBUFUTIL_BYTE_STREAM_H_
#define PROTOBUFUTIL_BYTE_STREAM_H_

#include "openssl/ssl.h"

#include "common.h"
#include "incoming_value.h"
#include "outgoing_value.h"

namespace palominolabs {
namespace protobufutil {

class ByteStreamInterface {
    public:
    virtual ~ByteStreamInterface() {}
    virtual bool Read(void *buf, size_t n) = 0;
    virtual bool Write(const void *buf, size_t n) = 0;
    virtual IncomingValueInterface *ReadValue(size_t n) = 0;
    virtual bool WriteValue(const OutgoingValueInterface &value) = 0;
};

class PlainByteStream : public ByteStreamInterface {
    public:
    PlainByteStream(int fd, IncomingValueFactoryInterface &value_factory);
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    IncomingValueInterface *ReadValue(size_t n);
    bool WriteValue(const OutgoingValueInterface &value);

    private:
    int fd_;
    IncomingValueFactoryInterface &value_factory_;
    DISALLOW_COPY_AND_ASSIGN(PlainByteStream);
};

class SslByteStream : public ByteStreamInterface {
    public:
    explicit SslByteStream(SSL *ssl);
    virtual ~SslByteStream();
    bool Read(void *buf, size_t n);
    bool Write(const void *buf, size_t n);
    IncomingValueInterface *ReadValue(size_t n);
    bool WriteValue(const OutgoingValueInterface &value);

    private:
    SSL *ssl_;
    DISALLOW_COPY_AND_ASSIGN(SslByteStream);
};

} // namespace protobufutil
} // namespace palominolabs

#endif  // PROTOBUFUTIL_BYTE_STREAM_H_
