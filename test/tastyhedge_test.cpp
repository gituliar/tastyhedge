#include <gtest/gtest.h>

#include "Core/Logger.h"

using namespace tasty;


int main(int argc, char **argv)
{
    Logger::level() = INFO;

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
