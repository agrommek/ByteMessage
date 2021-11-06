/*
  ByteMessage library for Arduino

  This sketch constitutes a demo for the library.
  See the included header file on how to define own messages.
  
  This example code is in the public domain.
*/

#include "ByteMessageExamples.h"

void setup() {

    Serial.begin(115200);
    while (!Serial);

    Serial.println(F("\n### Starting demo for ByteMessage objects ###\n"));

    Serial.print(F("Type of Point3D objects: "));
    Serial.println(Point3D::type);

    Serial.print(F("Size of Point3D objects: "));
    Serial.println(Point3D::size);
    
    // create a Point3D instance
    Point3D p1;
    
    // fill some data into Point3D instance
    p1.x.set(11.1);
    p1.y.set(22.2);
    p1.z.set(33.3);

    Serial.println(F("The data which is now stored in p1:"));
    Serial.print(F("x: "));
    Serial.println(p1.x.get(), 6);
    Serial.print(F("y: "));
    Serial.println(p1.y.get(), 6);    
    Serial.print(F("z: "));
    Serial.println(p1.z.get(), 6);
    
    // when done, update checksum
    p1.checksum.update();

    // store checksum in a variable
    // Note: *this* checksum returns a uint8_t. 
    // Other checksum algorithms may return other (i.e. larger) data types.
    uint8_t checksum1 = p1.checksum.get();
    Serial.print(F("Value of checkusm: "));
    Serial.println(checksum1, DEC);

    // iterate over raw data using subscript operator (read-only!!)
    Serial.print(F("raw data: "));
    for (size_t i=0; i<p1.size; ++i) {
        Serial.print(p1[i], DEC);
        Serial.print(F(" "));
    }
    Serial.println();

    // get a read-only pointer to the raw data
    // Note: MUST be const, i.e. pointer to const data
    const uint8_t* p1_ptr = p1.get_ptr();

    // Now we can send the raw data somewhere, for example over a serial connection
    Serial.print(F("raw data of p1 sent over a serial connections (most probably looks like garbage): "));
    Serial.write(p1_ptr, p1.size);
    Serial.println();

    // Imagine that we received the data via serial.
    // For simplicity, we simulate this using memcpy.
    uint8_t data_received[p1.size];
    memcpy(data_received, p1_ptr, p1.size);

    // now let's create a new Point3D object and populate it with the received data
    // populate() will only work of type and size match
    Point3D p2;
    bool populate_ok;
    populate_ok = p2.populate(data_received, p1.size);
    if (populate_ok) {
        Serial.println(F("Successfully populated p2 with received data!"));
    }
    else {
        Serial.println(F("Failed to populate p2 with received data! Type and/or size do not match!"));        
    }

    // verify the checksum, i.e. make sure that transmission went without errors
    Serial.print(F("Checksum of p2: "));
    Serial.println(p2.checksum.get(), DEC);
    bool checksum_ok = p2.checksum.check();
    if (checksum_ok) {
        Serial.println(F("Checksum is ok!"));
    }
    else {
        Serial.println(F("Checksum is NOT ok!"));
    }
    
    // now we can easily access all reveived data
    Serial.println(F("The data we get from p2 (the same as for p1):"));
    Serial.print(F("x: "));
    Serial.println(p2.x.get(), 6);
    Serial.print(F("y: "));
    Serial.println(p2.y.get(), 6);    
    Serial.print(F("z: "));
    Serial.println(p2.z.get(), 6);

    // we can use copy-construction to initialize new ByteMessage objects upon creation
    Point3D p3{p1}; // initialize p3 with data from p1

    // we can use the assignment operator to assign all values from an existing ByteMessage
    // object to another ByteMessage object:
    Point3D p4;
    p4 = p1;

    // Note, however, that we cannot copy-construct or assign between different types of
    // ByteMessage objects. This will not work:
    TankControl tc;
    // the next line results in a compiler error: we cannot assign a Point3D object to a TankControl object!
//    tc = p1;
} // end of setup()

void loop() {
    // nothing to do here...
}
