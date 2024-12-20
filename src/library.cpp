#include "library.hpp"
#include <bit> //endian check and rotl
#include <bitset>
#include <iostream>
#include <cstring>
#include <format>
#include <sstream>
#include <chrono>
/*
#if __cplusplus < 202002L //std::format is used
#error "Compile with C++20 support."
#endif*/

SHA1::SHA1() {
    //this is a library, constructor is private - so it's only accessible through function
    //that creates digest state once
    _digest = {h0, h1, h2, h3, h4};
}


std::string SHA1::hash_from_string(const std::string &data) {
    /* Handling padding in input with copying
    std::string input = data;
    input.reserve(data.length() + 64);
    input.append(1, 0x80);
    uint64_t len = input.size();
    //must be 448 mod 512 so 56bytes

    input.append(pad_len, 0x0);
    std::string len_pad(8,0);
    uint64_t original_len = data.size()*8;
    for (int i = 1; i <= 8 && original_len; i++) {
        len_pad[8-i] = static_cast<char>(original_len&0xFF);
        original_len >>= 8;
    }
    input += len_pad;
    */
    SHA1 sha;
    //auto start = std::chrono::high_resolution_clock::now();
    sha.compute_digest(data);

    //auto duration = std::chrono::high_resolution_clock::now() - start;
    //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << std::endl;

    return sha.digest_to_string();
}

void SHA1::compute_digest(const std::string &input) {
    /*if (input.empty() || (input.size()%64) != 0)
        throw std::invalid_argument("Input to compute_digest be a multiple of 64 bytes.");
    */
    uint64_t len = input.size();
    std::string last_block;
    int is_last_block = 0;
    for (int l = 0; l <= len; l += (BLOCK_SIZE / 8)) {
        if (l + 64 > len) {
            is_last_block = 1;
            last_block = input.substr(l, len % 64);
            last_block.append(1, 0x80);
            len++;
            uint64_t pad_len = (len % 64 == 56) ? 0 : (64 + 56 - len % 64) % 64;
            last_block.append(pad_len, 0x0);
            std::string len_pad(8, 0);
            uint64_t original_len = input.size() * 8;
            for (int j = 1; j <= 8 && original_len; j++) {
                len_pad[8 - j] = static_cast<char>(original_len & 0xFF);
                original_len >>= 8;
            }
            last_block += len_pad;
            if (last_block.size() > 64) is_last_block++;
            /* Debug value check
            std::cout << last_block.size() << std::endl;
            for (unsigned char c: last_block) {
                std::cout << std::format("{:02X} ", c);
            }
            std::cout << std::endl;
            */
        }
        for (int i = 0; i < 16; i++) {
            //operate with 512bit blocks
            //so thats 64 symbols
            uint32_t w = 0;
            is_last_block!=0
            ? memcpy(&w, &last_block[i*4], 4)
            : memcpy(&w, &input[l + (i * 4)], 4);
            if (std::endian::native == std::endian::little) {
                byte_reverse(w);
                //if i understood correct std::byteswap
                //can be used for this in C++23 and higher
            }

            _words[i] = w;
        }
        compute_block();
    }
    if (is_last_block == 2) {
        //TBH I'm a bit too lazy to implement this in more pretty way
        for (int i = 0; i < 16; i++) {
            uint32_t w = 0;
            memcpy(&w, &last_block[(BLOCK_SIZE/8)+i*4], 4);
            if (std::endian::native == std::endian::little) {
                byte_reverse(w);
            }
            _words[i] = w;
        }
        compute_block();
    }
}

void SHA1::compute_block() {
    for (int i = 16; i < 80; i++) {
        _words[i] = left_rotate(_words[i - 3] ^ _words[i - 8] ^ _words[i - 14] ^ _words[i - 16], 1);
    }

    uint32_t a = _digest[0],
            c = _digest[2],
            b = _digest[1],
            d = _digest[3],
            e = _digest[4];
    for (int i = 0; i < 80; i++) {
        uint32_t f = 0;
        uint32_t k;
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = k1;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = k2;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = k3;
        } else {
            f = b ^ c ^ d;
            k = k4;
        }
        uint32_t tmp = left_rotate(a, 5) + f + e + k + _words[i];
        e = d;
        d = c;
        c = left_rotate(b, 30);
        b = a;
        a = tmp;
    }
    _digest[0] += a;
    _digest[1] += b;
    _digest[2] += c;
    _digest[3] += d;
    _digest[4] += e;
}

std::string SHA1::digest_to_string() {
    std::stringstream result;
    //result << std::hex << std::setfill('0');
    for (int i = 0; i < 5; i++)
        result << std::format("{:02X}", _digest[i]);
    return result.str();
}


void SHA1::byte_reverse(uint32_t &num) {
    //this
    uint32_t t1 = num << 24, //last byte at 0x78000000
            t2 = num >> 24; //first byte at 0x00000012
    num &= 0x00FFFF00;
    num |= t1 | t2;
    t1 = (num >> 8) & 0x0000FF00; //0x00003400
    //0x00123456
    t2 = (num << 8) & 0x00FF0000; //0x00560000
    //0x34567800
    num &= 0xFF0000FF;
    num |= t1 | t2;
}
