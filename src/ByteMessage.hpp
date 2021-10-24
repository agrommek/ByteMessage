#include <string.h> // needed for memcpy()

// implement default constructor (without parameters)
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>::ByteMessage(void)
    : msgarr{0} {
    msgarr[0] = TYPE;
}

// implement copy constructor
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>::ByteMessage(const ByteMessage<TYPE, SIZE>& bm) {
    memcpy(msgarr, bm.msgarr, SIZE);
}

// implement assignment operator
// assignment operator does nothing! The real assignment must happen on
// the level of the members (ByteMessageField, ByteMessageChecksum, and
// ByteMessageFieldBlob).
template <uint8_t TYPE, size_t SIZE>
ByteMessage<TYPE, SIZE>& ByteMessage<TYPE, SIZE>::operator= (const ByteMessage<TYPE, SIZE>& bm) {
    return *this;
}

// implement type()
template <uint8_t TYPE, size_t SIZE>
uint8_t ByteMessage<TYPE, SIZE>::type(void) {
    return TYPE;
}

// implement size()
template <uint8_t TYPE, size_t SIZE>
size_t ByteMessage<TYPE, SIZE>::size(void) {
    return SIZE;
}

// implement get_ptr()
template <uint8_t TYPE, size_t SIZE>
const uint8_t* ByteMessage<TYPE, SIZE>::get_ptr(void) const {
    return msgarr;
}

// implement populate()
template <uint8_t TYPE, size_t SIZE>
bool ByteMessage<TYPE, SIZE>::populate(const uint8_t * raw_message, size_t message_size) {
    // only allow population of internal array if type and size are both correct
    if ( (size() != message_size) || (*raw_message != type()) ) {
        return false;
    }
    else {
        memcpy(msgarr, raw_message, size());
        return true;
    }
}
