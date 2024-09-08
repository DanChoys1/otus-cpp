#include "lib.h"

#include <gtest/gtest.h>

TEST(gtest_version, test)
{
    ASSERT_TRUE(version() > 0);
}