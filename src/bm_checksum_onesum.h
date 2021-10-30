/**
 * @file    bm_checksum_onesum.h
 * @brief   Header file for one's complement checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_onesum_h License
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

#ifndef onesum_checksum_h
#define onesum_checksum_h

/* one's complement checksum over different sizes */

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

// versions optimized for speed - use these!
uint8_t onesum8_checksum(const uint8_t * data, size_t length);
uint16_t onesum16_checksum(const uint8_t * data, size_t length);
uint32_t onesum32_checksum(const uint8_t * data, size_t length);

// Textbook versions - straightforward implementation, but bad performance.
// Don't use in live code, but only for testing optimized implementations.
uint8_t onesum8_checksum_textbook(const uint8_t * data, size_t length);
uint16_t onesum16_checksum_textbook(const uint8_t * data, size_t length);
uint32_t onesum32_checksum_textbook(const uint8_t * data, size_t length);

// aliases: one's complement sum over 16 bit is the internet checksum
#define internet_checksum onesum16_checksum ///< alias: one's complement sum over 16 bit is also called "internet checksum" (RFC1071).

#endif
