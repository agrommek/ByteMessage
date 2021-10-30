/**
 * @file    bm_checksum_fletcher.cpp
 * @brief   Implementation file for Fletcher's checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_fletcher_cpp License
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

#include "bm_checksum_fletcher.h"

/**
 * @brief   Fletcher's checksum with single-byte return value.
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum as an unsigned integer number of correct width.
 * @note    Works internally with half-bytes (nibbles).
 */
uint8_t fletcher8_checksum(const uint8_t * data, size_t length) {
    constexpr uint_fast8_t base = 15; // 0x0F
    uint_fast8_t sum1 = 0;
    uint_fast8_t sum2 = 0;
    for (size_t i=0; i< length; i++) {
        // most significant nibble first
        sum1 = (sum1 + (data[i] >> 4)); // defer modulus operation
        sum2 = (sum2 + sum1);           // defer modulus operation
        // least significant nibble last
        sum1 = (sum1 + (data[i] & 0x0F)) % base;
        sum2 = (sum2 + sum1) % base;
    }
    return static_cast<uint8_t>(sum2 << 4 | sum1);
}

/**
 * @brief   The original Fletcher's checksum
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. 
 * @return  The checksum as an unsigned integer number of correct width.
 */
uint16_t fletcher16_checksum(const uint8_t * data, size_t length) {
    constexpr uint_fast8_t base = 255; // 0xFF
    /*
    prevent overflow of sum1 with deferred modulus operation
    solve for n with n > 0 and n*(n+1)/2*base < UINT_FASTxx_MAX
    --> solve n^1 + n + (1 - 2 * UINT FASTxx_MAX / base) = 0
    */
    constexpr uint_fast16_t blocksize_limit = 
        (UINT_FAST16_MAX == 0xFFFFFF) ?                 362 : // 24 bit
        (UINT_FAST16_MAX == 0xFFFFFFFF) ?              5803 : // 32 bit
        (UINT_FAST16_MAX == 0xFFFFFFFFFF) ?           92862 : // 40 bit
        (UINT_FAST16_MAX == 0xFFFFFFFFFFFF) ?       1485814 : // 48 bit
        (UINT_FAST16_MAX == 0xFFFFFFFFFFFFFF) ?    23773043 : // 56 bit
        (UINT_FAST16_MAX == 0xFFFFFFFFFFFFFFFF) ? 380368696 : // 64 bit 
                                                         22 ; // else assume 16 bit
    uint_fast16_t sum1 = 0;
    uint_fast16_t sum2 = 0;
    while (length > 0) {
        uint_fast16_t blocksize = (length > blocksize_limit) ? blocksize_limit : length;
        length -= blocksize;
        while (blocksize > 0) {
            sum1 += *data++;
            sum2 += sum1;
            blocksize--;
        }
        sum1 %= base;
        sum2 %= base;
    }
    return static_cast<uint16_t>(sum2 << 8 | sum1);
}

/**
 * @brief   Fletcher's checksum taken over pairs of bytes.
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is uneven, an implicit 0 is added to data.
 * @return  The checksum as an unsigned integer number of correct width.
 */
uint32_t fletcher32_checksum(const uint8_t * data, size_t length) {
    constexpr uint_fast16_t base = 65535; // 0xFFFF
    /*
    prevent overflow of sum1 with deferred modulus operation
    solve for n with n > 0 and n*(n+1)/2*base < UINT_FASTxx_MAX
    --> solve n^1 + n + (1 - 2 * UINT FASTxx_MAX / base) = 0
    Also make sure that blocksize_limit is even!
    */
    constexpr uint_fast32_t blocksize_limit = 
        (UINT_FAST32_MAX == 0xFFFFFFFFFF) ?           5792 : // 40 bit
        (UINT_FAST32_MAX == 0xFFFFFFFFFFFF) ?        92682 : // 48 bit
        (UINT_FAST32_MAX == 0xFFFFFFFFFFFFFF) ?    1482920 : // 56 bit
        (UINT_FAST32_MAX == 0xFFFFFFFFFFFFFFFF) ? 23726746 : // 64 bit 
                                                       360 ; // else assume 32 bit
    const uint_fast8_t modulus = length & 0x01; // same as length % 2
    uint_fast32_t sum1 = 0;
    uint_fast32_t sum2 = 0;
    length -= modulus; // make sure we handle an even number 
    while (length > 1) {
        uint_fast32_t blocksize = (length > blocksize_limit) ? blocksize_limit : length;
        length -= blocksize;
        while (blocksize > 0) {
            uint_fast16_t number16 = static_cast<uint_fast16_t>(*data++) << 8;
            number16 |= static_cast<uint_fast16_t>(*data++);
            sum1 += number16;
            sum2 += sum1;
            blocksize -= 2;
        }        
        sum1 %= base;
        sum2 %= base;       
    }
    if (modulus) {
        sum1 = (sum1 + (static_cast<uint_fast32_t>(*data) << 8)) % base;
        sum2 = (sum2 + sum1) % base;
    }
    return static_cast<uint32_t>(sum2 << 16 | sum1);
}
