#ifndef xorChecksum_h
#define xorChecksum_h

#include <stddef.h> // needed for size_t data type
#include <stdint.h> // needed for fixed size integer data types

uint8_t xor8_checksum(const uint8_t * data, size_t length);
uint16_t xor16_checksum(const uint8_t * data, size_t length);
uint32_t xor32_checksum(const uint8_t * data, size_t length);
uint64_t xor64_checksum(const uint8_t * data, size_t length);

#endif
