/**
 * @file    ByteMessageField.h
 * @brief   Header file for the ByteMessageField class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageField_h License
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

#ifndef ByteMessageField_h
#define ByteMessageField_h

#include <stdint.h> // needed for uint8_t data type
#include <stddef.h> // needed for size_t data type

/* declaration of class template */
/**
 * @class   ByteMessageField
 * @brief   Templated class for data fields in ByteMessage objects.
 * @note    This class can only be instantiated for the following data 
 *          types: uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t,
 *          int32_t, int64_t, bool, float, double.
 * @note    The size field will always have the value of sizeof(T),
 *          except for bool, where it is fixed to one byte.
 */
template <class T>
class ByteMessageField final {
    public:
        // number of bytes for data type T
        static constexpr size_t size = sizeof(T); ///< Size of the message field value in bytes
        
        // constructor
        ByteMessageField(uint8_t * messagepointer, size_t pos);
        
        // delete copy constructor
        ByteMessageField(const ByteMessageField &bmf) = delete;
        
        // default destructor
        ~ByteMessageField() = default;
        
        // assignment operator
        ByteMessageField& operator= (const ByteMessageField &bmf);

        // member function declarations  
        // Those exist as specialized-only versions, i.e. there is no 
        // implementation for generic type T.
        void set(T value); // set value of field
        T get(void) const; // get value from field

    private:
        // const pointer to non-const value
        uint8_t * const msgptr;
};

// include implementation
#include "ByteMessageField.hpp"

#endif
