/**
 * @file    ByteMessageChecksum.hpp
 * @brief   Implementation file for the ByteMessageChecksum class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageChecksum_hpp License
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

// constructor
/**
 * @brief  The constructor
 * @param  baseptr
 *         A pointer to the beginning on an array of bytes. The checksum 
 *         value is written to and read from this array.
 * @param  position
 *         A position into the array given by messagepointer. The checksum
 *         is calculated from baseptr up to but not including baseptr+position.
 *         The checksum value is written to an read from this position 
 *         in the array represented by baseptr.
 * @param  checksumFunctionPtr
 *         A pointer to a checksum function. In order for a function
 *         to be usable as a checksum function, it must take two parameters:
 *         A const pointer to an uint8_t array (i.e. "const uint8_t *") and
 *         the number of bytes to include in the checksum. In our case, 
 *         this number is equivalent to position. The function must return
 *         the same data type as the class is instantiated with.
 * @note   Values stored in array pointed to by baseptr at an after offset
 *         position are not included in the checksum calculation. 
 * @note   Because this class uses a ByteMessageField internally to manage
 *         array access the same limitation for usable data types apply.
 *         However, this should not be a problem. Checksum functions 
 *         should return unsigned integers, and those are usable.
 */
template <class T>
ByteMessageChecksum<T>::ByteMessageChecksum(uint8_t* baseptr, size_t position, T (*checksumFunctionPtr)(const uint8_t*, size_t))
    : bptr{baseptr}, 
      pos{position}, 
      checksum_function{checksumFunctionPtr}, 
      bmf{baseptr, position} {} // empty body

// assignment operator
// Delegates some work to assignment operator of ByteMessageField.
// Only the value of the checksum is copied, not the function pointer to
// calculate the checksum.
/**
 * @brief  The copy-assignment operator 
 * @param  bmcs
 *         A reference to a ByteMessageChecksum object.
 * @return A reference to a ByteMessageChecksum object.
 * @note   Copies the underlying ByteMessageField.
 */
template <class T>
ByteMessageChecksum<T>& ByteMessageChecksum<T>::operator= (const ByteMessageChecksum<T> &bmcs) {
    if (this == &bmcs) return *this;
    bmf = bmcs.bmf;
    return *this;
}

// calc()
/**
 * @brief  Calculate the checksum, but do not store it.
 * @return The calculated checksum.
 */
template <class T>
T ByteMessageChecksum<T>::calc(void) const {
    return checksum_function(bptr, pos);
}

// get()
/**
 * @brief  Get the stored checksum, do not (re-)calculate it.
 * @return The stored checksum.
 */
template <class T>
T ByteMessageChecksum<T>::get(void) const {
    return bmf.get();
}

// update()
/**
 * @brief  Calculate the checksum and store it.
 */
template <class T>
void ByteMessageChecksum<T>::update(void)  {
    bmf.set( calc() );
}

// check()
/**
 * @brief  Check if the (re-)calculated checksum matches the stored checksum.
 * @return true if calculated and stored checksum match exacly, false otherwise.
 */
template <class T>
bool ByteMessageChecksum<T>::check(void) const {
    return calc() == get();
}
