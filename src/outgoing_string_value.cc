#include "protobufutil/outgoing_value.h"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>

#include "glog/logging.h"

#include "protobufutil/reader_writer.h"

namespace palominolabs {
namespace protobufutil {

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
