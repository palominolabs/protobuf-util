cmake_minimum_required(VERSION 2.8.6)
project(protobufutil CXX C)

find_package (Threads)

option(BUILD_FOR_ARM "Build for ARM instead of x86" off)

if(APPLE)
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
    set(OPENSSL_CONFIGURE_COMMAND ../openssl/Configure darwin64-x86_64-cc)
else(APPLE)
    if(${BUILD_FOR_ARM})
        set(CMAKE_C_COMPILER "arm-marvell-linux-gnueabi-gcc")
        set(CMAKE_CXX_COMPILER "arm-marvell-linux-gnueabi-g++")
        set(CMAKE_RANLIB "arm-marvell-linux-gnueabi-ranlib")
        set(OPENSSL_CONFIGURE_COMMAND ../openssl/Configure linux-armv4)
        set(CONFIG_HOST_FLAG --host=arm)
        set(CHILD_MAKE_FLAGS CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} RANLIB=${CMAKE_RANLIB})
    else(${BUILD_FOR_ARM})
        set(CMAKE_C_COMPILER "gcc")
        set(CMAKE_CXX_COMPILER "g++")
        set(OPENSSL_CONFIGURE_COMMAND ../openssl/config -DPURIFY)
    endif(${BUILD_FOR_ARM})
endif(APPLE)

set(ADDITIONAL_CXX_FLAGS "--std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-local-typedefs -Wno-unknown-warning-option -Wno-null-dereference -DGTEST_USE_OWN_TR1_TUPLE=1")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ADDITIONAL_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ADDITIONAL_CXX_FLAGS}")

include(ExternalProject)
set(EXTERNAL_PROJECTS gflags glog gmock gtest openssl protobuf)

set(EXTERNAL_PREFIX "${protobufutil_BINARY_DIR}/vendor")

ExternalProject_add(
    gflags
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/gflags-2.0-no-svn-files.tar.gz"
    URL_MD5 "9084829124e02a7e6be0f0f824523423"
    CONFIGURE_COMMAND ../gflags/configure --prefix=${EXTERNAL_PREFIX} --enable-static ${CONFIG_HOST_FLAG} ${CHILD_MAKE_FLAGS}
)

ExternalProject_add(
    glog
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/glog-0.3.3.tar.gz"
    URL_MD5 "a6fd2c22f8996846e34c763422717c18"
    PATCH_COMMAND sh ${protobufutil_SOURCE_DIR}/patches/apply-glog-patches.sh ${protobufutil_SOURCE_DIR}
    CONFIGURE_COMMAND ../glog/configure --prefix=${EXTERNAL_PREFIX} --with-gflags=${EXTERNAL_PREFIX} --enable-static ${CONFIG_HOST_FLAG} ${CHILD_MAKE_FLAGS}
    DEPENDS gflags
)

ExternalProject_add(
    gtest
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/gtest-1.6.0.zip"
    URL_MD5 "4577b49f2973c90bf9ba69aa8166b786"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_CXX_COMPILER} -DGTEST_USE_OWN_TR1_TUPLE=1 -I../gtest -I../gtest/include -c ../gtest/src/gtest-all.cc && ar -rv libgtest.a gtest-all.o && ranlib libgtest.a
    INSTALL_COMMAND ""
)

ExternalProject_add(
    gmock
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/gmock-1.6.0.zip"
    URL_MD5 "f547f47321ca88d3965ca2efdcc2a3c1"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_CXX_COMPILER} -DGTEST_USE_OWN_TR1_TUPLE=1 -I../gmock -I../gmock/include -I../gtest -I../gtest/include -c ../gmock/src/gmock-all.cc && ar -rv libgmock.a gmock-all.o && ranlib libgmock.a
    INSTALL_COMMAND ""
    DEPENDS gtest
)

ExternalProject_add(
    openssl
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/openssl-1.0.1e.tar.gz"
    URL_MD5 "66bf6f10f060d561929de96f9dfe5b8c"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_COMMAND} --prefix=${EXTERNAL_PREFIX}
    BUILD_COMMAND touch apps/openssl && touch openssl.pc && make ${CHILD_MAKE_FLAGS} build_libs libssl.pc libcrypto.pc
    INSTALL_COMMAND make install_sw
)

# The protobuf build requires the existence of a protoc binary that can be
# executed on the host machine. To handle cross compilation, we always build
# protobuf once for the host so that we have a suitable copy of protoc.
ExternalProject_add(
    protoc
    PREFIX ${EXTERNAL_PREFIX}/host
    URL "${protobufutil_SOURCE_DIR}/tarballs/protobuf-2.5.0.tar.bz2"
    URL_MD5 "a72001a9067a4c2c4e0e836d0f92ece4"
    CONFIGURE_COMMAND ../protoc/configure --prefix=${EXTERNAL_PREFIX}/host --enable-static
)

ExternalProject_add(
    protobuf
    PREFIX ${EXTERNAL_PREFIX}
    URL "${protobufutil_SOURCE_DIR}/tarballs/protobuf-2.5.0.tar.bz2"
    URL_MD5 "a72001a9067a4c2c4e0e836d0f92ece4"
    CONFIGURE_COMMAND ../protobuf/configure --prefix=${EXTERNAL_PREFIX} --enable-static --with-protoc=../../host/bin/protoc ${CONFIG_HOST_FLAG} ${CHILD_MAKE_FLAGS}
    DEPENDS protoc
)

include_directories(
    include
    ${EXTERNAL_PREFIX}/include
    ${EXTERNAL_PREFIX}/src/gmock/include
    ${EXTERNAL_PREFIX}/src/gtest/include
)

add_library(protobufutil
    src/byte_stream.cc
    src/incoming_string_value.cc
    src/message_stream.cc
    src/outgoing_string_value.cc
    src/reader_writer.cc
)

add_dependencies(protobufutil ${EXTERNAL_PROJECTS})
set(LIBRARY_DEPENDENCIES
    ${EXTERNAL_PREFIX}/src/gmock/libgmock.a
    ${EXTERNAL_PREFIX}/src/gtest/libgtest.a
    ${EXTERNAL_PREFIX}/lib/libglog.a
    ${EXTERNAL_PREFIX}/lib/libgflags.a
    ${EXTERNAL_PREFIX}/lib/libssl.a
    ${EXTERNAL_PREFIX}/lib/libcrypto.a
    ${EXTERNAL_PREFIX}/lib/libprotobuf.a
    dl
)
set(TEST_BINARY protobufutil_test)

add_executable(${TEST_BINARY}
    src/message_stream_test.cc
    src/protobufutil_test.cc
    src/string_value_test.cc
)
target_link_libraries(protobufutil_test protobufutil ${LIBRARY_DEPENDENCIES} ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(test
    COMMAND ${TEST_BINARY} --gtest_output=xml:gtestresults.xml
    DEPENDS ${TEST_BINARY}
)
if(APPLE)
    add_custom_target(test_valgrind
        COMMAND echo "Valgrind not supported on OSX")
    add_custom_target(test_valgrind_report
        COMMAND echo "Valgrind not supported on OSX")
else(APPLE)
    add_custom_target(test_valgrind
        COMMAND valgrind --leak-check=full --show-reachable=yes --track-fds=yes --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./${TEST_BINARY}
        DEPENDS ${TEST_BINARY}
    )
endif(APPLE)

add_custom_target(lint
    ./bin/lint
    WORKING_DIRECTORY ${protobufutil_SOURCE_DIR}
    COMMENT "Running style checker" VERBATIM
)
