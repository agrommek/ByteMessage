/*
  ByteMessage library for Arduino

  This sketch constitutes a unit test for the library.
  This example code is in the public domain.
*/

// ByteMessage and contained objects
#include <ByteMessage.h>
#include <ByteMessageField.h>
#include <ByteMessageChecksum.h>
#include <ByteMessageFieldBlob.h>

// checksum functions
#include <bm_checksum_fletcher.h>
#include <bm_checksum_luhn.h>
#include <bm_checksum_onesum.h>
#include <bm_checksum_twosum.h>
#include <bm_checksum_xor.h>

// function to test ByteMessageField instances
template <typename T>
uint8_t unittest_ByteMessageField(T testvalue) {
    uint32_t errorcounter = 0;
    const size_t s = ByteMessageField<T>::size;
    uint8_t arr[s] = {0};

    ByteMessageField<T> bmf{arr, 0};
    /* first test: set value and return it */
    Serial.print(F("Testing set()/get():         "));
    bmf.set(testvalue);
    unittest_message( bmf.get() == testvalue, errorcounter);

    /* second test: assignment operater */
    Serial.print(F("Testing assignment operator: "));
    uint8_t arr2[s] = {0};
    ByteMessageField<T> bmf2{arr2, 0};
    bmf2 = bmf;
    unittest_message( bmf.get() == bmf2.get(), errorcounter);
    
    return errorcounter;
}

// function to test checksum functions
// Note: The third parameter is a pointer to a function taking a const uint8_t* and a size_t as parameters and returns a T.
template <typename T> uint8_t unittest_checksum_function(const uint8_t* in, size_t len, T (*cfp)(const uint8_t*, size_t), T testvalue) {
    uint32_t errorcounter = 0;
    Serial.print(F("len = "));
    Serial.print(len, DEC);
    Serial.print(F(": "));
    const T result = cfp(in, len);
    unittest_message( result == testvalue, errorcounter);
//    Serial.println(result, DEC);
    
    return errorcounter;
}

// helper function to print results 
void unittest_message(bool result, uint32_t &counter) {
    if (result) {
        Serial.println(F("PASS"));
    }
    else {
        Serial.println(F("FAIL"));
        ++counter;
    }
}

void setup() {
    
    // the number of errors during all tests
    uint32_t errorcount = 0;

    Serial.begin(115200);
    while (!Serial);
    Serial.println(F("\n###### Starting unit tests for ByteMessage library! ######\n"));

    /* ---- ByteMessageField ---- */

    Serial.println(F("\n### Running unit tests for ByteMessageField class ###\n"));
    
    Serial.println(F("\nTest ByteMessageField<uint8_t>..."));
    errorcount += unittest_ByteMessageField<uint8_t>(170); /// 170 = 0b10101010

    Serial.println(F("\nTest ByteMessageField<int8_t> with negative number..."));
    errorcount += unittest_ByteMessageField<int8_t>(-99);

    Serial.println(F("\nTest ByteMessageField<int8_t> with positive number..."));
    errorcount += unittest_ByteMessageField<int8_t>(99);


    Serial.println(F("\nTest ByteMessageField<uint16_t>..."));
    errorcount += unittest_ByteMessageField<uint16_t>(43690); // 43690 = 0b1010101010101010

    Serial.println(F("\nTest ByteMessageField<int16_t> with negative number..."));
    errorcount += unittest_ByteMessageField<int16_t>(-5555);

    Serial.println(F("\nTest ByteMessageField<int16_t> with positive number..."));
    errorcount += unittest_ByteMessageField<int16_t>(5555);
    

    Serial.println(F("\nTest ByteMessageField<uint32_t>..."));
    errorcount += unittest_ByteMessageField<uint32_t>(0xAAAAAAAA); // 0b1010101010101010

    Serial.println(F("\nTest ByteMessageField<int32_t> with negative number..."));
    errorcount += unittest_ByteMessageField<int32_t>(-262167);

    Serial.println(F("\nTest ByteMessageField<int32_t> with positive number..."));
    errorcount += unittest_ByteMessageField<int32_t>(262167);


    Serial.println(F("\nTest ByteMessageField<uint64_t>..."));
    errorcount += unittest_ByteMessageField<uint64_t>(0xAAAAAAAAAAAAAAAA);

    Serial.println(F("\nTest ByteMessageField<int64_t> with negative number..."));
    errorcount += unittest_ByteMessageField<int64_t>(-56920584707903424);

    Serial.println(F("\nTest ByteMessageField<int64_t> with positive number..."));
    errorcount += unittest_ByteMessageField<int64_t>(56920584707903424);


    Serial.println(F("\nTest ByteMessageField<float> with 0..."));
    errorcount += unittest_ByteMessageField<float>(0.0);

    constexpr float pi_float = 3.1415926535897932384626433832795;
    Serial.println(F("\nTest ByteMessageField<float> with negative number..."));
    errorcount += unittest_ByteMessageField<int64_t>(-pi_float);

    Serial.println(F("\nTest ByteMessageField<float> with positive number..."));
    errorcount += unittest_ByteMessageField<float>(pi_float);


    #if (__SIZEOF_DOUBLE__ == 8)
    Serial.println(F("\nTest ByteMessageField<double> with 0..."));
    errorcount += unittest_ByteMessageField<double>(0.0);

    constexpr double pi_double = 3.1415926535897932384626433832795;
    Serial.println(F("\nTest ByteMessageField<double> with negative number..."));
    errorcount += unittest_ByteMessageField<double>(-pi_float);

    Serial.println(F("\nTest ByteMessageField<double> with positive number..."));
    errorcount += unittest_ByteMessageField<double>(pi_double);
    #endif


    Serial.println(F("\nTest ByteMessageField<bool> with true value..."));
    errorcount += unittest_ByteMessageField<bool>(true);

    Serial.println(F("\nTest ByteMessageField<bool> with false value..."));
    errorcount += unittest_ByteMessageField<bool>(false);

    /* ---- ByteMessageFieldBlob ---- */

    Serial.println(F("\n### Running unit tests for ByteMessageFieldBlob class ###\n"));

    uint8_t data[16] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115};

    // ByteMessageFieldBlob with 10 bytes
    constexpr size_t size1 = 10;
    uint8_t backend1[size1];
    ByteMessageFieldBlob bmfb1{backend1, 0, size1};
    const uint8_t * ptr1 = bmfb1.get_ptr();

    // ByteMessageFieldBlob with data array as backend, but const
    constexpr size_t size_const = 10;
    const ByteMessageFieldBlob bmfb_const{data, 0, size_const};
    const uint8_t * ptr_const = bmfb_const.get_ptr();
    
    // ByteMessageFieldBlob with same size as backend1
    constexpr size_t size2 = size1;
    uint8_t backend2[size2]; 
    ByteMessageFieldBlob bmfb2{backend2, 0, size2};
    const uint8_t * ptr2 = bmfb2.get_ptr();

    // ByteMessageFieldBlob with smaller size than backend1
    constexpr size_t size3 = size1 - 2;
    uint8_t backend3[size3];
    ByteMessageFieldBlob bmfb3{backend3, 0, size3};
    const uint8_t * ptr3 = bmfb3.get_ptr();

    // ByteMessageFieldBlob with bigger size than backend1
    constexpr size_t size4 = size1 + 2;
    uint8_t backend4[size4];
    ByteMessageFieldBlob bmfb4{backend4, 0, size4};
    const uint8_t * ptr4 = bmfb4.get_ptr();

    // start tests
    Serial.print(F("Check correct size of bmfb1: "));
    unittest_message( bmfb1.size == size1, errorcount);

    Serial.print(F("Filling ByteMessageFieldBlob with exact amount of data: "));
    size_t l;
    l = bmfb1.set(data, size1);
    unittest_message( l == size1, errorcount);

    Serial.print(F("Checking if data in bmfb1 matches the original data: "));
    unittest_message( memcmp(data, ptr1, l) == 0, errorcount);

    Serial.print(F("Filling ByteMessageFieldBlob with MORE data than available space: "));
    l = bmfb1.set(data, sizeof(data)); // only size1 bytes are copied
    unittest_message( l == size1, errorcount);

    Serial.print(F("Checking if data in bmfb1 matches the original data: "));
    unittest_message( memcmp(data, ptr1, l) == 0, errorcount);

    Serial.print(F("Filling ByteMessageFieldBlob with LESS data than available space: "));
    l = bmfb1.set(data, size3); // only 8 bytes are copied, the remaining two bytes are zero-padded
    unittest_message( l == size3, errorcount); 

    Serial.print(F("Checking if data in bmfb1 matches the original data (8 bytes): "));
    unittest_message( memcmp(data, ptr1, l) == 0, errorcount);

    Serial.print(F("Checking for correct zero-padding: "));
    unittest_message( ptr1[8] == 0 && ptr1[9] == 0, errorcount);

    Serial.print(F("Checking setting data bmbf1 to constant value: "));
    bmfb1.set(123);
    bool all_same;
    all_same = true;
    for (size_t i=0; i<bmfb1.size; ++i) {
        if (ptr1[i] != 123) {
            all_same = false;
            break;
        }
    }
    unittest_message( all_same, errorcount);

    Serial.print(F("Test assignment operator with ByteMessageFieldBlob of same size: "));
    bmfb2.set(123);
    bmfb2 = bmfb1;
    unittest_message( memcmp(ptr1, ptr2, bmfb1.size) == 0, errorcount);

    Serial.print(F("Test assignment operator with ByteMessageFieldBlob of smaller size (truncate): "));
    bmfb3.set(123);
    bmfb3 = bmfb1;
    unittest_message( memcmp(ptr1, ptr3, bmfb3.size) == 0, errorcount);

    Serial.print(F("Test assignment operator with ByteMessageFieldBlob of smaller size (pad with 0): "));
    bmfb4.set(123);
    bmfb4 = bmfb1;
    unittest_message( memcmp(ptr1, ptr4, bmfb1.size) == 0 && ptr4[10] == 0 && ptr4[11] == 0, errorcount);

    Serial.print(F("Test regular read-access through subscript operator: "));
    all_same = true;
    for (size_t i=0; i<bmfb1.size; ++i) {
        if (ptr1[i] != bmfb1[i]) {
            all_same = false;
            break;
        }
    }
    unittest_message( all_same, errorcount);    

    Serial.print(F("Test regular write-access through subscript operator: "));
    uint8_t value;
    value = ptr1[0];
    value++;
    bmfb1[0] = value;
    unittest_message(ptr1[0] == value, errorcount);

    Serial.print(F("Test out-of-bounds read-access through subscript operator: "));
    unittest_message(bmfb1[bmfb1.size+55] == 0, errorcount);

    Serial.print(F("Test out-of-bounds write-access through subscript operator: "));
    bmfb1[bmfb1.size+55] = 123;
    unittest_message(bmfb1[bmfb1.size+55] == 0, errorcount);

    Serial.print(F("Test regular read-access on const instance through subscript operator: "));
    unittest_message(bmfb_const[3] == ptr_const[3] && bmfb_const[3] == data[3], errorcount);
    
    Serial.print(F("Test out-of-bounds read-access on const instance through subscript operator: "));
    unittest_message(bmfb_const[size_const+100] == 0, errorcount);

    // The following line does not compile due to const-ness:
    // Subscript assignment is not allowed on const instances!
//    bmfb_const[3] = 10;

    /* ---- checksum functions ---- */

    /*
     * Note:
     * We check all checksum functions with eight different input lengths, because there is some implicit 
     * zero padding going on, depending on the exact function used. For completeness, we do this for every
     * checksum function, even for those where zero padding does not apply.
     * 
     * The checksum values to compare to are calculated by the Python implementation(s) over the same input
     * data.
     */

    // some random data to run the checksum functions on
    const uint8_t msg[] = {
                           0xc1, 0x80, 0x75, 0x39, 0x2d, 0x78, 0xb4, 0x29,
                           0xbc, 0x4e, 0xe2, 0xef, 0x91, 0xe2, 0xcf, 0x60,
                           0x9f, 0x9f, 0x9b, 0x01, 0x1d, 0x7c, 0xba, 0x20,
                           0x18, 0x1b, 0x0c, 0xed, 0xee, 0x15, 0xeb, 0x07,
                           0x49, 0x4c, 0xb2, 0xe3, 0xbc, 0x11, 0x60, 0x82,
                           0x18, 0xfb, 0x8f, 0x44, 0xe2, 0x0b, 0x7b, 0xb6,
                           0xe9, 0x0b, 0x31, 0xa7, 0xb7, 0xc6, 0xdd, 0x34,
                           0xc5, 0x56, 0x60, 0xa9, 0x17, 0x2b, 0x7e, 0x5a,
                           0xae, 0x4e, 0x20, 0xe7, 0x7a, 0x5f, 0x56, 0x69,
                           0x98, 0x1d, 0x88, 0x97, 0x3a, 0x96, 0x16, 0xda,
                           0x79, 0x9e, 0x03, 0xfb, 0x54, 0x75, 0xe0, 0xfa,
                           0x12, 0x50, 0xd8, 0x73, 0x6c, 0x17, 0xf1, 0x7b,
                           0x9d, 0x11, 0x08, 0x55, 0x8f, 0x6a, 0xbc, 0x4c,
                           0x71, 0xb6, 0x3d, 0x7f, 0xf3, 0x7b, 0x2f, 0xf0,
                           0x30, 0x84, 0x42, 0xc2, 0x09, 0xd6, 0xe0, 0x84,
                           0x43, 0xb2, 0xca, 0xef, 0x49, 0x80, 0xfc, 0x60
                           };
    static_assert(sizeof(msg) == 128);
    
    Serial.println(F("\n### Running unit tests for checksum functions ###\n"));

    Serial.println(F("\nfletcher8_checksum:"));
    errorcount += unittest_checksum_function<uint8_t>(msg, 128, &fletcher8_checksum, 74);
    errorcount += unittest_checksum_function<uint8_t>(msg, 127, &fletcher8_checksum, 228);
    errorcount += unittest_checksum_function<uint8_t>(msg, 126, &fletcher8_checksum, 55);
    errorcount += unittest_checksum_function<uint8_t>(msg, 125, &fletcher8_checksum, 78);
    errorcount += unittest_checksum_function<uint8_t>(msg, 124, &fletcher8_checksum, 1);
    errorcount += unittest_checksum_function<uint8_t>(msg, 123, &fletcher8_checksum, 210);
    errorcount += unittest_checksum_function<uint8_t>(msg, 122, &fletcher8_checksum, 74);
    errorcount += unittest_checksum_function<uint8_t>(msg, 121, &fletcher8_checksum, 28);

    Serial.println(F("\nfletcher16_checksum:"));
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &fletcher16_checksum, 46556);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &fletcher16_checksum, 55420);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &fletcher16_checksum, 23679);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &fletcher16_checksum, 56574);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &fletcher16_checksum, 56757);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &fletcher16_checksum, 10437);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &fletcher16_checksum, 25338);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &fletcher16_checksum, 26440);

    Serial.println(F("\nfletcher_checksum:"));
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &fletcher_checksum, 46556);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &fletcher_checksum, 55420);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &fletcher_checksum, 23679);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &fletcher_checksum, 56574);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &fletcher_checksum, 56757);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &fletcher_checksum, 10437);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &fletcher_checksum, 25338);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &fletcher_checksum, 26440);

    Serial.println(F("\nfletcher32_checksum:"));
    errorcount += unittest_checksum_function<uint32_t>(msg, 128, &fletcher32_checksum, 2091923110);
    errorcount += unittest_checksum_function<uint32_t>(msg, 127, &fletcher32_checksum, 2085631558);
    errorcount += unittest_checksum_function<uint32_t>(msg, 126, &fletcher32_checksum, 1175075397);
    errorcount += unittest_checksum_function<uint32_t>(msg, 125, &fletcher32_checksum, 1166686661);
    errorcount += unittest_checksum_function<uint32_t>(msg, 124, &fletcher32_checksum, 197521604);
    errorcount += unittest_checksum_function<uint32_t>(msg, 123, &fletcher32_checksum, 181858261);
    errorcount += unittest_checksum_function<uint32_t>(msg, 122, &fletcher32_checksum, 452994517);
    errorcount += unittest_checksum_function<uint32_t>(msg, 121, &fletcher32_checksum, 441328931);

    Serial.println(F("\nluhn256_checksum:"));
    errorcount += unittest_checksum_function<uint8_t>(msg, 128, &luhn256_checksum, 192);
    errorcount += unittest_checksum_function<uint8_t>(msg, 127, &luhn256_checksum, 138);
    errorcount += unittest_checksum_function<uint8_t>(msg, 126, &luhn256_checksum, 124);
    errorcount += unittest_checksum_function<uint8_t>(msg, 125, &luhn256_checksum, 3);
    errorcount += unittest_checksum_function<uint8_t>(msg, 124, &luhn256_checksum, 198);
    errorcount += unittest_checksum_function<uint8_t>(msg, 123, &luhn256_checksum, 132);
    errorcount += unittest_checksum_function<uint8_t>(msg, 122, &luhn256_checksum, 111);
    errorcount += unittest_checksum_function<uint8_t>(msg, 121, &luhn256_checksum, 203);


    Serial.println(F("\nonesum8_checksum:"));
    errorcount += unittest_checksum_function<uint8_t>(msg, 128, &onesum8_checksum, 35);
    errorcount += unittest_checksum_function<uint8_t>(msg, 127, &onesum8_checksum, 131);
    errorcount += unittest_checksum_function<uint8_t>(msg, 126, &onesum8_checksum, 128);
    errorcount += unittest_checksum_function<uint8_t>(msg, 125, &onesum8_checksum, 1);
    errorcount += unittest_checksum_function<uint8_t>(msg, 124, &onesum8_checksum, 74);
    errorcount += unittest_checksum_function<uint8_t>(msg, 123, &onesum8_checksum, 58);
    errorcount += unittest_checksum_function<uint8_t>(msg, 122, &onesum8_checksum, 5);
    errorcount += unittest_checksum_function<uint8_t>(msg, 121, &onesum8_checksum, 183);

    Serial.println(F("\nonesum16_checksum:"));
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &onesum16_checksum, 51545);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &onesum16_checksum, 51641);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &onesum16_checksum, 50618);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &onesum16_checksum, 50746);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &onesum16_checksum, 3899);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &onesum16_checksum, 4138);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &onesum16_checksum, 55850);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &onesum16_checksum, 56028);

    Serial.println(F("\ninternet_checksum:")); // alias for onesum16_checksum...
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &internet_checksum, 51545);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &internet_checksum, 51641);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &internet_checksum, 50618);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &internet_checksum, 50746);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &internet_checksum, 3899);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &internet_checksum, 4138);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &internet_checksum, 55850);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &internet_checksum, 56028);

    Serial.println(F("\nonesum32_checksum:"));
    errorcount += unittest_checksum_function<uint32_t>(msg, 128, &onesum32_checksum, 3769952420);
    errorcount += unittest_checksum_function<uint32_t>(msg, 127, &onesum32_checksum, 3769952516);
    errorcount += unittest_checksum_function<uint32_t>(msg, 126, &onesum32_checksum, 3770017028);
    errorcount += unittest_checksum_function<uint32_t>(msg, 125, &onesum32_checksum, 3778405636);
    errorcount += unittest_checksum_function<uint32_t>(msg, 124, &onesum32_checksum, 708175109);
    errorcount += unittest_checksum_function<uint32_t>(msg, 123, &onesum32_checksum, 708175348);
    errorcount += unittest_checksum_function<uint32_t>(msg, 122, &onesum32_checksum, 708227060);
    errorcount += unittest_checksum_function<uint32_t>(msg, 121, &onesum32_checksum, 719892468);

    Serial.println(F("\nsum8_checksum:"));
    errorcount += unittest_checksum_function<uint8_t>(msg, 128, &sum8_checksum, 158);
    errorcount += unittest_checksum_function<uint8_t>(msg, 127, &sum8_checksum, 62);
    errorcount += unittest_checksum_function<uint8_t>(msg, 126, &sum8_checksum, 66);
    errorcount += unittest_checksum_function<uint8_t>(msg, 125, &sum8_checksum, 194);
    errorcount += unittest_checksum_function<uint8_t>(msg, 124, &sum8_checksum, 121);
    errorcount += unittest_checksum_function<uint8_t>(msg, 123, &sum8_checksum, 138);
    errorcount += unittest_checksum_function<uint8_t>(msg, 122, &sum8_checksum, 192);
    errorcount += unittest_checksum_function<uint8_t>(msg, 121, &sum8_checksum, 14);

    Serial.println(F("\nsum16_checksum:"));
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &sum16_checksum, 13958);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &sum16_checksum, 13862);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &sum16_checksum, 14886);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &sum16_checksum, 14758);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &sum16_checksum, 61606);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &sum16_checksum, 61367);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &sum16_checksum, 9655);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &sum16_checksum, 9477);

    Serial.println(F("\nsum32_checksum:"));
    errorcount += unittest_checksum_function<uint32_t>(msg, 128, &sum32_checksum, 525014860);
    errorcount += unittest_checksum_function<uint32_t>(msg, 127, &sum32_checksum, 525014764);
    errorcount += unittest_checksum_function<uint32_t>(msg, 126, &sum32_checksum, 524950252);
    errorcount += unittest_checksum_function<uint32_t>(msg, 125, &sum32_checksum, 516561644);
    errorcount += unittest_checksum_function<uint32_t>(msg, 124, &sum32_checksum, 3586792172);
    errorcount += unittest_checksum_function<uint32_t>(msg, 123, &sum32_checksum, 3586791933);
    errorcount += unittest_checksum_function<uint32_t>(msg, 122, &sum32_checksum, 3586740221);
    errorcount += unittest_checksum_function<uint32_t>(msg, 121, &sum32_checksum, 3575074813);

    Serial.println(F("\nsum64_checksum:"));
    errorcount += unittest_checksum_function<uint64_t>(msg, 128, &sum64_checksum, 11208524911326685518U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 127, &sum64_checksum, 11208524911326685422U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 126, &sum64_checksum, 11208524911326620910U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 125, &sum64_checksum, 11208524911318232302U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 124, &sum64_checksum, 11208524910093495534U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 123, &sum64_checksum, 11208523883596311790U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 122, &sum64_checksum, 11208301782247501038U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 121, &sum64_checksum, 11158199236393004270U);

    Serial.println(F("\nxor8_checksum:"));
    errorcount += unittest_checksum_function<uint8_t>(msg, 128, &xor8_checksum, 40);
    errorcount += unittest_checksum_function<uint8_t>(msg, 127, &xor8_checksum, 72);
    errorcount += unittest_checksum_function<uint8_t>(msg, 126, &xor8_checksum, 180);
    errorcount += unittest_checksum_function<uint8_t>(msg, 125, &xor8_checksum, 52);
    errorcount += unittest_checksum_function<uint8_t>(msg, 124, &xor8_checksum, 125);
    errorcount += unittest_checksum_function<uint8_t>(msg, 123, &xor8_checksum, 146);
    errorcount += unittest_checksum_function<uint8_t>(msg, 122, &xor8_checksum, 88);
    errorcount += unittest_checksum_function<uint8_t>(msg, 121, &xor8_checksum, 234);

    Serial.println(F("\nxor16_checksum:"));
    errorcount += unittest_checksum_function<uint16_t>(msg, 128, &xor16_checksum, 24182);
    errorcount += unittest_checksum_function<uint16_t>(msg, 127, &xor16_checksum, 24086);
    errorcount += unittest_checksum_function<uint16_t>(msg, 126, &xor16_checksum, 41494);
    errorcount += unittest_checksum_function<uint16_t>(msg, 125, &xor16_checksum, 41622);
    errorcount += unittest_checksum_function<uint16_t>(msg, 124, &xor16_checksum, 60310);
    errorcount += unittest_checksum_function<uint16_t>(msg, 123, &xor16_checksum, 60281);
    errorcount += unittest_checksum_function<uint16_t>(msg, 122, &xor16_checksum, 8569);
    errorcount += unittest_checksum_function<uint16_t>(msg, 121, &xor16_checksum, 8651);

    Serial.println(F("\nxor32_checksum:"));
    errorcount += unittest_checksum_function<uint32_t>(msg, 128, &xor32_checksum, 2957307442);
    errorcount += unittest_checksum_function<uint32_t>(msg, 127, &xor32_checksum, 2957307474);
    errorcount += unittest_checksum_function<uint32_t>(msg, 126, &xor32_checksum, 2957251154);
    errorcount += unittest_checksum_function<uint32_t>(msg, 125, &xor32_checksum, 2965639762);
    errorcount += unittest_checksum_function<uint32_t>(msg, 124, &xor32_checksum, 4190376530);
    errorcount += unittest_checksum_function<uint32_t>(msg, 123, &xor32_checksum, 4190376637);
    errorcount += unittest_checksum_function<uint32_t>(msg, 122, &xor32_checksum, 4190427325);
    errorcount += unittest_checksum_function<uint32_t>(msg, 121, &xor32_checksum, 4185315517);

    Serial.println(F("\nxor64_checksum:"));
    errorcount += unittest_checksum_function<uint64_t>(msg, 128, &xor64_checksum, 5020566273477069918U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 127, &xor64_checksum, 5020566273477069886U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 126, &xor64_checksum, 5020566273477095486U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 125, &xor64_checksum, 5020566273468706878U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 124, &xor64_checksum, 5020566272512405566U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 123, &xor64_checksum, 5020566371296653374U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 122, &xor64_checksum, 5020502599622242366U);
    errorcount += unittest_checksum_function<uint64_t>(msg, 121, &xor64_checksum, 4980533152929329214U);

    /* ---- ByteMessage objects ---- */

    Serial.println(F("\n### Running unit tests for ByteMessage class ###\n"));

    constexpr uint8_t BM_TYPE = 1;
    constexpr size_t BM_SIZE = 15;

    class UnitTestMessage : public ByteMessage<BM_TYPE, BM_SIZE> {
        public:
            // create a copy constructor which does nothing
            UnitTestMessage(const UnitTestMessage &src) : ByteMessage{src} {};

            // if we provide a copy constructor, we also *have* to provide a default constructor...
            UnitTestMessage() = default;

            // explicitly use default copy assignment operator
            UnitTestMessage& operator= (const UnitTestMessage&) = default;
    
            // index 0 --> implicit type byte
            ByteMessageField<uint32_t>    foo{msgarr, 1}; // index 1, 2, 3, 4
            ByteMessageField<uint32_t>    bar{msgarr, 5}; // index 5, 6, 7, 8
            ByteMessageField<uint32_t>    baz{msgarr, 9}; // index 9, 10, 11, 12
            ByteMessageChecksum<uint16_t> checksum{msgarr, 13, &internet_checksum}; // index 13
    };

    Serial.print(F("Checking that UnitTest class has correct 'size' property: "));
    unittest_message(UnitTestMessage::size == BM_SIZE, errorcount);
    
    Serial.print(F("Checking that UnitTest class has correct 'type' property: "));
    unittest_message(UnitTestMessage::type == BM_TYPE, errorcount);

    // create a UnitTest object.
    UnitTestMessage utm;
    const uint8_t* utm_ptr = utm.get_ptr();

    Serial.print(F("Checking that UnitTest instance has correct 'size' property: "));
    unittest_message(utm.size == BM_SIZE, errorcount);
    
    Serial.print(F("Checking that UnitTest instance has correct 'type' property: "));
    unittest_message(utm.type == BM_TYPE, errorcount);

    // create an array containing only zeros, except at first position (contains type)
    uint8_t empty_data[BM_SIZE];
    empty_data[0] = BM_TYPE;
    for (size_t i=1; i<BM_SIZE; ++i) { empty_data[i] = 0; } // make absolutely sure that array contains only zeros

    Serial.print(F("Check that underlying array of ByteMessage object is properly initialized: "));
    unittest_message(memcmp(empty_data, utm_ptr, utm.size) == 0, errorcount);
    
    uint8_t data_array[BM_SIZE];
    for (size_t i=0; i<BM_SIZE; ++i) { data_array[i] = random(0, 256); }

    bool populate_ok;
    
    Serial.print(F("Checking that 'populate()' cannot be used when raw data has wrong length: "));
    data_array[0] = BM_TYPE; // set correct type
    populate_ok = utm.populate(data_array, BM_SIZE-1); // use incorrect length
    unittest_message(!populate_ok && memcmp(empty_data, utm_ptr, utm.size) == 0, errorcount);

    Serial.print(F("Checking that 'populate()' cannot be used when type is wrong: "));
    data_array[0] = BM_TYPE+1; // set incorrect type
    populate_ok = utm.populate(data_array, BM_SIZE); // use correct length
    unittest_message(!populate_ok && memcmp(empty_data, utm_ptr, utm.size) == 0, errorcount);
    
    Serial.print(F("Checking that 'populate()' cannot be used when both raw data has wrong length and type is wrong: "));
    data_array[0] = BM_TYPE+1; // set incorrect type
    populate_ok = utm.populate(data_array, BM_SIZE-1); // use incorrect length
    unittest_message(!populate_ok && memcmp(empty_data, utm_ptr, utm.size) == 0, errorcount);
    
    Serial.print(F("Checking that 'populate()' works when given a well-formed array: "));
    data_array[0] = BM_TYPE; // set correct type
    populate_ok = utm.populate(data_array, BM_SIZE); // use correct length
    unittest_message(populate_ok && memcmp(data_array, utm_ptr, utm.size) == 0, errorcount);

    Serial.print(F("Testing copy construction of ByteMessage object: "));
    UnitTestMessage utm2{utm};
    const uint8_t* utm2_ptr = utm2.get_ptr();
    unittest_message(utm.size == utm2.size && utm_ptr != utm2_ptr && memcmp(utm_ptr, utm2_ptr, utm.size) == 0, errorcount);

    Serial.print(F("Testing assignment operator of ByteMessage object: "));
    UnitTestMessage utm3;
    const uint8_t* utm3_ptr = utm2.get_ptr();
    utm3 = utm;
    unittest_message(utm.size == utm3.size && utm_ptr != utm3_ptr && memcmp(utm_ptr, utm3_ptr, utm.size) == 0, errorcount);

    Serial.print(F("Testing read-only subscript operator for ByteMessage object: "));
    unittest_message(utm[0] == BM_TYPE && utm[1] == data_array[1] && utm[utm.size+1] == 0, errorcount);

    /* ---- final evaluation ---- */
        
    // force at least one test to fail for testing...
//    unittest_message(false, errorcount);

    Serial.print(F("\n\n"));
    if (errorcount == 0) {
        Serial.println(F("\t###########################################"));
        Serial.println(F("\t#   All unit tests PASSED successfully!   #"));
        Serial.println(F("\t###########################################"));
    }
    else {
        Serial.println(F("\t###################################################"));
        Serial.println(F("\t   Some unit tests FAILED!"));
        Serial.print  (F("\t   There were a total of "));
        Serial.print  (errorcount, DEC);
        Serial.println(F(" FAILED unit test(s)!"));
        Serial.println(F("\t###################################################"));
    }
}

void loop() {
    // do nothing
}
