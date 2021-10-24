#ifndef ByteMessageFieldBlob_h
#define ByteMessageFieldBlob_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

class ByteMessageFieldBlob final {
    public:
        // constructor
        // Note: There is *NO* error checking to prevent pos+bloblength > sizeof(messagepointer)
        ByteMessageFieldBlob(uint8_t * messagepointer, size_t pos, size_t bloblength); 
        
        // delete copy constructor
        ByteMessageFieldBlob(const ByteMessageFieldBlob &copy) = delete;
        
        // assignment operator
        ByteMessageFieldBlob& operator= (const ByteMessageFieldBlob &bmfb);
        
        size_t set(const uint8_t *data, size_t length);  // return number of bytes copied from data to msgptr
        size_t set(uint8_t value=0);                     // set whole blob to constant value
        size_t get(uint8_t * data, size_t length) const; // return number of bytes copied from msgptr to data
        const uint8_t* get_ptr(void) const;              // return pointer to constant data

        // public member variables
        const size_t size;
        
    private:
        // private helper functions
        void zerofill(size_t startpos);
        
        // private member variables
        uint8_t * const msgptr; // const pointer to non-const value
};

#endif
