#include "gtest/gtest.h" 
#include <google/protobuf/stubs/common.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    const int r = RUN_ALL_TESTS();
    ::google::protobuf::ShutdownProtobufLibrary();
    return r;
}
    
