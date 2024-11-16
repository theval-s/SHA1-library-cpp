#ifndef CRYPTO_LAB6_LIBRARY_H
#define CRYPTO_LAB6_LIBRARY_H

#include <cstdint>
#include <string>
#include <array>

inline static uint32_t left_rotate(const uint32_t& x, const int& n) {
    return (x << n) | (x >> (32 - n));
} //bit library already has it but it's slower

class SHA1 {
public:
    static std::string hash_from_string(const std::string &input);
private:
    static constexpr uint32_t h0 = 0x67452301;
    static constexpr uint32_t h1 = 0xEFCDAB89;
    static constexpr uint32_t h2 = 0x98BADCFE;
    static constexpr uint32_t h3 = 0x10325476;
    static constexpr uint32_t h4 = 0xC3D2E1F0;

    static constexpr uint32_t k1 = 0x5A827999;
    static constexpr uint32_t k2 = 0x6ED9EBA1;
    static constexpr uint32_t k3 = 0x8F1BBCDC;
    static constexpr uint32_t k4 = 0xCA62C1D6;

    static constexpr int BLOCK_SIZE = 512;
    static constexpr int DIGEST_SIZE = 160;

    std::array<uint32_t,(DIGEST_SIZE/32)> _digest{};
    uint32_t _words[80];
    SHA1();
    void compute_digest(const std::string &input);
    void compute_block();
    std::string digest_to_string();
    void byte_reverse(uint32_t&);
};

#endif //CRYPTO_LAB6_LIBRARY_H