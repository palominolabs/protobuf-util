#include "gtest/gtest.h"
#include "glog/logging.h"

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  testing::InitGoogleTest(&argc, argv);
  int status = RUN_ALL_TESTS();
  google::ShutdownGoogleLogging();
  return status;
}
