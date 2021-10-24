#ifndef LuhnChecksum_h
#define LuhnChecksum_h

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

// textbook implementation for bases up to 256
// base = 256 <--> base = 0
uint8_t luhn_checksum_textbook(const uint8_t* ptr, size_t length, uint8_t base=10);

// implementation optimized for speed
// use this one when in doubt
uint8_t luhn_checksum(const uint8_t* ptr, size_t length, uint8_t base=10);

// version optimized for base == 256. 
// May or may not be faster than general implementation.
uint8_t luhn256_checksum(const uint8_t* ptr, size_t length);

#endif

