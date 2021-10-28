/**
 * @file    ByteMessageFieldBlob.h
 * @brief   Header file for the ByteMessageFieldBlob class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageFieldBlob_h License
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

#ifndef ByteMessageFieldBlob_h
#define ByteMessageFieldBlob_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

/**
 * @class   ByteMessageFieldBlob
 * @brief   Class for generic data fields in ByteMessage objects.
 * @details This class can be used to add generic binary data (like hash
 *          values, encryption key, etc.) to a ByteMessage object. Data
 *          is stored as an array of uint8_t (i.e. single bytes).
 */
class ByteMessageFieldBlob final {
    public:
        // constructor
        // Note: There is *NO* error checking to prevent pos+bloblength > sizeof(messagepointer)
        ByteMessageFieldBlob(uint8_t * messagepointer, size_t pos, size_t bloblength); 
        
        // delete copy constructor
        ByteMessageFieldBlob(const ByteMessageFieldBlob &copy) = delete; ///< The copy constructor is explicitly deleted.
        
        // default destructor
        ~ByteMessageFieldBlob(void) = default; ///< The default destructor.
        
        // assignment operator
        ByteMessageFieldBlob& operator= (const ByteMessageFieldBlob &bmfb);
        
        size_t set(const uint8_t *data, size_t length);  // return number of bytes copied from data to msgptr
        size_t set(uint8_t value=0);                     // set whole blob to constant value
        size_t get(uint8_t * data, size_t length) const; // return number of bytes copied from msgptr to data
        const uint8_t* get_ptr(void) const;              // return pointer to constant data

        // public member variables
        const size_t size; ///< Size of the binary blob in bytes.
        
    private:
        // private helper functions
        void zerofill(size_t startpos);
        
        // private member variables
        uint8_t * const msgptr; // const pointer to non-const value
};

#endif
