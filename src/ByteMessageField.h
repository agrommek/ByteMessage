#ifndef ByteMessageField_h
#define ByteMessageField_h

#include <stdint.h> // needed for uint8_t data type
#include <stddef.h> // needed for size_t data type

/* declaration of class template */

template <class T>
class ByteMessageField final {
    public:
        // number of bytes for data type T
        static constexpr size_t size = sizeof(T);
        
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
