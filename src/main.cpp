//
// Created by Val on 11/12/2024.
//
#include <iostream>
#include "library.hpp"
#include <string>
#include <vector>

bool test(const std::string& input, const std::string& expected) {
    std::string received = SHA1::hash_from_string(input);
    if (received != expected) {
        std::cerr << "Mismatch! Received: " << received << "\nExpected: " << expected << std::endl;
        return false;
    }
    return true;
}

int main() {
    std::vector<std::pair<std::string, std::string>> test_vector{
        {"abc", "A9993E364706816ABA3E25717850C26C9CD0D89D"},
        {"", "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"},
        {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983E441C3BD26EBAAE4AA1F95129E5E54670F1"},
        {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "A49B2446A02C645BF419F995B67091253A04A259"},
        {"The quick brown fox jumps over the lazy dog", "2FD4E1C67A2D28FCED849EE1BB76E7391B93EB12"},
    };

    std::string long_a_string(1e6,'a');
    test_vector.emplace_back(long_a_string, "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F");

    std::string gb_string = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
    gb_string.reserve(16777216*64);
    for (int i = 1; i < 16777216; i++)
        gb_string += "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
    test_vector.emplace_back(gb_string, "7789F0C9EF7BFC40D93311143DFBE69E2017F592");


    bool failed = false;
    for (int i = 0; i < test_vector.size();i++) {
        if (!test(test_vector[i].first, test_vector[i].second)) {
            std::cerr << "Test " << i << " failed" << std::endl;
            failed = true;
        }
    }
    if (!failed) {
        std::cout << "All tests passed" << std::endl;
    }
    return 0;
}