/**
 * @file    ByteMessageField.hpp
 * @brief   Implementation file for the ByteMessageField class
 * @author  Andreas Grommek
 * @version 1.0.0
 * @date    2021-10-22
 * 
 * @section license_ByteMessageField_hpp License
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

#include <string.h>  // needed for memcpy()
/*
 * Note: We re-implement a bit of functionality from the netconv library
 * (see https://github.com/agrommek/netconv) in order to reduce external
 * dependencies.
 */

/* common member function definitions */

// definition of constructor
/**
 * @brief  The constructor
 * @param  messagepointer
 *         A pointer to an array of bytes. The encoded value is written 
 *         to and read from this array.
 * @param  pos
 *         A position into the array given by messagepointer. The encoded
 *         value is written to an read from this position in the array.
 */
template <class T>
ByteMessageField<T>::ByteMessageField(uint8_t * messagepointer, size_t pos) 
    : msgptr{messagepointer+pos} {}; // empty body

// definition of assignment operator
/**
 * @brief  The copy-assignment operator 
 * @param  bmf
 *         A reference to a ByteMessageField object.
 * @return A reference to a ByteMessageField object.
 * @note   Copies data from one underlying array to the other.
 */
template <class T>
ByteMessageField<T>& ByteMessageField<T>::operator= (const ByteMessageField<T> &bmf) {
    if (this == &bmf) return *this;
    memcpy(msgptr, bmf.msgptr, sizeof(T));
    return *this;
}

/**
 * @fn         template <class T> void ByteMessageField::set(T value)
 * @brief      Set value of ByteMessageField.
 * @details    The value is written to the underlying array.
 */

/**
 * @fn         template <class T> T ByteMessageField::get(void) const
 * @brief      Get value of ByteMessageField.
 * @details    The value is read from the underlying array.
 * @return     The value stored in the ByteMessageField object.
 */

/** @cond method_specializations */

/* function specializations for uint8_t */

template<> void ByteMessageField<uint8_t>::set(uint8_t value) {
    *msgptr = value;
}

template<> uint8_t ByteMessageField<uint8_t>::get(void) const {
    return *msgptr;
}

/* function specializations for int8_t */

template<> void ByteMessageField<int8_t>::set(int8_t value) {
    *msgptr = static_cast<uint8_t>(value);
}

template<> int8_t ByteMessageField<int8_t>::get(void) const {
    return static_cast<int8_t>(*msgptr);
}

/* function specializations for uint16_t */

template<> void ByteMessageField<uint16_t>::set(uint16_t value) {
//    htons(value, msgptr);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        value = __builtin_bswap16(value);
    #endif
    memcpy(msgptr, &value, sizeof(uint16_t));
}

template<> uint16_t ByteMessageField<uint16_t>::get(void) const {
    //return ntohs(msgptr);
    uint16_t v;
    memcpy(&v, msgptr, sizeof(uint16_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap16(v);
    #endif
    return v;
}

/* function specializations for int16_t */

template<> void ByteMessageField<int16_t>::set(int16_t value) {
//    htons(static_cast<uint16_t>(value), msgptr);
    uint16_t v = static_cast<uint16_t>(value);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap16(v);
    #endif
    memcpy(msgptr, &v, sizeof(uint16_t));
}

template<> int16_t ByteMessageField<int16_t>::get(void) const {
//    return static_cast<int16_t>(ntohs(msgptr));
    uint16_t v;
    memcpy(&v, msgptr, sizeof(uint16_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap16(v);
    #endif
    return static_cast<int16_t>(v);
}

/* function specializations for uint32_t */

template<> void ByteMessageField<uint32_t>::set(uint32_t value) {
//    htonl(value, msgptr);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        value = __builtin_bswap32(value);
    #endif
    memcpy(msgptr, &value, sizeof(uint32_t));
}

template<> uint32_t ByteMessageField<uint32_t>::get(void) const {
//    return ntohl(msgptr);
    uint32_t v;
    memcpy(&v, msgptr, sizeof(uint32_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap32(v);
    #endif
    return v;
}

/* function specializations for int32_t */

template<> void ByteMessageField<int32_t>::set(int32_t value) {
//    htonl(static_cast<uint32_t>(value), msgptr);
    uint32_t v = static_cast<uint32_t>(value);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap32(v);
    #endif
    memcpy(msgptr, &v, sizeof(uint32_t));
}

template<> int32_t ByteMessageField<int32_t>::get(void) const {
//    return static_cast<int32_t>(ntohl(msgptr));
    uint32_t v;
    memcpy(&v, msgptr, sizeof(uint32_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap32(v);
    #endif
    return static_cast<int32_t>(v);
}

/* function specializations for uint64_t */

template<> void ByteMessageField<uint64_t>::set(uint64_t value) {
//    htonll(value, msgptr);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        value = __builtin_bswap64(value);
    #endif
    memcpy(msgptr, &value, sizeof(uint64_t));
}

template<> uint64_t ByteMessageField<uint64_t>::get(void) const {
//    return ntohll(msgptr);
    uint64_t v;
    memcpy(&v, msgptr, sizeof(uint64_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap64(v);
    #endif
    return v;
}

/* function specializations for int64_t */

template<> void ByteMessageField<int64_t>::set(int64_t value) {
//    htonll(static_cast<uint64_t>(value), msgptr);
    uint64_t v = static_cast<uint64_t>(value);
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap64(v);
    #endif
    memcpy(msgptr, &v, sizeof(uint64_t));
}

template<> int64_t ByteMessageField<int64_t>::get(void) const {
//    return static_cast<int64_t>(ntohll(msgptr));
    uint64_t v;
    memcpy(&v, msgptr, sizeof(uint64_t));
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap64(v);
    #endif
    return static_cast<int64_t>(v);
}

/* function specializations for float */

template<> void ByteMessageField<float>::set(float value) {
//    htonf(value, msgptr);
    static_assert(sizeof(uint32_t) == sizeof(float));
    #if ( __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        // convert to unsigned integer, byte-swap, copy to array
        uint32_t v;
        memcpy(&v, &value, sizeof(float));
        v = __builtin_bswap32(v);
        memcpy(msgptr, &v, sizeof(uint32_t));
    #elif ( __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__ )
        // direct copy of float to array
        memcpy(msgptr, &value, sizeof(float));
    #else
        #error "float word order could not be determined!" 
    #endif
}

template<> float ByteMessageField<float>::get(void) const {
//    return ntohf(msgptr);
    static_assert(sizeof(uint32_t) == sizeof(float));
    uint32_t v;
    memcpy(&v, msgptr, sizeof(float));
    #if ( __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap32(v);
    #endif
    float f;
    memcpy(&f, &v, sizeof(float));
    return f;        
}

/* function specializations for double */

#if (__SIZEOF_DOUBLE__ == 8)
template<> void ByteMessageField<double>::set(double value) {
//    htond(value, msgptr);
    static_assert(sizeof(uint64_t) == sizeof(double));
    #if ( __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        // convert to unsigned integer, byte-swap, copy result to array
        uint64_t v;
        memcpy(&v, &value, sizeof(double));
        v = __builtin_bswap64(v);
        memcpy(msgptr, &v, sizeof(uint64_t));
    #elif ( __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__ )
        // direct copy of double to array
        memcpy(msgptr, &value, sizeof(double));
    #else
        #error "float word order could not be determined!" 
    #endif
}

template<> double ByteMessageField<double>::get(void) const {
//    return ntohd(msgptr);
    static_assert(sizeof(uint64_t) == sizeof(double));
    uint64_t v;
    memcpy(&v, msgptr, sizeof(double));
    #if ( __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
        v = __builtin_bswap64(v);
    #endif
    double d;
    memcpy(&d, &v, sizeof(double));
    return d;        
}
#endif

/** @endcond */

/** @cond class_specialization_bool */
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
/** @endcond */
