#include "protobufutil/incoming_value.h"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>

#include "glog/logging.h"

#include "protobufutil/reader_writer.h"

namespace palominolabs {
namespace protobufutil {

IncomingStringValue::IncomingStringValue(const std::string &s)
    : s_(s), defunct_(false) {}

size_t IncomingStringValue::size() {
    return s_.size();
}

bool IncomingStringValue::TransferToFile(int fd) {
    if (defunct_) {
        return false;
    }
    defunct_ = true;

    ReaderWriter reader_writer(fd);
    return reader_writer.Write(s_.data(), s_.size());
}

bool IncomingStringValue::ToString(std::string *result) {
    if (defunct_) {
        return false;
    }
    defunct_ = true;

    *result = s_;
    return true;
}

void IncomingStringValue::Consume() {
    defunct_ = true;
}

} // namespace protobufutil
} // namespace palominolabs
