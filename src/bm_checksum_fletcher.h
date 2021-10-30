/**
 * @file    bm_checksum_fletcher.h
 * @brief   Header file for Fletcher's checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_fletcher_h License
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

#ifndef FletcherChecksum_h
#define FletcherChecksum_h

#include <stddef.h> // needed for size_t data type
#include <stdint.h> // needed for fixed size integer data types

uint8_t fletcher8_checksum(const uint8_t * data, size_t length);
uint16_t fletcher16_checksum(const uint8_t * data, size_t length);
uint32_t fletcher32_checksum(const uint8_t * data, size_t length);

// alias: original fletcher checksum is fletcher16
#define fletcher_checksum fletcher16_checksum ///< fletcher16_checksum is the original Fletcher's checksum

#endif
