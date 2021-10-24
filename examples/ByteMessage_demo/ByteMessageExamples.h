#ifndef ByteMessageExamples_h
#define ByteMessageExamples_h


// Note: use "some_header.h" instad of <some_header.h> here in order for this 
// example header file to work when the header files included are in the same
// directory and not in a standard search path.
// Usually, you should use the <some_header.h> form.
#include "ByteMessage.h"
#include "ByteMessageField.h"
#include "ByteMessageChecksum.h"
#include "ByteMessageFieldBlob.h"

#include "bm_checksum_luhn.h"
#include "bm_checksum_twosum.h"
#include "bm_checksum_xor.h"


/*
 * Example 1:
 * 
 * Define a point in 3D cartesian space. The three data fields are the
 * three coordinates (x, y, z). The message is integrity-protected by
 * a Luhn's mod N checksum with length of one byte.
 * 
 * Total message length is 14 bytes:
 * 
 * implicit type byte       1 byte at index 0
 * x coordinate (float)     4 bytes starting at index 1
 * y coordinate (float)     4 bytes starting at index 5
 * z coordinate (float)     4 bytes starting at index 9
 * Luhn checksum (uint8_t)  1 byte at index 13
 * 
 * Message Type shall be 21 for this example
 */
namespace {
    constexpr size_t Point3D_TYPE = 21;
    constexpr size_t Point3D_SIZE = 14;
}

class Point3D : public ByteMessage<Point3D_TYPE, Point3D_SIZE> {
    public:
        // create a copy constructor which does nothing
        Point3D(const Point3D &src) : ByteMessage{src} {};
        // if we provide a copy constructor, we also *have* to provide a default constructor...
        Point3D() = default;
        // explicitly use default copy assignment operator
        Point3D& operator= (const Point3D&) = default;

        // index 0 --> implicit type byte
        ByteMessageField<float>      x{msgarr, 1}; // index 1, 2, 3, 4
        ByteMessageField<float>      y{msgarr, 5}; // index 5, 6, 7, 8
        ByteMessageField<float>      z{msgarr, 9}; // index 9, 10, 11, 12
        ByteMessageChecksum<uint8_t> checksum{msgarr, 13, &luhn256_checksum}; // index 13
        
        static_assert(3*sizeof(float)+sizeof(uint8_t)+1 == Point3D_SIZE);
        static_assert(sizeof(uint8_t) == sizeof(luhn256_checksum(nullptr, 0)));
};

/*
 * Example 2:
 * 
 * Message for robot control. Assume robot has two wheels, left and right,
 * and we want to control the speed and directin "tank style", i.e. control
 * each wheel separately. Assume wheel speeds can be in the range -100 to
 * + 100 --> int8_t is a good data type.
 * 
 * We use a 8-bit "sum checksum" (a simple two's complement sum) for
 * integrity checking.
 * 
 * Total message length is 4 bytes:
 * 
 * implicit type byte       1 byte at index 0
 * left (int8_t             1 byte at index 1
 * right (int8_t            1 byte at index 2
 * sum checksum (uint8_t)   1 byte at index 3
 * 
 * Message Type shall be 30 for this example
 */
namespace {
    constexpr size_t TankControl_TYPE = 30;
    constexpr size_t TankControl_SIZE = 4;
}

class TankControl : public ByteMessage<TankControl_TYPE, TankControl_SIZE> {
    public:
        // create a copy constructor which does nothing
        TankControl(const TankControl &src) : ByteMessage{src} {};
        // if we provide a copy constructor, we also *have* to provide a default constructor...
        TankControl() = default;
        // explicitly use default copy assignment operator
        TankControl& operator= (const TankControl &p) = default;

        // index 0 --> implicit type byte
        ByteMessageField<int8_t>     left{msgarr, 1};
        ByteMessageField<int8_t>     right{msgarr, 2};
        ByteMessageChecksum<uint8_t> checksum{msgarr, 13, &sum8_checksum};
        
        static_assert(2*sizeof(int8_t)+sizeof(uint8_t)+1 == TankControl_SIZE);
        static_assert(sizeof(uint8_t) == sizeof(sum8_checksum(nullptr, 0)));
};

/*
 * Example 3
 * 
 * Pack some binary data of arbitrary length into the message, e.g. an 
 * AES encryption key.
 * 
 * An AES key can have a length of 128 bit = 16 bytes. Total message 
 * length is 17 bytes. We do not add a checksum in this case.
 * 
 * Message Type shall be 55 for this example.
 */
namespace {
    constexpr size_t AESkey_TYPE = 55;
    constexpr size_t AESkey_SIZE = 17;
}

class AESkey : public ByteMessage<AESkey_TYPE, AESkey_SIZE> {
    public:
        // create a copy constructor which does nothing
        AESkey(const AESkey &src) : ByteMessage{src} {};
        // if we provide a copy constructor, we also *have* to provide a default constructor...
        AESkey() = default;
        // explicitly use default copy assignment operator
        AESkey& operator= (const AESkey &p) = default;

        // index 0 --> implicit type
        // binary blob named "key" starting at index 1 and 16 bytes length
        ByteMessageFieldBlob key{msgarr, 1, 16};
};

/*
 * Example 4
 * 
 * Message for some sensor values, say temperature in degrees celsius and
 * relative humidity in percent. Let's assume the sensor delivers floating
 * point temperatures and whole number humidity percentages (0 to 100%).
 * 
 * implicit type byte       1 byte at index 0
 * temperature (float)      4 bytes starting at index 1
 * humidity (uint8_t)       1 byte at index 5
 * xor8 checksum (uint8_t)  1 byte at index 6
 * 
 * Message length is 7 bytes total.
 * 
 * Message Type shall be 66 for this example.
 */
namespace {
    constexpr size_t SensorData_TYPE = 66;
    constexpr size_t SensorData_SIZE = 7;
}

class SensorData : public ByteMessage<SensorData_TYPE, SensorData_SIZE> {
    public:
        SensorData(const SensorData &src) : ByteMessage{src} {};
        SensorData() = default;
        SensorData& operator= (const SensorData &p) = default;
        
        ByteMessageField<float>      temperature{msgarr, 1};
        ByteMessageField<uint8_t>    humidity{msgarr, 5};
        ByteMessageChecksum<uint8_t> checksum{msgarr, 6, &xor8_checksum};  
};
#endif
