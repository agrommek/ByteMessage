/**
 * @file    ByteMessageChecksum.h
 * @brief   Header file for the ByteMessageChecksum class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageChecksum_h License
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

#ifndef ByteMessageByteMessageChecksum_h
#define ByteMessageByteMessageChecksum_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

#include "ByteMessageField.h" // used internally

/* 
 * Note: All function definitions are included in the header file.
 * No .cpp file with explicit instantiations is necessary.
 */


/*
 * ByteMessageChecksum is a composition of a (private) ByteMessageField
 * and additional checksum functionality.
 * 
 * - use a class template to facilitate different checksum sizes (1 byte, two bytes, ...)
 * - internally (i.e. through compsition) use a ByteMessageField to put
 * - the checksum to / get the checksum from a byte array.
 * - make class final in order to not care about virtual functions and destructors
 */

/**
 * @class   ByteMessageChecksum
 * @brief   Templated class for all checksum fields in ByteMessage objects.
 * @details This class is meant to be used in a composition in a class
 *          derived from ByteMessage, i.e. as a data member.
 *          It facilitates calculation, of a checksum. The checksum
 *          function must be specified in the constructor.
 */
template <class T>
class ByteMessageChecksum final {
    public:
        // number of bytes for data type T
        static constexpr size_t size = sizeof(T); ///< Size of the checksum value in bytes

        // constructor - there is constructor without arguments
        ByteMessageChecksum(uint8_t* baseptr, size_t position, T (*checksumFunctionPtr)(const uint8_t*, size_t));

        // delete copy constructor
        ByteMessageChecksum(const ByteMessageChecksum &copy) = delete;
        
        // default destructor
        ~ByteMessageChecksum(void) = default;

        // assignment operator
        ByteMessageChecksum& operator= (const ByteMessageChecksum &bmcs);
        
        // calculate checksum and return it without storing it
        T calc(void) const;
        
        // return stored checksum without calculating it
        T get(void) const;
        
        // calculate checksum and store it
        void update(void);
        
        // check if calculaded checksum matches stored checksum
        bool check(void) const;

    private:
        // variables
        uint8_t* const bptr; // the base pointer, starting address for the checksum
        
        // the position/offset of basepointer to write the checksum to.
        // Also the length of the checksum calculation.
        const size_t pos;    
        
        // A function pointer to a function returning T and two function parameters: uint8_t* and size_t.
        // All checksum functions take a data array as a uint8_t* and a length as size_t.
        T (*checksum_function)(const uint8_t*, size_t);
        
        // internal ByteMessageField to handle interface to array.
        ByteMessageField<T> bmf;
};

// include implementation
#include "ByteMessageChecksum.hpp"

#endif
