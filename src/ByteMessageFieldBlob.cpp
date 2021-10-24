#include "ByteMessageFieldBlob.h"
#include <string.h> // needed for memcpy(), memset()

// constructor
ByteMessageFieldBlob::ByteMessageFieldBlob(uint8_t * messagepointer, size_t pos, size_t bloblength) 
    : size{bloblength}, msgptr{messagepointer+pos} {
    zerofill(0); // pre-fill buffer with zeros
    return;
}

// assignment operator
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
size_t ByteMessageFieldBlob::set(uint8_t value) {
    memset(msgptr, value, size);
    return size; 
}

// copy at most length bytes from message to *data
// return number of bytes effectively copied
size_t ByteMessageFieldBlob::get(uint8_t *data, size_t length) const {
    if (length > size) length = size;
    memcpy(data, msgptr, length);
    return length;
}

// return a read-only pointer to the data blob
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
