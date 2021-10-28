/**
 * @file    ByteMessage.h
 * @brief   Header file for the ByteMessage class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessage_h License
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

#ifndef ByteMessage_h
#define ByteMessage_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

/* Note: This header file also includes the complete implementation from ByteMessage.hpp! */

/* 
 * Important points:
 *  - SIZE and TYPE are both template parameters
 * - type and size() are implemented as static constexpr class members
 * - all functions are implemented within this header file (by including ByteMessage.hpp)
 * - We do NOT derive from a common base class because this has no advantages:
 *   We do a *composition* with ByteMessageFields, thus we cannot access member
 *   variables in derived classes from pointers of base classes.
 * - The copy constructor only copies the array.
 * - The assignment operator does nothing. Members in derived classes
 *   are responsible for data copies.
 */
 

/**
 * @class   ByteMessage
 * @brief   Templated base class for all ByteMessages.
 * @details This class is not meant to be instantiated directly, but to
 *          be derived from. It offers only basic functionality.
 */
template <uint8_t TYPE, size_t SIZE>
class ByteMessage {

    public:
        ByteMessage(void);                              // default constructor
        ByteMessage(const ByteMessage &bm);             // copy constructor
        virtual ~ByteMessage() = default;               // default public virtual destructor
        ByteMessage& operator= (const ByteMessage& bm); // assignment operator

        static constexpr uint8_t type = TYPE;           ///< The numeric type of the message.
        static constexpr size_t  size = SIZE;           ///< The size of the underlying array.

        // return pointer to constant value(s)
        virtual const uint8_t* get_ptr(void) const final; 

        // populate message from raw byte array
        virtual bool populate(const uint8_t * raw_message, size_t message_size) final; 
            
    protected:
        uint8_t msgarr[SIZE];                           ///< The underlying array to hold the actual message data.
};

// include implementation file
#include "ByteMessage.hpp"

#endif
