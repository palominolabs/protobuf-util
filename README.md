[![Built on CloudBees](https://palominolabs.ci.cloudbees.com/job/protobufutil/badge/icon)](https://palominolabs.ci.cloudbees.com/job/protobufutil/)

Introduction
============
Protobufutil is a library of helper routines for reading and writing protocol
buffers over a socket, including over SSL/TLS.

Building
========
To build the library, run `cmake . && make`. You can then run `make test` to
run the unit test suite. If you are running Linux and have Valgrind available
on your system, you can also run `make test_valgrind` to run the test suite
under Valgrind.

To incorporate protobufutil into another project, make sure the `include/`
directory is on your compiler's header search path. You can link against the
static library produced by the build process, `libprotobufutil.a`.
