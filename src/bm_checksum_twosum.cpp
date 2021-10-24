#include "bm_checksum_twosum.h"

uint8_t sum8_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum = 0;
    const uint8_t * const end = data + length;
    while (data < end) {
        sum += *data++;
    }
    return static_cast<uint8_t>(sum & UINT8_MAX);
}

uint16_t sum16_checksum(const uint8_t * data, size_t length) {
    uint_fast16_t sum = 0;
    const uint_fast8_t modulus = length & 0x01; // same as length % 2
    const uint8_t * const end = data + length - modulus;
    while (data < end) {
        uint_fast16_t msb = static_cast<uint_fast16_t>(*data++) << 8;
        sum += msb | static_cast<uint_fast16_t>(*data++);
    }
    // handle remaining byte
    if (modulus) {
        sum += static_cast<uint_fast16_t>(*data) << 8;
    }
    return static_cast<uint16_t>(sum & UINT16_MAX);
}

uint32_t sum32_checksum(const uint8_t * data, size_t length) {
    uint_fast32_t sum = 0;
    const uint_fast8_t modulus = length & 0x03; // same as length % 4
    const uint8_t * const end = data + length - modulus;
    uint_fast32_t addend;
    while (data < end) {
        addend = 0;
        addend |= static_cast<uint_fast32_t>(*data++) << 24;
        addend |= static_cast<uint_fast32_t>(*data++) << 16;
        addend |= static_cast<uint_fast32_t>(*data++) << 8;
        addend |= static_cast<uint_fast32_t>(*data++);
        sum += addend;
    }
    // handle remaining bytes
    addend = 0;
    for (uint_fast8_t i = 0; i < modulus; i++)  {
        addend |= static_cast<uint_fast32_t>(*data++) << (24-i*8);
    }
    sum += addend;
    return static_cast<uint32_t>(sum & UINT32_MAX);
}

uint64_t sum64_checksum(const uint8_t * data, size_t length) {
    uint_fast64_t sum = 0;
    const uint_fast8_t modulus = length & 0x07; // same as length % 8
    const uint8_t * const end = data + length - modulus;
    uint_fast64_t addend;
    while (data < end) {
        addend = 0;
        addend |= static_cast<uint_fast32_t>(*data++) << 56;
        addend |= static_cast<uint_fast32_t>(*data++) << 48;
        addend |= static_cast<uint_fast32_t>(*data++) << 40;
        addend |= static_cast<uint_fast32_t>(*data++) << 32;
        addend |= static_cast<uint_fast32_t>(*data++) << 24;
        addend |= static_cast<uint_fast32_t>(*data++) << 16;
        addend |= static_cast<uint_fast32_t>(*data++) << 8;
        addend |= static_cast<uint_fast32_t>(*data++);
        sum += addend;
    }
    // handle remaining bytes
    addend = 0;
    for (uint_fast8_t i = 0; i < modulus; i++)  {
        addend |= static_cast<uint_fast32_t>(*data++) << (56-i*8);
    }
    sum += addend;
    return static_cast<uint32_t>(sum & UINT32_MAX);
}
