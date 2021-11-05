/**
 * @file    bm_checksum_twosum.cpp
 * @brief   Implementation file for two's complement checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_twosum_cpp License
 * 
 * The MIT Licence (MIT)
 * 
 * Copyright (c) 2021 Andreas Grommek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bm_checksum_twosum.h"

/**
 * @brief   Twos's complement sum over single bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum value
 */
uint8_t sum8_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum = 0;
    const uint8_t * const end = data + length;
    while (data < end) {
        sum += *data++;
    }
    return static_cast<uint8_t>(sum & UINT8_MAX);
}

/**
 * @brief   Twos's complement sum over pairs of bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is uneven, data is implicity padded with a zero.
 * @return  The checksum value
 */
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

/**
 * @brief   Twos's complement sum over groups of four bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by four, data is implicity padded with zeros.
 * @return  The checksum value
 */
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

/**
 * @brief   Twos's complement sum over groups of eight bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by eight, data is implicity padded with zeros.
 * @return  The checksum value
 */
uint64_t sum64_checksum(const uint8_t * data, size_t length) {
    uint_fast64_t sum = 0;
    const uint_fast8_t modulus = length & 0x07; // same as length % 8
    const uint8_t * const end = data + length - modulus;
    uint_fast64_t addend;
    while (data < end) {
        addend = 0;
        addend |= static_cast<uint_fast64_t>(*data++) << 56;
        addend |= static_cast<uint_fast64_t>(*data++) << 48;
        addend |= static_cast<uint_fast64_t>(*data++) << 40;
        addend |= static_cast<uint_fast64_t>(*data++) << 32;
        addend |= static_cast<uint_fast64_t>(*data++) << 24;
        addend |= static_cast<uint_fast64_t>(*data++) << 16;
        addend |= static_cast<uint_fast64_t>(*data++) << 8;
        addend |= static_cast<uint_fast64_t>(*data++);
        sum += addend;
    }
    // handle remaining bytes
    addend = 0;
    for (uint_fast8_t i = 0; i < modulus; i++)  {
        addend |= static_cast<uint_fast64_t>(*data++) << (56-i*8);
    }
    sum += addend;
    return static_cast<uint64_t>(sum & UINT64_MAX);
}
