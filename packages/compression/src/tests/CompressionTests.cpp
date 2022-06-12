#include <gtest/gtest.h>
#include "squak/inflate.h"

TEST(CompressionTests, BasicTests) {
    // "aaaaabbbbbaaaaa"
    uint8_t data[] = { 0x78, 0x9c, 0x4b, 0x4c, 0x4, 0x82, 0x24, 0x10, 0x0, 0x31, 0x12, 0x1, 0x2d, 0xaf, 0x5, 0xb5 };
    
    Buffer deflated(sizeof(data));
    std::memcpy(deflated.data, data, sizeof(data));

    Buffer inflated(30);
    size_t uncompressedSize = inflate(inflated, deflated);
    ASSERT_EQ(uncompressedSize, 15);

    inflated.data[uncompressedSize] = 0;
    ASSERT_EQ(std::string(reinterpret_cast<char*>(inflated.data)), "aaaaabbbbbaaaaa");
}