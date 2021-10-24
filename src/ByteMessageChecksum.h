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

template <class T>
class ByteMessageChecksum final {
    public:
        // number of bytes for data type T
        static constexpr size_t size = sizeof(T);

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
