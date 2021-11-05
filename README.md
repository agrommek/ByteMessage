# ByteMessage Class

## What you can do with this library

Imagine you want to send some binary data to somwhere else, say over a network or serial connection. This can be a nightmare! You have to consider endianness (byte order), sizes of data types and a lot of other things. Wouldn't it be nice to have a tool to build your own message framework in just a few minutes? Well, now you can!  This class provides a framework for designing your own messaging system.

For example, consider a point in 3D space. Each of the three coordinates should be IEEE 754 single precision floating point number. Using this framework you can easily define a class, let's call it `Point3D`, and use it like this:

    Point3D p;      // create Point3D object
    p.x.set(11.1);  // set x coordinate
    p.y.set(22.2);  // set y coordinate
    p.z.set(33.3);  // set z coordinate

    uint8_t* data = p.get_ptr();  // get a read-only pointer to the underlying array of data
    size_t message_size = p.size; // get length of the underlying array

Now you can use `data` and `message_size` to send it somewhere. 

Suppose on the receiving end you have the data in a buffer called `receive_buffer` and the length of the received byte stream was `msglen`. Then you can do somthing like this to extract the coordinates:

    float x, y, z;
    Point3D q;

    // read raw data into buffer
    q.populate(receive_buffer, msglen);

    // get coordinates
    x = q.x.get();
    y = q.y.get();
    z = q.z.get();

Easy, isn't it?

## How to use this library

### General idea

The `ByteMessage` class is a template class with two non-type template parameters: Message type (a `uint8_t`) and message size (`size_t`, in bytes). *Every* message must have a type to distinguish it from, even if you only want a single message. **You have to make sure yourself that each message type is unique in your application!** When calculating the size parameter, do not forget to leave space for the type byte. The type is *always* the first byte in the message.

The class `ByteMessage` is not meant to be used directly. Instead, derive from it and add the data members you want in your derived class:

    class Point2D : public ByteMessage<2, 9>;
    class Point3D : public ByteMessage<3, 13>;

Here, we declared two classes, a `Point2D` of type `2` and a `Point3D` of type `3`. `Point2D` is meant do hold two floating point numbers, `Point3D` should hold three floats. Each float consists of four bytes, so size of `Point2D` is 2 * 4 + 1 = 9 bytes and size of `Point3D` is 3 * 4 + 1 = 13 bytes.

The general idea is that `ByteMessage` objects contain a protected array of `uint8_t` to store all message content. However, `ByteMessage`objects do not have methods to fill in data or extract data from this array. Data is added to and extracted from this array through use of data members which you add in your derived class. The relevant data members have to be given a pointer to this array and a position to write to.


### The ByteMessage class

The following methods exist for this class:

| method / member | description |
|:-------|:------------|
| `static constexpr uint8_t type` | public static constant data to determine the message type |
| `static constexpr size_t size`  | public static constant data to determined the size of the underlying data array in bytes |
| `ByteMessage(void)` | default constructor without parameters |
| `ByteMessage(const ByteMessage &bm)`| copy constructor |
| `virtual ~ByteMessage() = default` | default public virtual destructor |
| `ByteMessage& operator= (const ByteMessage& bm)` | the assignment operator |
| `virutal const uint8_t* get_ptr(void) const final` | return read-only pointer to data |
| `virtual bool populate(const uint8_t * raw_message, size_t message_size) final` | populate message from raw byte array |

### Types of data members to be uses within a ByteMessage object

There are three different types of data members which you can add to your classes derived from `ByteMessage`- they are described in the following sections.

#### ByteMessageField

A `ByteMessageField` is the most basic data member to handle fixed-length primitive data types (signed and unsigned integers, floating point numbers, and booleans). `ByteMessageField` is a template class (i.e. `template <class T> ByteMessageField`, with `T` being the primitive type to store. However, some methods (`get()` and `set()`) are specialized methods and there is *no* generic implementation for them. This means you can only create `ByteMessageField` objects for data types supported by this library. There are `ByteMessageField`s for the following data types.

| `ByteMessageField`           | size (bytes) | description |
|:-----------------------------|:------------:|:------------|
| `ByteMessageField<uint8_t>`  | 1            | unsigned integer |
| `ByteMessageField<uint16_t>` | 2            | unsigned integer |
| `ByteMessageField<uint32_t>` | 3            | unsigned integer |
| `ByteMessageField<uint64_t>` | 4            | unsigned integer |
| `ByteMessageField<int8_t>`   | 1            | signed integer |
| `ByteMessageField<int16_t>`  | 2            | signed integer |
| `ByteMessageField<int32_t>`  | 3            | signed integer |
| `ByteMessageField<int64_t>`  | 4            | signed integer |
| `ByteMessageField<float>`    | 4            | IEEE 745 single precision float |
| `ByteMessageField<double>`   | 8            | IEEE 745 double precision float. Only defined if `double` is really 8 bytes long, not defined if `double` is only an alias for `float` (as is the case with AVR Arduinos). |
| `ByteMessageField<bool>`     | 1            | boolean |

The following methods and data members exist for this class:

| method / member| description |
|:---------------|:------------|
| `static constexpr size_t size` | public static constant data to determine the size of the data field in bytes |
| `ByteMessageField(uint8_t * messagepointer, size_t pos)` | the constructor with *two* mandatory parameters |
| `~ByteMessageField() = default` | default destructor |
| `ByteMessageField& operator= (const ByteMessageField &bmf)` | assignment operator |
| `void set(T value)` | method to set value of field |
| `T get(void) const` | method to retrieve value from field |
| `ByteMessageField(const ByteMessageField &bmf) = delete` | explicitly delete the copy constructor |

The constructor takes two arguments: A pointer to a `uint8_t` array and a position within this array. This is the place where the value is written to by `set()`and retrieved from by `get()`. Example:

    // create an array of uint8_t with 10 elements
    uint8_t arr[10];

    // create a ByteMessageField object, based on arr, starting at position 2
    ByteMessageField<float> bmf{arr, 2};

    size_t s = bmf.size;  // s == 4, as sizeof(float) is four bytes
    bmf.set(13.4);        // write four bytes of float to arr at positions 2, 3, 4, 5
    float f = bmf.get();  // get four bytes of data from arr, starting at position 2, and interprets them as float

    // this line generates a compiler error, as the copy constructor is explicitly deleted
    ByteMessageField<float> bmf2{bmf};

    // however, assignment to an existing ByteMessageField of the same template type works fine
    ByteMessageField<float> bmf3{arr, 6};  // note: same array, but used range does not overlap
    bmf3 = bmf;

Note that different `ByteMessageField`s can (and most often will) share a common array. You have to make sure yourself that the used ranges do not overlap and you do not write out-of-bound of the array!

Data is written to the array in a defined format. The chosen format is big-endian byte order (i.e. network byte order), meaning the byte at the lowest memory address is the most significant byte.

#### ByteMessageChecksum

A `ByteMessageChecksum` represents a checksum value over all preceding bytes in the message. The class is a template class, i.e. `template <class T> ByteMessageChecksum`. The following public members and methods are available:

| method / member| description |
|:---------------|:------------|
| `static constexpr size_t size` | public static constant data to determine the size of the data field in bytes |
| `ByteMessageChecksum(uint8_t* baseptr, size_t position, T (*checksumFunctionPtr)(const uint8_t*, size_t))` | the constructor with *three* mandatory parameters |
| `ByteMessageChecksum(const ByteMessageChecksum &copy) = delete` | explicitly delete the copy constructor |
| `~ByteMessageChecksum(void) = default` | default destructor |
| `ByteMessageChecksum& operator= (const ByteMessageChecksum &bmcs)` | assignment operator |
| `T calc(void) const` | calculate the checksum and return the value, do *not* update internal state |
| `T get(void) const` | return stored message checksum |
| `void update(void)` | calculate the checksum and store the value in message |
| `bool check(void) const` | check if the stored checksum matches the calculated checksum |

The constructor takes *three* parameters. The first and the second parameter are a pointer to a `uint8_t` array and an an offset within this array, respectively. This is similar to the parameters of `ByteMessageField()`. The third parameter is a pointer to a function with return type `T`and taking itself two parameters, a `const uint8_t*` and a `size_t`. All checksum functions included with this library have this kind of function signature (see below).

Note that the checksum is calculated over the values in the `uint8_t`array starting from position 0 up to, but *not* including, the storage position of the checksum value. Values in the array *after* the checksum are not included in the calculation. Thus, it is best to position `ByteMessageChecksum` objects at the end of the array.

Several checksum algorithms with different output sizes have been implemented:

| checksum algorithm     | 1 byte | 2 bytes | 4 bytes | 8 bytes |
|:-----------------------|:-------|:--------|:--------|:--------|
| one's complement sum   | `onesum8_checksum` | `onesum16_checksum` / `internet_checksum` | `onesum32_checksum` | x |
| two's complement sum   | `sum8_checksum` | `sum16_checksum` | `sum32_checksum` | `sum64_checksum` |
| Fletcher's checksum    | `fletcher8_checksum` | fletcher16_checksum / fletcher_checksum | fletcher32_checksum | x |
| XOR checksum           | `xor8_checksum` | `xor16_checksum` | `xor32_checksum` | `xor64_checksum` |
| Luhn's mod256 checksum | `luhn256_checksum` | x | x | x |

All checksum functions have similar signatures: `uintX_t checksum(uint8_t* data, size_t len)`. The checksum functions take a pointer to an array of `uint8_t`s and a length parameters and return the calculated checksum a an unsigned integer of the apropriate size.

Example:

    // create an array of uint8_t with 10 elements
    uint8_t arr[10];

    // create a ByteMessageChecksum object, based on arr, starting at position 8
    // internet_checksum() is a checksum function returning a uint16_t
    ByteMessageChecksum<uint16_t> bmc{arr, 2, &internet_checksum};

    size_t s = bmc.size;       // s == 2, as sizeof(uint16_t) is two bytes
    uint16_t cs = bmc.calc();  // (re-)calculate the checksum over arr[0] to arr[7] using internet_checksum()
    bmf.update();              // (re-)calculate the checksum and store it at arr[8] and arr[9]
    cs = bmc.get();            // get stored checksum from arr[8] and arr[9] without calculating it
    bool ok = bmc.check();     // check if stored checksum matches calculated checksum

    // this line generates a compiler error, as the copy constructor is explicitly deleted
    ByteMessageChecksum<uint16_t> bmc2{bmc};

    // however, assignment to an existing ByteMessageChecksum of the same template type works fine
    ByteMessageChecksum<uint16_t> bmc3{arr, 4};  // note: same array, but used range does not overlap
    bmc3 = bmc;

#### ByteMessageFieldBlob

A `ByteMessageFieldBlob` serves the same function as a `ByteMessageField`, only that it is not defined for primitive data types, but rather for arbitrary binary data. The following public members and methods are available:

| method / member| description |
|:---------------|:------------|
| `const size_t size` | public data to determine the size of the binary data blob in bytes |
| `ByteMessageFieldBlob(uint8_t * messagepointer, size_t pos, size_t bloblength)` | the only constructor with *three* parameters |
| `yteMessageFieldBlob(const ByteMessageFieldBlob &copy) = delete` | explicitly delete the copy constructor |
| `ByteMessageFieldBlob& operator= (const ByteMessageFieldBlob &bmfb)` | assignment operator |
| `size_t set(const uint8_t *data, size_t length)` | copy data from `data` to blob, return number of bytes copied |
| `size_t set(uint8_t value=0)` | set all bytes in blob to a constant value |
| `size_t get(uint8_t * data, size_t length) const` | copy data from blob to `data`, return number of bytes copied |
| `const uint8_t* get_ptr(void) const` | return pointer to constant data |

Whenever data is written to a binary blob (using `set(const uint8_t, size_t)`), the return value indicates the actual number of bytes copied. It is not possible to copy more than `size` bytes. If `length > size`, additional bytes in `uint8_t *data` are silently ignored and the return value of `set()`equals `size`.  If `length < size`, the data blob is padded to the full size with zeros.

Note: As of now, `ByteMessageFieldBlob` is the only non-templated class whithin this library.

## Important notes for deriving from ByteMessage

### Provide a copy constructor for each derived class

When deriving from `ByteMessage<uint8_t TYPE, size_t SIZE>`, you *should*  provide an empty
copy constructor, i.e.

    constexpr uint8_t MESSAGE_TYPE = 10;
    constexpr size_t MESSAGE_SIZE = 20;

    class SomeMessage : public ByteMessage<MESSAGE_TYPE, MESSAGE_SIZE> {
        public:
            // note the empty body of the copy constructor!
            SomeMessage(const SomeMessage &src) : ByteMessage{src} {}
            // see explanation below
            SomeMessage() = default;

            /* add data members here */
    };

Note that we call the copy constructor of the parent class exlicitly in the initializier list. This is necessary to prevent warning messages when compiling with `g++`'s option `-Wextra`.

If you choose not to define a copy constructor (or you forget to do so), copy construction of your derived class will NOT work. This is due to the fact that class members `ByteMessageField`, `ByteMessageChecksum` and `ByteMessageFieldBlob` have their copy constructors explicitly deleted. This makes the copy constructor of your derived class implicitly deleted as well when it contains at least one member of one of those types. By defining an empty copy constructor, we tell the compiler not to try to copy the members by default upon copy construction. The actual data is then copied by the copy constructor of `ByteMessage`, which copies the whole array.

While your derived class will work perfectly without a copy constructor, the compiler will yell at you when you try to use copy construction in your application. It is better to provide one.

IF you provide an empty copy constructor, you **must** also provide at least one constructor. This is a C++ rule: When any constructor is defined, even if it is a "only" a copy constructor, no further constructors are implicitly generated by the compiler. The easiest thing is to just declare a parameter-less constructor as `default` (see code example above).

### Optionally: Declare assignment operator as "= default"

If you want to be explicit, you can also declare a `default`ed assignment operator, like so:

    constexpr uint8_t MESSAGE_TYPE = 10;
    constexpr size_t MESSAGE_SIZE = 20;

    class SomeMessage : public ByteMessage<MESSAGE_TYPE, MESSAGE_SIZE> {
        public:
            SomeMessage(const SomeMessage &src) : ByteMessage{src} {};
            SomeMessage() = default;

            // explicitly declare a default assignment operator
            SomeMessage& operator= (const SomeMessage& sm) = default;

            /* add data members here */
    };

This is strictly optional, the generated code will be the same whether you add this line or not. It is just a friendly reminder that the default assignment operator will do "the right thing": It will first call the assignment operator of the base class (i.e. `ByteMessage<MESSAGE_TYPE, MESSAGE_SIZE>`) which is explicitly defined and does absolutely nothing. Then it will call the assignment operators for all contained data members. As `ByteMessageField`, `ByteMessageChecksum` and `ByteMessageFieldBlob` all have working assignment operators, this will suffice.

## Examples

### Point3D

    constexpr size_t Point3D_TYPE = 21; // pick unique number between 0 and 255
    constexpr size_t Point3D_SIZE = 14; // 1 byte for type, 3*4 bytes for floats, 1 byte for checksum

    class Point3D : public ByteMessage<Point3D_TYPE, Point3D_SIZE> {
        public:
            // create a copy constructor which does nothing
            Point3D(const Point3D &src) : ByteMessage{src} {};
            Point3D() = default;                             // create default constructor
            Point3D& operator= (const Point3D &p) = default; // explicitly use default copy assignment operator

            // index 0 --> implicit type byte
            // msgarr is a proteted member of ByteMessage
            ByteMessageField<float>      x{msgarr, 1};       // index 1, 2, 3, 4
            ByteMessageField<float>      y{msgarr, 5};       // index 5, 6, 7, 8
            ByteMessageField<float>      z{msgarr, 9};       // index 9, 10, 11, 12
            ByteMessageChecksum<uint8_t> checksum{msgarr, 13, &luhn256_checksum}; // index 13
    };

## Compatibility

Although written originally for the Arduino platform, there is nothing which prevents the library from being used on any other platform. However, to determine endianness, some gcc-specific macros are used. Your mileage with other compilers may vary.

Also note that data types `uintX_t` are optional in C++. If they are not defined for your platform and/or compiler, this library will not work.

