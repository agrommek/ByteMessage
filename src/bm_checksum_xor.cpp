#include "bm_checksum_xor.h"

uint8_t xor8_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum = 0;
    const uint8_t * const end = data + length;
    while (data < end) {
        sum ^= static_cast<uint_fast8_t>(*data++);
    }
    return static_cast<uint8_t>(sum);
}

uint16_t xor16_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum_msb = 0;
    uint_fast8_t sum_lsb = 0;
    const uint_fast8_t modulus = length & 0x01; // same as length % 2
    const uint8_t * const end = data + length - modulus;
    // independently XOR together bytes for all complete two-byte-blocks
    while (data < end) {
        sum_msb ^= static_cast<uint_fast8_t>(*data++);
        sum_lsb ^= static_cast<uint_fast8_t>(*data++);
    }
    // XOR potential last remaining byte
    if (modulus) {
        sum_msb ^= static_cast<uint_fast8_t>(*data);
    }
    // pack data together and return as uint16_t
    return static_cast<uint16_t>(sum_msb) << 8 | static_cast<uint16_t>(sum_lsb);
}

uint32_t xor32_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum[4] = {0, 0, 0, 0};
    const uint_fast8_t modulus = length & 0x03; // same as length % 4
    const uint8_t * const end = data + length - modulus;
    // independently XOR together bytes for all complete four-byte-blocks
    while (data < end) {
        sum[0] ^= static_cast<uint_fast8_t>(*data++);
        sum[1] ^= static_cast<uint_fast8_t>(*data++);
        sum[2] ^= static_cast<uint_fast8_t>(*data++);
        sum[3] ^= static_cast<uint_fast8_t>(*data++);
    }
    // XOR together the remaining 0..3 bytes
    for (uint_fast8_t i = 0; i < modulus; i++)  {
        sum[i] ^= static_cast<uint_fast8_t>(*data++);
    }
    // pack data together and return as uint32_t
    return static_cast<uint32_t>(sum[0]) << 24 | 
           static_cast<uint32_t>(sum[1]) << 16 | 
           static_cast<uint32_t>(sum[2]) <<  8 | 
           static_cast<uint32_t>(sum[3]);
}

uint64_t xor64_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    const uint_fast8_t modulus = length & 0x07; // same as length % 8
    const uint8_t * const end = data + length - modulus;
    // independently XOR together bytes for all complete four-byte-blocks
    while (data < end) {
        sum[0] ^= static_cast<uint_fast8_t>(*data++);
        sum[1] ^= static_cast<uint_fast8_t>(*data++);
        sum[2] ^= static_cast<uint_fast8_t>(*data++);
        sum[3] ^= static_cast<uint_fast8_t>(*data++);
        sum[4] ^= static_cast<uint_fast8_t>(*data++);
        sum[5] ^= static_cast<uint_fast8_t>(*data++);
        sum[6] ^= static_cast<uint_fast8_t>(*data++);
        sum[7] ^= static_cast<uint_fast8_t>(*data++);
    }
    // XOR together the remaining 0..7 bytes
    for (uint_fast8_t i = 0; i < modulus; i++)  {
        sum[i] ^= static_cast<uint_fast8_t>(*data++);
    }
    // pack data together and return as uint64_t
    return static_cast<uint64_t>(sum[0]) << 56 | 
           static_cast<uint64_t>(sum[1]) << 48 | 
           static_cast<uint64_t>(sum[2]) << 40 | 
           static_cast<uint64_t>(sum[3]) << 32 |
           static_cast<uint64_t>(sum[4]) << 24 | 
           static_cast<uint64_t>(sum[5]) << 16 | 
           static_cast<uint64_t>(sum[6]) <<  8 | 
           static_cast<uint64_t>(sum[7]);
}
