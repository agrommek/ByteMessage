/**
 * @file    ByteMessageFieldBlob.cpp
 * @brief   Implementation file for the ByteMessageFieldBlob class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageFieldBlob_cpp License
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

#include "ByteMessageFieldBlob.h"
#include <string.h> // needed for memcpy(), memset()

// constructor
/**
 * @brief  The constructor
 * @param  messagepointer
 *         A pointer to the beginning on an array of bytes. The the binary
 *         data is written to and read from this array.
 * @param  pos
 *         A position into the array given by messagepointer. The binary
 *         data is written to and read from this position in the array 
 *         represented by messagepointer.
 * @param  bloblength
 *         The length of the binary data in bytes.
 */
ByteMessageFieldBlob::ByteMessageFieldBlob(uint8_t * messagepointer, size_t pos, size_t bloblength) 
    : size{bloblength}, msgptr{messagepointer+pos} {
    zerofill(0); // pre-fill buffer with zeros
    return;
}

// assignment operator
/**
 * @brief  The copy-assignment operator 
 * @param  bmfb
 *         A reference to a ByteMessageFieldBlob object.
 * @return A reference to a ByteMessageFieldBlob object.
 * @note   Copies the underlying bytes. If the lengths of the two 
 *         ByteMessageFieldBlob objects do not match, copy only the smaller
 *         number of bytes from from bmfb. If the receiving
 *         ByteMessageFieldBlob is the larger one, the binary message is 
 *         padded with zeros. If it is the smaller one, the binary data
 *         is truncated.
 */
ByteMessageFieldBlob& ByteMessageFieldBlob::operator= (const ByteMessageFieldBlob &bmfb) {
    if (this == &bmfb) return *this;
    // pick the smaller size, copy only that many bytes
    const size_t length = (size > bmfb.size) ? bmfb.size : size ;
    // copy only length bytes over
    memcpy(msgptr, bmfb.msgptr, length);
    // fill rest with zeros (if any)
    zerofill(length);
    return *this;
}

// copy data from *data to the message
// return the number of bytes effectively copied
/**
 * @brief   Fill ByteMessageFieldBlob with data from an array.
 * @param   data
 *          Pointer to array of bytes to copy into the ByteMessageFieldBlob.
 * @param   length
 *          Number of bytes to copy from data. If length > size, not all
 *          data is copied. Check the return value!
 * @return  Number of bytes copied. This is either length (if length <=
 *          size), or size (if length > size).
 * @note    If length < size, the binary data written is padded with zeros.
 */
size_t ByteMessageFieldBlob::set(const uint8_t *data, size_t length) {
    if (length > size) length = size;
    // copy data
    memcpy(msgptr, data, length);
    // pad rest of buffer with zeros (if any)
    zerofill(length);
    return length; 
}

// set all bytes of blob in message to the same value
// return the number of bytes written (== size)
/**
 * @brief   Fill all bytes of ByteMessageFieldBlob with a single value.
 * @param   value
 *          The value to fill in.
 * @return  Number of bytes filled, always equals size.
 */
size_t ByteMessageFieldBlob::set(uint8_t value) {
    memset(msgptr, value, size);
    return size; 
}

// copy at most length bytes from message to *data
// return number of bytes effectively copied
/**
 * @brief   Copy bytes from ByteMessageFieldBlob to an existing array.
 * @details
 * @param   data
 *          Pointer to an array of uint8_t. The bytes stored in the
 *          ByteMessageFieldBlob are copied to this array.
 * @param   length
 *          The maximum number of bytes to be copied from ByteMessageFieldBlob
 *          to data array. If length < size, only length bytes are copied.
 *          If length >= size, at most size bytes are copied.
 * @return  Number of bytes which were actually copied to data.
 */
size_t ByteMessageFieldBlob::get(uint8_t *data, size_t length) const {
    if (length > size) length = size;
    memcpy(data, msgptr, length);
    return length;
}

// return a read-only pointer to the data blob
/**
 * @brief   Get read-only access to the underlying data array.
 * @details Use this function (rather than get()) if you want (or need) 
 *          to transfer the information byte-by-byte, e.g. if you want to 
 *          write the information to a serial output connections and do
 *          not want to store it somewhere in between.
 * @return  A pointer to constant data, pointing to the beginning of the
 *          underlying array.
 * @warning Using the pointer returned, you can read after the end of the
 *          ByteMessageFieldBlob.
 */
const uint8_t* ByteMessageFieldBlob::get_ptr(void) const {
    return static_cast<const uint8_t*>(msgptr);
}

// helper function to fill blob with zeros, starting at startpos 
// relative to start of blob
void ByteMessageFieldBlob::zerofill(size_t startpos) {
    if (startpos < size) {
        memset( (msgptr+startpos), 0, (size-startpos) );
    }
}
