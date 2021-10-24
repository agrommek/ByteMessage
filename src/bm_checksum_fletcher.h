#ifndef FletcherChecksum_h
#define FletcherChecksum_h

#include <stddef.h> // needed for size_t data type
#include <stdint.h> // needed for fixed size integer data types

uint8_t fletcher8_checksum(const uint8_t * data, size_t length);
uint16_t fletcher16_checksum(const uint8_t * data, size_t length);
uint32_t fletcher32_checksum(const uint8_t * data, size_t length);

// alias: original fletcher checksum is fletcher16
#define fletcher_checksum fletcher16_checksum

#endif
