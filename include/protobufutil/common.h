#ifndef PROTOBUFUTIL_COMMON_H_
#define PROTOBUFUTIL_COMMON_H_

// Most classes do not need copy or assignment constructors and should use this
// macro to disable them.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

#endif  // PROTOBUFUTIL_COMMON_H_
