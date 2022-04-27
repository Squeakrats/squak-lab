#include <gtest/gtest.h>
#include "compression.h"

TEST(CompressionTests, BasicTests) {
    // "aaaaabbbbbaaaaa"
    uint8_t data[] = { 0x78, 0x9c, 0x4b, 0x4c, 0x4, 0x82, 0x24, 0x10, 0x0, 0x31, 0x12, 0x1, 0x2d, 0xaf, 0x5, 0xb5 };
    
    Buffer deflated(sizeof(data));
    std::memcpy(deflated.data, data, sizeof(data));

    std::vector<uint8_t> inflated{};
    inflate(inflated, deflated);

    ASSERT_EQ(std::string(inflated.begin(), inflated.end()), "aaaaabbbbbaaaaa");
}