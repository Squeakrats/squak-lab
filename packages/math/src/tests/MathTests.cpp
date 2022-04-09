#include <gtest/gtest.h>
#include "../Vector3.h"

TEST(math, Vector3) {
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);

    EXPECT_EQ(a + b, Vector3(5, 7, 9));
}
