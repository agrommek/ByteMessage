#include "bm_checksum_onesum.h"

/* One's complement checksum over different sizes */

/* one byte version */

// optimized implementation
uint8_t onesum8_checksum(const uint8_t * data, size_t len) {
    constexpr size_t blocklength_limit = 256;
    uint_fast16_t sum = 0;
    while (len > 0) {
        size_t blocklength = (len > blocklength_limit) ? blocklength_limit : len;
        len -= blocklength;
        while (blocklength > 0) {
            sum += (static_cast<uint_fast16_t>(*data++));
            blocklength -= 1;
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 8) {
            sum = (sum >> 8) + (sum & 0xFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint8_t>(sum));
}

// general implementation straight from the textbook
uint8_t onesum8_checksum_textbook(const uint8_t * data, size_t len) {
    uint_fast16_t  sum = 0;
    while (len > 0) {
        sum += static_cast<uint_fast16_t>(*data++);
        sum = (sum >> 8) + (sum & 0xFF);
        len -= 1;            
    }
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint8_t>(sum));
}

/*
 *  two byte versions 
 * 
 * onesum16_checksum is the internet checksum according to RFC1071
 * (https://datatracker.ietf.org/doc/html/rfc1071)
 *
 * Handle stream of bytes as stream of big-endian 16 bit unsigned
 * numbers. If number of bytes is uneven, pad with zeros in the
 * end.
 * 
 * The result of the one's complement addition is negated to give
 * the final checksum, as demanded by the standard.
 * 
 * Note: The negation of the calculation to give the final checksum
 * is done in order for a byte stream with the checksum appended
 * will result in a checksum equal to 0. However, this is ONLY
 * true if the checksum starts on an even byte number. If one 
 * wanted to append the checksum to a byte stream with uneven
 * length, one would have to insert an 0x00 pad byte first, i.e. 
 * make the implicitly assumed trailing 0-byte explicit. If the
 * checksum is embedded within the byte stream, make sure it starts 
 * on an even-byte-boundary.
 */

// optimized implementation
uint16_t onesum16_checksum(const uint8_t * data, size_t len) {
    // Note: blocklength_limit must be even for the calculate-last-byte logic to work correctly!!
    constexpr size_t blocklength_limit = 2*254;
    uint_fast32_t  sum = 0;
    // handle all complete byte-pairs 
    while (len > 0) {
        // blocklength is even, except maybe during last iteration if len is uneven
        size_t blocklength = (len > blocklength_limit) ? blocklength_limit : len;
        len -= blocklength;
        while (blocklength > 1) {
            sum += (static_cast<uint_fast32_t>(*data++) << 8);
            sum += static_cast<uint_fast32_t>(*data++);
            blocklength -= 2;
        }
        // handle last byte if input length was uneven
        if (blocklength == 1) {
            sum += static_cast<uint_fast32_t>(*data) << 8;
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 16) {
            sum = (sum >> 16) + (sum & 0xFFFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint16_t>(sum));
}

// general implementation straight from the textbook
uint16_t onesum16_checksum_textbook(const uint8_t * data, size_t len) {
    uint_fast32_t  sum = 0;
    while (len > 1) {
        const uint_fast32_t hi_byte = static_cast<uint_fast32_t>(*data++) << 8;
        const uint_fast32_t lo_byte = static_cast<uint_fast32_t>(*data++);
        sum += (hi_byte | lo_byte);
        sum = (sum >> 16) + (sum & 0xFFFF);
        len -= 2;            
    }
    // handle last byte if input length was uneven
    if (len == 1) {
        sum += static_cast<uint_fast32_t>(*data) << 8;
    }
    // Add the two most significant bytes (the carry) 
    // to the two least significant bytes.
    // We only have to do this once when assuming len <= 2*2^16 bytes.
    sum = (sum >> 16) + (sum & 0xFFFF);
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint16_t>(sum));
}

/* four byte version */

// optimized implementation
uint32_t onesum32_checksum(const uint8_t * data, size_t len) {
    // Note: blocklength_limit must be dividable by 4 for the calculate-last-bytes logic to work correctly!!
    constexpr size_t blocklength_limit = 4*254;
    uint_fast64_t  sum = 0;
    uint_fast64_t bytes[4] = {0, 0, 0, 0};
    uint_fast8_t modulus = len & 0x03; // same as len % 4
    // handle all complete byte-quartets
    while (len > 0) {
        // blocklength is dividable by four, except maybe during last iteration if len is uneven
        size_t blocklength = (len > blocklength_limit) ? blocklength_limit : len;
        len -= blocklength;
        while (blocklength > 3) {
            bytes[0] = static_cast<uint_fast64_t>(*data++) << 24;
            bytes[1] = static_cast<uint_fast64_t>(*data++) << 16;
            bytes[2] = static_cast<uint_fast64_t>(*data++) << 8;
            bytes[3] = static_cast<uint_fast64_t>(*data++) << 0;
            sum += (bytes[0] | bytes[1] | bytes[2] | bytes[3]);
            blocklength -= 4;
        }
        // handle remaining bytes if input length was not dividable by four
        for (uint_fast8_t i=0; i<modulus; i++) {
            sum += static_cast<uint_fast16_t>(*data) << (24-(8*i));
        }
        // fold back carry into the sum until there is no more carry
        while (sum >> 32) {
            sum = (sum >> 32) + (sum & 0xFFFFFFFF);
        }
    } // end of outer loop
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint32_t>(sum));
}

// general implementation straight from the textbook
uint32_t onesum32_checksum_textbook(const uint8_t * data, size_t len) {
    uint_fast64_t  sum = 0;
    uint_fast64_t bytes[4] = {0, 0, 0, 0};
    uint_fast8_t modulus = len & 0x03; // same as len % 4
    while (len > 3) {
        bytes[0] = static_cast<uint_fast64_t>(*data++) << 24;
        bytes[1] = static_cast<uint_fast64_t>(*data++) << 16;
        bytes[2] = static_cast<uint_fast64_t>(*data++) << 8;
        bytes[3] = static_cast<uint_fast64_t>(*data++) << 0;
        sum += (bytes[0] | bytes[1] | bytes[2] | bytes[3]);
        sum = (sum >> 32) + (sum & 0xFFFFFFFF);
        len -= 4;            
    }
    // handle remaining bytes if input length was not dividable by four
    for (uint_fast8_t i=0; i<modulus; i++) {
        sum += static_cast<uint_fast16_t>(*data) << (24-(8*i));
    }
    sum = (sum >> 32) + (sum & 0xFFFFFFFF);
    // invert sum (i.e. calculate the one's complement) and return it
    return ~(static_cast<uint32_t>(sum));
}
