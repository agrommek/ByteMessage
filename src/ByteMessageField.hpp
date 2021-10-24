#include <string.h>  // needed for memcpy()
#include "netconv.h" // get it from https://github.com/agrommek/netconv

/* common member function definitions */

// definition of constructor
template <class T>
ByteMessageField<T>::ByteMessageField(uint8_t * messagepointer, size_t pos) 
    : msgptr{messagepointer+pos} {}; // empty body

// definition of assignment operator
template <class T>
ByteMessageField<T>& ByteMessageField<T>::operator= (const ByteMessageField<T> &bmf) {
    if (this == &bmf) return *this;
    memcpy(msgptr, bmf.msgptr, sizeof(T));
    return *this;
}

/* function specializations for uint8_t */
template<> void ByteMessageField<uint8_t>::set(uint8_t value) { *msgptr = value; }
template<> uint8_t ByteMessageField<uint8_t>::get(void) const { return *msgptr; }

/* function specializations for int8_t */
template<> void ByteMessageField<int8_t>::set(int8_t value) { *msgptr = static_cast<uint8_t>(value); }
template<> int8_t ByteMessageField<int8_t>::get(void) const { return static_cast<int8_t>(*msgptr); }

/* function specializations for uint16_t */
template<> void ByteMessageField<uint16_t>::set(uint16_t value) { htons(value, msgptr); }
template<> uint16_t ByteMessageField<uint16_t>::get(void) const { return ntohs(msgptr); }

/* function specializations for int16_t */
template<> void ByteMessageField<int16_t>::set(int16_t value) { htons(static_cast<uint16_t>(value), msgptr); }
template<> int16_t ByteMessageField<int16_t>::get(void) const { return static_cast<int16_t>(ntohs(msgptr)); }

/* function specializations for uint32_t */
template<> void ByteMessageField<uint32_t>::set(uint32_t value) { htonl(value, msgptr); }
template<> uint32_t ByteMessageField<uint32_t>::get(void) const { return ntohl(msgptr); }

/* function specializations for int32_t */
template<> void ByteMessageField<int32_t>::set(int32_t value) { htonl(static_cast<uint32_t>(value), msgptr);}
template<> int32_t ByteMessageField<int32_t>::get(void) const { return static_cast<int32_t>(ntohl(msgptr)); }

/* function specializations for uint64_t */
template<> void ByteMessageField<uint64_t>::set(uint64_t value) { htonll(value, msgptr); }
template<> uint64_t ByteMessageField<uint64_t>::get(void) const { return ntohll(msgptr); }

/* function specializations for int64_t */
template<> void ByteMessageField<int64_t>::set(int64_t value) { htonll(static_cast<uint64_t>(value), msgptr); }
template<> int64_t ByteMessageField<int64_t>::get(void) const { return static_cast<int64_t>(ntohll(msgptr)); }

/* function specializations for float */
template<> void ByteMessageField<float>::set(float value) { htonf(value, msgptr); }
template<> float ByteMessageField<float>::get(void) const { return ntohf(msgptr); }

/* function specializations for double */
#if (__SIZEOF_DOUBLE__ == 8)
template<> void ByteMessageField<double>::set(double value) { htond(value, msgptr); }
template<> double ByteMessageField<double>::get(void) const { return ntohd(msgptr); }
#endif

/* complete class specialization for bool - definitions inside! */
template <>
class ByteMessageField<bool> final {
    public:
        // Represent bool as one byte in message field, because 
        // sizeof(bool) is implementation defined and might be larger
        // than one byte. --> Force it to use only one byte!
        static constexpr size_t size = 1;
        
        // constructor including implementation
        ByteMessageField<bool>(uint8_t * messagepointer, size_t pos=0) 
            : msgptr{messagepointer+pos} { *msgptr = 0; }
            
        // delete copy constructor
        ByteMessageField(const ByteMessageField &bmf) = delete;
        
        // default destructor
        ~ByteMessageField<bool>() = default;
        
        // assignment operator
        ByteMessageField& operator= (const ByteMessageField& bmf) {
            if (this == &bmf) return *this;
            *msgptr = *(bmf.msgptr);
            return *this;
        }        
        
        // represent 'true' as 1 and 'false' as 0 in a uint8_t
        void set(bool value) { *msgptr = (value) ? 1 : 0; }
        bool get(void) const { return static_cast<bool>(*msgptr); }
        
    private:
        // const pointer to non-const value
        uint8_t * const msgptr;
};
