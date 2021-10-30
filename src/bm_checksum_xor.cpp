/**
 * @file    bm_checksum_xor.cpp
 * @brief   Implementation file for XOR checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_xor_cpp License
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

#include "bm_checksum_xor.h"

/**
 * @brief   Simple XOR checksum over single bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum value
 */
uint8_t xor8_checksum(const uint8_t * data, size_t length) {
    uint_fast8_t sum = 0;
    const uint8_t * const end = data + length;
    while (data < end) {
        sum ^= static_cast<uint_fast8_t>(*data++);
    }
    return static_cast<uint8_t>(sum);
}

/**
 * @brief   Simple XOR checksum over pairs of bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is uneven, data is implicity padded with a zero.
 * @return  The checksum value
 */
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

/**
 * @brief   Simple XOR checksum over groups of four bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by four, data is implicity padded with zeros.
 * @return  The checksum value
 */
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

/**
 * @brief   Simple XOR checksum over groups of eight bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by eight, data is implicity padded with zeros.
 * @return  The checksum value
 */
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
