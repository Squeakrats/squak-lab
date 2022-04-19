#include "compression.h"
#include "BitStream.h"
#include <memory>
#include <array>
#include "utility.h"

uint8_t ZLIB_COMPRESSION_MODE_DEFLATE = 8;

struct HuffmaNode {
    size_t code{};
    HuffmaNode* left{};
    HuffmaNode* right{};

    ~HuffmaNode() {
        if (this->left) {
            delete left;
            this->left = nullptr;
        }

        if (this->right) {
            delete right;
            this->right = nullptr;
        }
    }
};

std::unique_ptr<HuffmaNode> CreateHuffmanTree(std::vector<size_t>& lengths, size_t maxLength) {
    std::vector<size_t> counts{};
    counts.resize(maxLength + 1);
    for (size_t i = 0; i < lengths.size(); i++) {
        counts[lengths[i]]++;
    }

    std::vector<size_t> bases{};
    bases.resize(maxLength + 1);
    for (size_t i = 1; i < counts.size(); i++) {
        bases[i] = (bases[i - 1] + counts[i - 1]) << 1;
    }

    std::vector<size_t> huffmanCodes{};
    huffmanCodes.resize(lengths.size());
    for (size_t i = 0; i < huffmanCodes.size(); i++) {
        if (lengths[i] > 0) {
            huffmanCodes[i] = bases[lengths[i]]++;
        }
    }

    std::unique_ptr<HuffmaNode> tree = std::make_unique<HuffmaNode>();
    for (size_t symbol = 0; symbol < huffmanCodes.size(); symbol++) {
        size_t huffmanCode = huffmanCodes[symbol];

        HuffmaNode* current = tree.get();
        for (size_t i = lengths[symbol]; i-- > 0;) {
            HuffmaNode** which = ((huffmanCode & (1LL << i)) == 0) ? &current->left : &current->right;
            if (*which == nullptr) {
                *which = new HuffmaNode();
            }

            current = *which;
        }

        current->code = 1 + symbol;
    }

    return tree;
}

std::unique_ptr<HuffmaNode> CreateStaticLiteralLengthTree() {
    std::vector<size_t> lengths{};
    lengths.resize(288);

    // DEFLATE Compressed Data Format Specification version 1.3 - https://www.ietf.org/rfc/rfc1951.txt 
    size_t ranges[4][3] = {
        {   0, 143, 8},
        { 144, 255, 9},
        { 256, 279, 7},
        { 280, 287, 8}
    };

    for (size_t range = 0; range < 4; range++) {
        for (size_t symbol = ranges[range][0]; symbol <= ranges[range][1]; symbol++) {
            lengths[symbol] = ranges[range][2];
        }
    }

    return CreateHuffmanTree(lengths, 9);
}

size_t inflate_length(size_t code, BitStream& stream) {
    switch (code) {
        case 257: return 3;
        case 258: return 4;
        case 259: return 5;
        case 260: return 6;
        case 261: return 7;
        case 262: return 8;
        case 263: return 9;
        case 264: return 10;

        case 265: return 11LL + stream.ReadBit();
        case 266: return 13LL + stream.ReadBit();
        case 267: return 15LL + stream.ReadBit();
        case 268: return 17LL + stream.ReadBit();

        case 269: return 19LL + stream.ReadLiteral(2);
        case 270: return 23LL + stream.ReadLiteral(2);
        case 271: return 27LL + stream.ReadLiteral(2);
        case 272: return 31LL + stream.ReadLiteral(2);

        case 273: return 35LL + stream.ReadLiteral(3);
        case 274: return 43LL + stream.ReadLiteral(3);
        case 275: return 51LL + stream.ReadLiteral(3);
        case 276: return 59LL + stream.ReadLiteral(3);

        case 277: return 67LL + stream.ReadLiteral(4);
        case 278: return 83LL + stream.ReadLiteral(4);
        case 279: return 99LL + stream.ReadLiteral(4);
        case 280: return 115LL + stream.ReadLiteral(4);

        case 281: return 131LL + stream.ReadLiteral(5);
        case 282: return 163LL + stream.ReadLiteral(5);
        case 283: return 195LL + stream.ReadLiteral(5);
        case 284: return 227LL + stream.ReadLiteral(5);

        case 285: return 258;
        default:
            Assert(false, "unhandled code");
    }
}

size_t inflate_code(BitStream& stream) {
    static std::unique_ptr<HuffmaNode> tree = CreateStaticLiteralLengthTree();

    HuffmaNode* current = tree.get();
    while (current->code == 0) {
        current = (stream.ReadBit()) ? current->right : current->left;
    }

    return current->code - 1;
}

size_t inflate_distance(BitStream& stream) {
    size_t code = stream.ReadCode(5);
    switch (code) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;

        case 4: return 5LL + stream.ReadBit();
        case 5: return 7LL + stream.ReadBit();

        case 6: return 9LL + stream.ReadLiteral(2);
        case 7: return 13LL + stream.ReadLiteral(2);

        case 8: return 17LL + stream.ReadLiteral(3);
        case 9: return 25LL + stream.ReadLiteral(3);

        case 10: return 33LL + stream.ReadLiteral(4);
        case 11: return 49LL + stream.ReadLiteral(4);

        case 12: return 65LL + stream.ReadLiteral(5);
        case 13: return 97LL + stream.ReadLiteral(5);

        case 14: return 129LL + stream.ReadLiteral(6);
        case 15: return 193LL + stream.ReadLiteral(6);

        case 16: return 257LL + stream.ReadLiteral(7);
        case 17: return 385LL + stream.ReadLiteral(7);

        case 18: return 513LL + stream.ReadLiteral(8);
        case 19: return 769LL + stream.ReadLiteral(8);

        case 20: return 1025LL + stream.ReadLiteral(9);
        case 21: return 1537LL + stream.ReadLiteral(9);

        default:
            Assert(false, "unhandled code");
    }
}

std::vector<uint8_t> inflate_block(BitStream& stream) {
    std::vector<uint8_t> output{};

    uint8_t bFinal = stream.ReadBit();
    uint8_t bType = stream.ReadBit() | stream.ReadBit() << 1;

    Assert(bType == 1, "invalid header");

    while (true) {
        size_t code = inflate_code(stream);
        if (code <= 255) {
            output.push_back((uint8_t)code);
        }
        else if (code == 256) {
            break;
        }
        else {
            size_t length = inflate_length(code, stream);
            size_t distance = inflate_distance(stream);
            for (size_t i = 0; i < length; i++) {
                output.push_back(output[output.size() - distance]);
            }
        }
    }

    return output;
}

std::vector<uint8_t> inflate(std::vector<uint8_t>& compressed) {
    uint8_t compressionMethodAndFlags = compressed[0];
    uint8_t compressionMethod = compressionMethodAndFlags & 0xF;
    Assert(compressionMethod == ZLIB_COMPRESSION_MODE_DEFLATE, "invalid zlib compression method");

    BitStream stream(compressed, 16);

    return inflate_block(stream);
}