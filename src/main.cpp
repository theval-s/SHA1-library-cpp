//
// Created by Val on 11/12/2024.
//

#include <iostream>
#include "library.hpp"
#include <string>
#include <vector>
bool test(const std::string& input, const std::string& expected) {
    if (SHA1::hash_from_string(input) != expected) return false;
    return true;
}
int main() {
    std::vector<std::pair<std::string, std::string>> test_vector{
        {"abc", "A9993E364706816ABA3E25717850C26C9CD0D89D"},
        {"", "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"},
        {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983E441C3BD26EBAAE4AA1F95129E5E54670F1"},
        {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "A49B2446A02C645BF419F995B67091253A04A259"}
    };


    /*std::string long_a_string(1e9,'a');
    //for (int i = 0; i < 1e9; i++) long_a_string+='a';
    test_vector.emplace_back(long_a_string, "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F");
    */

    for (int i = 0; i < test_vector.size();i++) {
        if (!test(test_vector[i].first, test_vector[i].second)) {
            std::cerr << "Test " << i << " failed" << std::endl;
            std::cerr << "\nExpected: " << test_vector[i].second << "\nReceived: " <<
                SHA1::hash_from_string(test_vector[i].first) << std::endl;
        }
    }

    return 0;
}