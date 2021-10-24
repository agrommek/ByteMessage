#ifndef sumChecksum_h
#define sumChecksum_h

/* regular two's complement sum over different sizes */
#include <stddef.h> // needed for size_t data type
#include <stdint.h> // needed for fixed size integer data types

uint8_t sum8_checksum(const uint8_t * data, size_t length);
uint16_t sum16_checksum(const uint8_t * data, size_t length);
uint32_t sum32_checksum(const uint8_t * data, size_t length);
uint64_t sum64_checksum(const uint8_t * data, size_t length);

#endif
