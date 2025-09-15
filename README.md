# Stupid C++ Serde

Language: [English](./README.md) | [中文](./README_ZH.md)

## Project Purpose

After studying [libnop](https://github.com/google/libnop), I created a simple demo to implement serialization and deserialization in C++. I hope this helps those who want to understand how C++ handles serialization and deserialization.

## Example

<details>
<summary>basic usage</summary>

```cpp
#include <iostream>
#include <scs/serializer.h>

using namespace scs;

// Test struct definitions
struct Address {
    std::string street;
    std::string city;
    int32_t zip_code;

    SCS_STRUCTURE(Address, street, city, zip_code);
};

struct ContactInfo {
    std::string phone;
    std::string email;
    std::map<int32_t, std::string> phone_extensions;

    SCS_STRUCTURE(ContactInfo, phone, email, phone_extensions);
};

struct Person {
    int32_t id;
    std::string name;
    double salary;
    std::vector<int32_t> scores;
    Address address;
    ContactInfo contact;

    SCS_STRUCTURE(Person, id, name, salary, scores, address, contact);
};

int main() {
    // Initialize test data
    Address address;
    address.street = "123 Main St";
    address.city = "New York";
    address.zip_code = 10001;

    ContactInfo contact;
    contact.phone = "555-1234";
    contact.email = "alice@example.com";
    contact.phone_extensions = {
        {101, "Sales"},
        {102, "Support"},
        {103, "HR"}
    };

    Person person;
    person.id = 123;
    person.name = "Alice";
    person.salary = 50000.50;
    person.scores = {95, 88, 92, 78};
    person.address = address;
    person.contact = contact;

    // Print original object
    std::cout << "Original object:\n";
    Serializer<Person>::debug_print(person);

    // Serialize
    std::vector<char> buffer = Serializer<Person>::serialize(person);
    print_buffer(buffer);

    // Deserialize
    Person deserialized = Serializer<Person>::deserialize(buffer);

    // Print deserialized object
    std::cout << "Deserialized object:\n";
    Serializer<Person>::debug_print(deserialized);

    return 0;
}
```

Output:

```
Original object:
Start printing 6Person object
Field type: int32_t, value: 123
Field type: string, value: "Alice"
Field type: double, value: 50000.5
Field type: vector<int32_t>, value: [95, 88, 92, 78]
Field type: 7Address, value: (nested struct)
  Start printing 7Address object
  Field type: string, value: "123 Main St"
  Field type: string, value: "New York"
  Field type: int32_t, value: 10001
  Print complete

Field type: 11ContactInfo, value: (nested struct)
  Start printing 11ContactInfo object
  Field type: string, value: "555-1234"
  Field type: string, value: "alice@example.com"
  Field type: map<int32_t,string>, value: {101: "Sales", 102: "Support", 103: "HR"}
  Print complete

Print complete

Serialization result (155 bytes):
7b 00 00 00 05 00 00 00 41 6c 69 63 65 00 00 00
00 10 6a e8 40 04 00 00 00 5f 00 00 00 58 00 00
00 5c 00 00 00 4e 00 00 00 1f 00 00 00 0b 00 00
00 31 32 33 20 4d 61 69 6e 20 53 74 08 00 00 00
4e 65 77 20 59 6f 72 6b 11 27 00 00 4b 00 00 00
08 00 00 00 35 35 35 2d 31 32 33 34 11 00 00 00
61 6c 69 63 65 40 65 78 61 6d 70 6c 65 2e 63 6f
6d 03 00 00 00 65 00 00 00 05 00 00 00 53 61 6c
65 73 66 00 00 00 07 00 00 00 53 75 70 70 6f 72
74 67 00 00 00 02 00 00 00 48 52

Deserialized object:
Start printing 6Person object
Field type: int32_t, value: 123
Field type: string, value: "Alice"
Field type: double, value: 50000.5
Field type: vector<int32_t>, value: [95, 88, 92, 78]
Field type: 7Address, value: (nested struct)
  Start printing 7Address object
  Field type: string, value: "123 Main St"
  Field type: string, value: "New York"
  Field type: int32_t, value: 10001
  Print complete

Field type: 11ContactInfo, value: (nested struct)
  Start printing 11ContactInfo object
  Field type: string, value: "555-1234"
  Field type: string, value: "alice@example.com"
  Field type: map<int32_t,string>, value: {101: "Sales", 102: "Support", 103: "HR"}
  Print complete

Print complete
```
</details>

## Build & Install

Build:

```sh
mkdir build && cd build
cmake ..
# OR
#cmake .. -DSCS_BUILD_TESTS=ON -DSCS_BUILD_EXAMPLES=ON
make -j8
```

Install:

```sh
mkdir build && cd build
cmake .. -DSCS_BUILD_TESTS=OFF -DSCS_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
cmake --install .  # Install to system
```

## Binary Encoding for Serialization

See [ref](./doc/serde.md)

## Documentation

See [ref](./doc)
