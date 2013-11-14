cmake_minimum_required(VERSION 2.8.6)
project(kineticcommon CXX C)

option(BUILD_FOR_ARM "Build for ARM instead of x86" off)

if(APPLE)
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
    set(PTHREAD_FLAG "") # The -pthread flag isn't needed on OSX
    set(OPENSSL_CONFIGURE_COMMAND ./Configure darwin64-x86_64-cc)
else(APPLE)
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    set(PTHREAD_FLAG "-pthread")
    set(OPENSSL_CONFIGURE_COMMAND ./config)
endif(APPLE)

set(CMAKE_CXX_FLAGS "-g --std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter ${PTHREAD_FLAG}")

include(ExternalProject)
set(EXTERNAL_PROJECTS gflags glog gmock gtest openssl protobuf)

ExternalProject_add(
    gflags
    PREFIX "vendor/gflags"
    URL "https://gflags.googlecode.com/files/gflags-2.0-no-svn-files.tar.gz"
    URL_MD5 "9084829124e02a7e6be0f0f824523423"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --enable-static
    INSTALL_COMMAND ""
)

ExternalProject_add(
    glog
    PREFIX "vendor/glog"
    URL "https://google-glog.googlecode.com/files/glog-0.3.3.tar.gz"
    URL_MD5 "a6fd2c22f8996846e34c763422717c18"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --enable-static
    INSTALL_COMMAND ""
)

ExternalProject_add(
    gmock
    PREFIX "vendor/gmock"
    URL "https://googlemock.googlecode.com/files/gmock-1.6.0.zip"
    URL_MD5 "f547f47321ca88d3965ca2efdcc2a3c1"
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)

ExternalProject_add(
    gtest
    PREFIX "vendor/gtest"
    URL "https://googletest.googlecode.com/files/gtest-1.6.0.zip"
    URL_MD5 "4577b49f2973c90bf9ba69aa8166b786"
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)

ExternalProject_add(
    openssl
    PREFIX "vendor/openssl"
    URL "http://www.openssl.org/source/openssl-1.0.1e.tar.gz"
    URL_MD5 "66bf6f10f060d561929de96f9dfe5b8c"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_COMMAND}
    INSTALL_COMMAND ""
)

ExternalProject_add(
    protobuf
    PREFIX "vendor/protobuf"
    URL "https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2"
    URL_MD5 "a72001a9067a4c2c4e0e836d0f92ece4"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --enable-static
    INSTALL_COMMAND ""
)

include_directories(
    include
    vendor/gflags/src/gflags/src
    vendor/glog/src/glog/src
    vendor/gmock/src/gmock
    vendor/gmock/src/gmock/include
    vendor/gtest/src/gtest
    vendor/gtest/src/gtest/include
    vendor/openssl/src/openssl
    vendor/openssl/src/openssl/include
    vendor/protobuf/src/protobuf/src
)

add_library(kineticcommon
    src/byte_stream.cc
    src/message_stream.cc
    src/reader_writer.cc
    src/string_value.cc
    src/value_factory.cc
)

add_dependencies(kineticcommon ${EXTERNAL_PROJECTS})
set(LIBRARY_DEPENDENCIES
    ${kineticcommon_SOURCE_DIR}/vendor/gflags/src/gflags/.libs/libgflags.a
    ${kineticcommon_SOURCE_DIR}/vendor/glog/src/glog/.libs/libglog.a
    ${kineticcommon_SOURCE_DIR}/vendor/gmock/src/gmock/libgmock.a
    ${kineticcommon_SOURCE_DIR}/vendor/gtest/src/gtest/libgtest.a
    ${kineticcommon_SOURCE_DIR}/vendor/openssl/src/openssl/libssl.a
    ${kineticcommon_SOURCE_DIR}/vendor/openssl/src/openssl/libcrypto.a
    ${kineticcommon_SOURCE_DIR}/vendor/protobuf/src/protobuf/src/.libs/libprotobuf.a
    dl
)
set(TEST_BINARY kinetic_common_test)

add_executable(${TEST_BINARY}
    src/message_stream_test.cc
    src/kinetic_common_test.cc
    src/string_value_test.cc
)
target_link_libraries(kinetic_common_test kineticcommon ${LIBRARY_DEPENDENCIES})

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
        COMMAND valgrind --leak-check=full --show-reachable=yes --track-fds=yes ./${TEST_BINARY}
        DEPENDS ${TEST_BINARY}
    )
endif(APPLE)

add_custom_target(lint
    ./bin/lint
    WORKING_DIRECTORY ${kineticcommon_SOURCE_DIR}
    COMMENT "Running style checker" VERBATIM
)
