/**
 * @file    bm_checksum_onesum.cpp
 * @brief   Implementation file for one's complement checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_onesum_cpp License
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

#include "bm_checksum_onesum.h"

/* One's complement checksum over different sizes */

/* one byte version */

// general implementation straight from the textbook
/**
 * @brief   One's complement sum over single bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum value
 * @note    This is an optimized version with less overhead than the 
 *          textbook version.
 */
uint8_t onesum8_checksum_textbook(const uint8_t * data, size_t length) {
    uint_fast16_t  sum = 0;
    while (length > 0) {
        sum += static_cast<uint_fast16_t>(*data++);
        sum = (sum >> 8) + (sum & 0xFF);
        length -= 1;            
    }
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint8_t>(sum));
}

// optimized implementation
/**
 * @brief   One's complement sum over single bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum value
 * @note    This is the textbook version und quite inefficient. Only
 *          use this to verify correctnes for optimized implementations.
 */
uint8_t onesum8_checksum(const uint8_t * data, size_t length) {
    constexpr size_t blocklength_limit = 256;
    uint_fast16_t sum = 0;
    while (length > 0) {
        size_t blocklength = (length > blocklength_limit) ? blocklength_limit : length;
        length -= blocklength;
        while (blocklength > 0) {
            sum += (static_cast<uint_fast16_t>(*data++));
            blocklength -= 1;
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 8) {
            sum = (sum >> 8) + (sum & 0xFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint8_t>(sum));
}

/*
 *  two byte versions 
 * 
 * onesum16_checksum is the internet checksum according to RFC1071
 * (https://datatracker.ietf.org/doc/html/rfc1071)
 *
 * Handle stream of bytes as stream of big-endian 16 bit unsigned
 * numbers. If number of bytes is uneven, pad with zeros in the
 * end.
 * 
 * The result of the one's complement addition is negated to give
 * the final checksum, as demanded by the standard.
 * 
 * Note: The negation of the calculation to give the final checksum
 * is done in order for a byte stream with the checksum appended
 * will result in a checksum equal to 0. However, this is ONLY
 * true if the checksum starts on an even byte number. If one 
 * wanted to append the checksum to a byte stream with uneven
 * length, one would have to insert an 0x00 pad byte first, i.e. 
 * make the implicitly assumed trailing 0-byte explicit. If the
 * checksum is embedded within the byte stream, make sure it starts 
 * on an even-byte-boundary.
 */

// optimized implementation
/**
 * @brief   One's complement sum over pairs of bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is uneven, data is implicity padded with a zero.
 * @return  The checksum value
 * @note    This is an optimized version with less overhead than the 
 *          textbook version.
 * @note    This is the "internet checksum" described in RFC1071.
 */
uint16_t onesum16_checksum(const uint8_t * data, size_t length) {
    // Note: blocklength_limit must be even for the calculate-last-byte logic to work correctly!!
    constexpr size_t blocklength_limit = 2*254;
    uint_fast32_t  sum = 0;
    // handle all complete byte-pairs 
    while (length > 0) {
        // blocklength is even, except maybe during last iteration if len is uneven
        size_t blocklength = (length > blocklength_limit) ? blocklength_limit : length;
        length -= blocklength;
        while (blocklength > 1) {
            sum += (static_cast<uint_fast32_t>(*data++) << 8);
            sum += static_cast<uint_fast32_t>(*data++);
            blocklength -= 2;
        }
        // handle last byte if input length was uneven
        if (blocklength == 1) {
            sum += static_cast<uint_fast32_t>(*data) << 8;
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 16) {
            sum = (sum >> 16) + (sum & 0xFFFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint16_t>(sum));
}

// general implementation straight from the textbook
/**
 * @brief   One's complement sum over pairs of bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is uneven, data is implicity padded with a zero.
 * @return  The checksum value
 * @note    This is the textbook version und quite inefficient. Only
 *          use this to verify correctnes for optimized implementations.
 * @note    This is the "internet checksum" described in RFC1071.
 */
uint16_t onesum16_checksum_textbook(const uint8_t * data, size_t length) {
    uint_fast32_t  sum = 0;
    while (length > 1) {
        const uint_fast32_t hi_byte = static_cast<uint_fast32_t>(*data++) << 8;
        const uint_fast32_t lo_byte = static_cast<uint_fast32_t>(*data++);
        sum += (hi_byte | lo_byte);
        sum = (sum >> 16) + (sum & 0xFFFF);
        length -= 2;            
    }
    // handle last byte if input length was uneven
    if (length == 1) {
        sum += static_cast<uint_fast32_t>(*data) << 8;
    }
    // Add the two most significant bytes (the carry) 
    // to the two least significant bytes.
    // We only have to do this once when assuming length <= 2*2^16 bytes.
    sum = (sum >> 16) + (sum & 0xFFFF);
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint16_t>(sum));
}

/* four byte version */

// optimized implementation
/**
 * @brief   One's complement sum over groups of four bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by four, data is implicity padded with zeros.
 * @return  The checksum value
 * @note    This is an optimized version with less overhead than the 
 *          textbook version.
 */
uint32_t onesum32_checksum(const uint8_t * data, size_t length) {
    // Note: blocklength_limit must be dividable by 4 for the calculate-last-bytes logic to work correctly!!
    constexpr size_t blocklength_limit = 4*254;
    uint_fast64_t  sum = 0;
    uint_fast64_t bytes[4] = {0, 0, 0, 0};
    uint_fast8_t modulus = length & 0x03; // same as length % 4
    // handle all complete byte-quartets
    while (length > 0) {
        // blocklength is dividable by four, except maybe during last iteration if length is uneven
        size_t blocklength = (length > blocklength_limit) ? blocklength_limit : length;
        length -= blocklength;
        while (blocklength > 3) {
            bytes[0] = static_cast<uint_fast64_t>(*data++) << 24;
            bytes[1] = static_cast<uint_fast64_t>(*data++) << 16;
            bytes[2] = static_cast<uint_fast64_t>(*data++) << 8;
            bytes[3] = static_cast<uint_fast64_t>(*data++) << 0;
            sum += (bytes[0] | bytes[1] | bytes[2] | bytes[3]);
            blocklength -= 4;
        }
        // handle remaining bytes if input length was not dividable by four
        for (uint_fast8_t i=0; i<modulus; i++) {
            sum += static_cast<uint_fast16_t>(*data) << (24-(8*i));
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 32) {
            sum = (sum >> 32) + (sum & 0xFFFFFFFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint32_t>(sum));
}

// general implementation straight from the textbook
/**
 * @brief   One's complement sum over groups of four bytes
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum. If length
 *          is not devidable by four, data is implicity padded with zeros.
 * @return  The checksum value
 * @note    This is the textbook version und quite inefficient. Only
 *          use this to verify correctnes for optimized implementations.
 */
uint32_t onesum32_checksum_textbook(const uint8_t * data, size_t length) {
    uint_fast64_t  sum = 0;
    uint_fast64_t bytes[4] = {0, 0, 0, 0};
    uint_fast8_t modulus = length & 0x03; // same as length % 4
    while (length > 3) {
        bytes[0] = static_cast<uint_fast64_t>(*data++) << 24;
        bytes[1] = static_cast<uint_fast64_t>(*data++) << 16;
        bytes[2] = static_cast<uint_fast64_t>(*data++) << 8;
        bytes[3] = static_cast<uint_fast64_t>(*data++) << 0;
        sum += (bytes[0] | bytes[1] | bytes[2] | bytes[3]);
        sum = (sum >> 32) + (sum & 0xFFFFFFFF);
        length -= 4;            
    }
    // handle remaining bytes if input length was not dividable by four
    for (uint_fast8_t i=0; i<modulus; i++) {
        sum += static_cast<uint_fast16_t>(*data) << (24-(8*i));
    }
    sum = (sum >> 32) + (sum & 0xFFFFFFFF);
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint32_t>(sum));
}
