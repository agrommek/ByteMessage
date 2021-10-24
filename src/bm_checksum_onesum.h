#ifndef onesum_checksum_h
#define onesum_checksum_h

/* one's complement checksum over different sizes */

#include <stdint.h> // needed for fixed-size data types
#include <stddef.h> // needed for size_t data type

// versions optimized for speed - use these!
uint8_t onesum8_checksum(const uint8_t * data, size_t len);
uint16_t onesum16_checksum(const uint8_t * data, size_t len);
uint32_t onesum32_checksum(const uint8_t * data, size_t len);

// Textbook versions - straightforward implementation, but bad performance.
// Don't use in live code, but only for testing optimized implementations.
uint8_t onesum8_checksum_textbook(const uint8_t * data, size_t len);
uint16_t onesum16_checksum_textbook(const uint8_t * data, size_t len);
uint32_t onesum32_checksum_textbook(const uint8_t * data, size_t len);

// aliases: one's complement sum over 16 bit is the internet checksum
#define internet_checksum onesum16_checksum

#endif
