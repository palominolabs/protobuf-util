#ifndef KINETIC_COMMON_H_
#define KINETIC_COMMON_H_

// Most classes do not need copy or assignment constructors and should use this
// macro to disable them.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

#endif  // KINETIC_COMMON_H_
