/* implementations for ByteMessageChecksum template class */

// constructor
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
template <class T>
ByteMessageChecksum<T>& ByteMessageChecksum<T>::operator= (const ByteMessageChecksum<T> &bmcs) {
    if (this == &bmcs) return *this;
    bmf = bmcs.bmf;
    return *this;
}

// calc()
template <class T>
T ByteMessageChecksum<T>::calc(void) const {
    return checksum_function(bptr, pos);
}

// get()
template <class T>
T ByteMessageChecksum<T>::get(void) const {
    return bmf.get();
}

// update()
template <class T>
void ByteMessageChecksum<T>::update(void)  {
    bmf.set( calc() );
}

// check()
template <class T>
bool ByteMessageChecksum<T>::check(void) const {
    return calc() == get();
}
