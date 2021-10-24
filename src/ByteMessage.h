#ifndef ByteMessage_h
#define ByteMessage_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

/* Note: This header file also includes the complete implementation from ByteMessage.hpp! */

/* 
 * Important points:
 *  - SIZE and TYPE are both template parameters
 * - type() and size() are implemented as static functions (not member variables)
 * - all functions are implemented within this header file (by including ByteMessage.hpp)
 * - We do NOT derive from a common base class because this has no advantages:
 *   We do a *composition* with ByteMessageFields, thus we cannot access member
 *   variables in derived classes from pointers of base classes.
 * - The copy constructor only copies the array.
 * - The assignment operator does nothing. Members in derived classes
 *   are responsible for data copies.
 */
 

template <uint8_t TYPE, size_t SIZE>
class ByteMessage {

    public:
        ByteMessage(void);                              // default constructor
        ByteMessage(const ByteMessage &bm);             // copy constructor
        virtual ~ByteMessage() = default;               // default public virtual destructor
        ByteMessage& operator= (const ByteMessage& bm); // assignment operator

        static uint8_t type(void);  // return message type
        static size_t size(void);   // return message size in bytes

        // return pointer to constant value(s)
        const uint8_t* get_ptr(void) const; 

        // populate message from raw byte array
        virtual bool populate(const uint8_t * raw_message, size_t message_size); 
            
    protected:
        uint8_t msgarr[SIZE];
};

// include implementation file
#include "ByteMessage.hpp"

#endif
