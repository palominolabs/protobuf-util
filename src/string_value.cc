#include "protobufutil/string_value.h"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>

#include "glog/logging.h"

#include "protobufutil/outgoing_value.h"
#include "protobufutil/reader_writer.h"

namespace palominolabs {
namespace protobufutil {

StringValue::StringValue(const std::string &s)
    : s_(s), defunct_(false) {}

size_t StringValue::size() {
    return s_.size();
}

bool StringValue::TransferToFile(int fd) {
    if (defunct_) {
        return false;
    }
    defunct_ = true;

    ReaderWriter reader_writer(fd);
    return reader_writer.Write(s_.data(), s_.size());
}

bool StringValue::ToString(std::string *result) {
    if (defunct_) {
        return false;
    }
    defunct_ = true;

    *result = s_;
    return true;
}

void StringValue::Consume() {
    defunct_ = true;
}

OutgoingStringValue::OutgoingStringValue(const std::string &s)
    : s_(s) {}

size_t OutgoingStringValue::size() const {
    return s_.size();
}

bool OutgoingStringValue::TransferToSocket(int fd) const {
    ReaderWriter reader_writer(fd);
    return reader_writer.Write(s_.data(), s_.size());
}

bool OutgoingStringValue::ToString(std::string *result) const {
    *result = s_;
    return true;
}

} // namespace protobufutil
} // namespace palominolabs
