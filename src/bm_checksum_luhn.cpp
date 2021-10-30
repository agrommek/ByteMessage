/**
 * @file    bm_checksum_luhn.cpp
 * @brief   Implementation file for Luhn's mod-N checksum functions, to be used with the ByteMessageChecksum class.
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_bm_checksum_luhn_cpp License
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

#include "bm_checksum_luhn.h"

/**
 * @brief   Luhn's mod-N checksum, straight from the textbook
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @param   base
 *          The base of the numbering scheme. For "classic" applications
 *          (like checksums for credit cards) use base=10. base=0 means
 *          a base of 256.
 * @return  The checksum value
 * @note    This is the textbook version and thus very inefficient. Only
 *          use this to check correctness of more optimized versions.
 */
uint8_t luhn_checksum_textbook(const uint8_t* data, size_t length, uint8_t base) {
    // define internal base as larger data type, to be able to handle
    // a base of 256.
    // base == 0 means intbase == 256
    const uint_fast16_t intbase = (base == 0) ? 256 : base ;
    
    // The last (i.e. rightmost) non-checksum byte must be multiplied by 2.
    // --> Start with 1 for even length, 2 for uneven length
    uint_fast16_t factor = (length % 2 == 0) ? 1 : 2; 
    
    // use uint16_t data type to be able to handle sums larger than 255
    uint_fast16_t sum = 0;
    
    for (size_t i = 0; i < length; i++) { 
        const uint_fast16_t addend = factor * static_cast<uint_fast16_t>(data[i]); 
        sum += (addend / intbase) + (addend % intbase) ;
        // Do modulus every loop iteration to prevent overflow.
        // Necessary for large lengths.
        // Correct, but very inefficient.
        sum %= intbase;
        // toogle factor for next iteration: 2, 1, 2, 1, ...
        factor = (factor == 2) ? 1 : 2;
    }
    // we have to do a final mod intbase, in case sum == 0
    return static_cast<uint8_t>( (intbase - sum) % intbase );
}

/*
 * Version of Luhn's checksum algorithm optimized for speed.
 * Optimizations:
 * - Use inner and outer loop, so we have to use modulus operation only
 *   once in a while.
 * - Partly unroll the inner loop in order to skip on calculating factor.
 * - Use subtraction instead of modulus/division or shifting/masking 
 *   when calculating the addend for factor == 2.
 * - Only do modulus calculation for addend when really necessary.
 * - Use addend for factor == 1 directly (i.e. no modulus operation at all).
 */
/**
 * @brief   Luhn's mod-N checksum
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @param   base
 *          The base of the numbering scheme. For "classic" applications
 *          (like checksums for credit cards) use base=10. base=0 means
 *          a base of 256.
 * @return  The checksum value
 * @note    This is an optimized version with less overhead than the 
 *          textbook version. When in doubt, use this one.
 */
uint8_t luhn_checksum(const uint8_t* data, size_t length, uint8_t base) {
    // define internal base as larger data type
    // base==0 means base==256
    const uint_fast16_t intbase = (base == 0) ? 256 : base ;
    const uint_fast8_t modulus = length & 0x01;

    // use uint(_fast)16_t data type to be able to handle sums larger than 255
    uint_fast16_t sum = 0;
    uint_fast16_t addend = 0;
    
    // Choose blocksize_limit so that adding this many numbers in the 
    // chosen base does not yet lead to overflow. 
    // Factor 2 is necessary because we do two additions per operation.
    // Subtraction of 1 from blocksize_limit is necessary, because sum may contain a value 
    // as large as base-1 after an iteration over a complete block.
    // The bitmask makes sure that blocksize_limit is even, which is necessary 
    // for correct operation.
    const uint_fast16_t blocksize_limit = ((UINT_FAST16_MAX / (2*(intbase-1))) - 1) & ~static_cast<uint_fast16_t>(0x01);
    // Start summing up number from the end of the data buffer, working forward.
    // This has the advantage that we *know* to start with factor == 2, making
    // loop unrolling feasible.
    const uint8_t * end = data + length - 1;
    // make sure length is even
    // necessary for correct operation
    length -= modulus; 
    while (length > 0) {
        uint_fast16_t blocklength = (length > blocksize_limit) ? blocksize_limit : length;
        length -= blocklength;
        while (blocklength > 0) {
            // handle terms with factor == 2
            addend = static_cast<uint_fast16_t>(*end--) * 2;
            // calculate remainder by subtraction, if necessary
            sum += (addend < intbase) ? addend : addend - (intbase-1);
            // Handle terms with factor == 1
            // Those terms are always < intbase
            // --> modulus is never necessary
            // --> use directly
            sum += static_cast<uint_fast16_t>(*end--);
            blocklength -= 2;
        }
        // do modulus to prevent overflow only after every block 
        sum %= intbase;
    }
    // handle remaining byte, which must be treated with factor == 2
    // same code as above
    if (modulus) {
        addend = static_cast<uint_fast16_t>(*end) * 2;
            sum += (addend < intbase) ? addend : addend - (intbase-1);
        // do another modulus, if necessary
        // this time through cheaper subtraction
        sum = (sum < intbase) ? sum : sum - intbase;
    }
    return static_cast<uint8_t>( (intbase - sum) % intbase );
}

/*
 * Specialized implementation of Luhn's checksum for base = 256.
 * Due to the overflow properties we can skip on most modulus operations.
 * We don't care what sizeof(uint_fast16_t) actually is, because all possible
 * widths are a multiple of 256, resulting in desireable and correct 
 * overflow properties.
 */
/**
 * @brief   Luhn's mod-N checksum for base 256
 * @param   data
 *          Pointer to array with bytes to calculate the checksum over.
 * @param   length
 *          Number of bytes in data to calculate the checksum.
 * @return  The checksum value
 * @note    This version may or may not be faster than the general implementation,
 *          depending on input length and compiler optimization settings.
 *          Your mileage may vary!! Benchmark for your actual use case!
 */
uint8_t luhn256_checksum(const uint8_t* data, size_t length) {
    // define base
    constexpr uint_fast16_t base256 = 256;
    const uint8_t * end = data + length - 1; // pointer to last number
    uint_fast8_t sum = 0;
    // handle all byte pairs
    while (length > 1) {
        // handle term with factor == 2
        const uint_fast16_t addend = 2 * static_cast<uint_fast16_t>(*end--); 
        sum += (addend < base256) ? addend : addend-0xFF;
        // handle term with factor == 1
        sum += static_cast<uint_fast16_t>(*end--);
        length -= 2;        
    }
    // handle remaining byte, always with factor == 2
    if (length == 1) {
        const uint_fast16_t addend = 2 * static_cast<uint_fast16_t>(*end); 
        sum += (addend < base256) ? addend : addend-0xFF;
    }
    sum &= 0xFF; // same as sum %= 256, in case sizeof(uint_fast8_t) > 1
    return static_cast<uint8_t>( base256 - sum ); // use overflow properties in case of sum = 0
}
