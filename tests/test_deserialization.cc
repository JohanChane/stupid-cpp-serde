#include <gtest/gtest.h>
#include <scs/serializer.h>
#include <vector>
#include <string>

using namespace scs;

// Test deserialization of primitive types
TEST(DeserializationTest, PrimitiveTypes) {
    std::vector<char> buffer;
    
    // Serialize test data
    int32_t original_int = 42;
    double original_double = 3.14159;
    Encoding<int32_t>::write_payload(buffer, original_int);
    Encoding<double>::write_payload(buffer, original_double);

    // Deserialize
    const char* data = buffer.data();
    auto deserialized_int = Encoding<int32_t>::read_payload(data);
    auto deserialized_double = Encoding<double>::read_payload(data);

    EXPECT_EQ(original_int, deserialized_int);
    EXPECT_DOUBLE_EQ(original_double, deserialized_double);
}

// Test string deserialization
TEST(DeserializationTest, StringType) {
    std::vector<char> buffer;
    std::string original = "Hello, SCS!";
    
    Encoding<std::string>::write_payload(buffer, original);

    const char* data = buffer.data();
    auto deserialized = Encoding<std::string>::read_payload(data);

    EXPECT_EQ(original, deserialized);
}

// Test nested structure deserialization
TEST(DeserializationTest, NestedStructure) {
    struct Address {
        std::string street;
        int zip_code;
        SCS_STRUCTURE(Address, street, zip_code);
    };

    struct Person {
        std::string name;
        Address address;
        SCS_STRUCTURE(Person, name, address);
    };

    // Prepare test data
    Person original;
    original.name = "Alice";
    original.address.street = "123 Main St";
    original.address.zip_code = 10001;

    // Serialize
    auto buffer = Serializer<Person>::serialize(original);

    // Deserialize
    Person deserialized = Serializer<Person>::deserialize(buffer);

    // Verify
    EXPECT_EQ(original.name, deserialized.name);
    EXPECT_EQ(original.address.street, deserialized.address.street);
    EXPECT_EQ(original.address.zip_code, deserialized.address.zip_code);
}

// Test container deserialization
TEST(DeserializationTest, ContainerTypes) {
    std::vector<char> buffer;
    std::vector<int> original_vec = {1, 2, 3, 4, 5};
    std::vector<std::string> original_str_vec = {"a", "bb", "ccc"};

    Encoding<std::vector<int>>::write_payload(buffer, original_vec);
    Encoding<std::vector<std::string>>::write_payload(buffer, original_str_vec);

    const char* data = buffer.data();
    auto deserialized_vec = Encoding<std::vector<int>>::read_payload(data);
    auto deserialized_str_vec = Encoding<std::vector<std::string>>::read_payload(data);

    EXPECT_EQ(original_vec, deserialized_vec);
    EXPECT_EQ(original_str_vec, deserialized_str_vec);
}

// Test map deserialization
TEST(DeserializationTest, MapType) {
    std::vector<char> buffer;
    std::map<int, std::string> original_map = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    Encoding<std::map<int, std::string>>::write_payload(buffer, original_map);

    const char* data = buffer.data();
    auto deserialized_map = Encoding<std::map<int, std::string>>::read_payload(data);

    EXPECT_EQ(original_map, deserialized_map);
}