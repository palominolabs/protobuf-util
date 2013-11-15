#include "gtest/gtest.h"
#include "glog/logging.h"
#include "google/protobuf/stubs/common.h"

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  testing::InitGoogleTest(&argc, argv);
  int status = RUN_ALL_TESTS();
  google::ShutdownGoogleLogging();
  google::protobuf::ShutdownProtobufLibrary();
  return status;
}
