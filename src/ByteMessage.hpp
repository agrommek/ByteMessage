/**
 * @file    ByteMessage.hpp
 * @brief   Implementation file for the ByteMessage class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessage_hpp License
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

#include <string.h> // needed for memcpy()

// implement default constructor (without parameters)
/**
 * @brief  The default constructor.
 * @note   Sets type automatically.
 */
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>::ByteMessage(void)
    : msgarr{0}, blackhole{0} {
    msgarr[0] = TYPE;
}

// implement copy constructor
/**
 * @brief  The copy constructor
 * @param  bm
 *         A reference to a ByteMessage object.
 * @note   Copies the whole array msgarr.
 */
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>::ByteMessage(const ByteMessage<TYPE, SIZE>& bm) {
    memcpy(msgarr, bm.msgarr, SIZE);
}

// implement assignment operator
// assignment operator does nothing! The real assignment must happen on
// the level of the members (ByteMessageField, ByteMessageChecksum, and
// ByteMessageFieldBlob).
/**
 * @brief  The copy-assignment operator 
 * @param  bm
 *         A reference to a ByteMessage object.
 * @return A reference to a ByteMessage object.
 * @note   The copy-assignment operator really does nothing. There is @b
 *         no copying of the underlying array. Data must be copied by the
 *         data members in the derived class(es).
 */
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>& ByteMessage<TYPE, SIZE>::operator= (const ByteMessage<TYPE, SIZE>&) { // note: no named parameter to prevent compiler warning
    return *this;
}

// implement read-only subscript operator
/**
 * @brief   Read-only subscript operator
 * @details Access the raw bytes of the underlying array like an array.
 *          The first element (i.e. index 0) will always be the type.
 *          This operator works read-only. You cannot change data in the
 *          underlying array by assigning to an array element.
 *          If you try to access an element by an out-of-bounds index, a 
 *          reference to a constant containing zero is returned. It is not
 *          possible to read out-of-bounds data using this operator.
 * @param   index
 *          The index into the underlying array.
 * @return  A constant reference to the element in the underlying array.
 */
template <uint8_t TYPE, size_t SIZE>
const uint8_t& ByteMessage<TYPE, SIZE>::operator[] (size_t index) const {
    if (index < SIZE) {
        return msgarr[index];
    }
    else {
        return blackhole;
    }
}

// implement get_ptr()
/**
 * @brief  Get a pointer to the unterlying array. 
 * @return A pointer to the underlying array. Note that the pointer is 
 *         read-only, you cannot change the data in the array through it.
 */
template <uint8_t TYPE, size_t SIZE>
const uint8_t* ByteMessage<TYPE, SIZE>::get_ptr(void) const {
    return msgarr;
}

// implement populate()
/**
 * @brief  Populate the a ByteMessage with data from an array.
 * @param  raw_message
 *         A pointer to a uint8_t array to copy the data from.
 * @param  message_size
 *         The number of bytes to copy from raw_message.
 * @return true if contents of raw_message were successfully copied to
 *         ByteMessage object, false otherwise. If false, the ByteMessage
 *         object was change in no way.
 * @note   A raw massage can only be used to populate a ByteMessage object 
 *         if the number of bytes to copy exactly matches the number of 
 *         bytes expected for this type of message AND if the first byte
 *         in raw_message reflects the correct type.
 */
template <uint8_t TYPE, size_t SIZE>
bool ByteMessage<TYPE, SIZE>::populate(const uint8_t * raw_message, size_t message_size) {
    // only allow population of internal array if type and size are both correct
    if ( (SIZE != message_size) || (*raw_message != TYPE) ) {
        return false;
    }
    else {
        memcpy(msgarr, raw_message, SIZE);
        return true;
    }
}
